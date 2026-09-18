#include "ota_actor.h"

OtaActor::OtaActor()
    : Actor()
{
    _ota_task_handle = nullptr;
}

OtaActor::~OtaActor()
{
}

void OtaActor::setup()
{
    INFO("OTA Actor started");
}

void OtaActor::receive(const ActorMessage &msg)
{
    msg.handle<WifiConnected>([&](const auto &wifi_msg) {
        INFO("OTA received WifiConnected: src=%u dst=%u payload=%p",
             msg.src, msg.dst, (void *)msg.payload.get());
        start_task();
    });
    msg.handle<WifiDisconnected>([&](const auto &wifi_msg) {
        INFO("OTA received WifiDisconnected: src=%u dst=%u payload=%p",
             msg.src, msg.dst, (void *)msg.payload.get());
        stop_task();
    });
}

void OtaActor::start_task()
{
    INFO("Starting OTA TFTP server task...");
    if ( _ota_task_handle == nullptr )
    {
        _running = true;
        BaseType_t rc = xTaskCreate(
            &OtaActor::tftp_ota_server_task, 
            "tftp_ota_server_task", 
            8192, 
            this, 
            5, 
            &_ota_task_handle); 
        if ( rc != pdPASS )
        {
            ERROR("Failed to create OTA TFTP server task: %d", rc);
            _ota_task_handle = nullptr;
        } 
    }
}

void OtaActor::stop_task()
{
    INFO("Stopping OTA TFTP server task...");
    _running = false;
    // The task deletes itself once it notices the stop flag (recvfrom times
    // out within 200 ms). Wait for it so we never delete a dangling handle.
    for (int i = 0; i < 50 && _ota_task_handle != nullptr; ++i)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}



static void tftp_send_ack(int sock,
                          struct sockaddr_in *client,
                          uint16_t block)
{
    uint8_t pkt[4];
    pkt[0] = 0;
    pkt[1] = TFTP_OPCODE_ACK;
    pkt[2] = block >> 8;
    pkt[3] = block & 0xff;

    sendto(sock, pkt, sizeof(pkt), 0,
           (struct sockaddr *)client,
           sizeof(*client));
}

static void tftp_send_error(int sock,
                            struct sockaddr_in *client,
                            uint16_t code,
                            const char *msg)
{
    uint8_t pkt[128];
    size_t len = 4 + strlen(msg) + 1;

    pkt[0] = 0;
    pkt[1] = TFTP_OPCODE_ERROR;
    pkt[2] = code >> 8;
    pkt[3] = code & 0xff;
    strcpy((char *)&pkt[4], msg);

    sendto(sock, pkt, len, 0,
           (struct sockaddr *)client,
           sizeof(*client));
}

void OtaActor::tftp_ota_server_task(void *arg)
{
    OtaActor *actor = static_cast<OtaActor *>(arg);
    int sock = -1;
    uint8_t rx_buf[4 + TFTP_BLOCK_SIZE];
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    esp_ota_handle_t ota_handle = 0;
    const esp_partition_t *ota_part = NULL;

    uint16_t expected_block = 1;
    bool ota_in_progress = false;

    INFO("Starting TFTP OTA server");

    // esp_task_wdt_add(NULL);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0)
    {
        ERROR("socket() failed");
        actor->_ota_task_handle = nullptr;
        vTaskDelete(NULL);
    }

    // Periodically time out of recvfrom() so the task can notice a stop
    // request, close its socket (freeing the TFTP port) and delete itself.
    struct timeval rcv_timeout;
    rcv_timeout.tv_sec = 0;
    rcv_timeout.tv_usec = 1000000; // 1 second
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &rcv_timeout, sizeof(rcv_timeout));

    server.sin_family = AF_INET;
    server.sin_port = htons(TFTP_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        ERROR("bind() failed");
        close(sock);
        actor->_ota_task_handle = nullptr;
        vTaskDelete(NULL);
    }

    while (actor->_running)
    {
        //   esp_task_wdt_reset();
        INFO("Waiting for TFTP packet...");

        int len = recvfrom(sock, rx_buf, sizeof(rx_buf), 0,
                           (struct sockaddr *)&client, &client_len);
        if (len < 0)
        {
            // Stop requested (or recvfrom timeout): exit so the task cleans
            // up its socket and deletes itself.
            if (!actor->_running)
            {
                break;
            }
            continue;
        }
        if (len < 4)
        {
            continue;
        }

        uint16_t opcode = (rx_buf[0] << 8) | rx_buf[1];

        /* ---------- WRQ ---------- */
        if (opcode == TFTP_OPCODE_WRQ)
        {
            INFO("WRQ received, starting OTA");

            ota_part = esp_ota_get_next_update_partition(NULL);
            if (!ota_part)
            {
                tftp_send_error(sock, &client, 1, "No OTA partition");
                continue;
            }

            if (esp_ota_begin(ota_part, OTA_SIZE_UNKNOWN, &ota_handle) != ESP_OK)
            {
                tftp_send_error(sock, &client, 2, "OTA begin failed");
                continue;
            }

            expected_block = 1;
            ota_in_progress = true;

            /* ACK block 0 */
            tftp_send_ack(sock, &client, 0);
        }

        /* ---------- DATA ---------- */
        else if (opcode == TFTP_OPCODE_DATA && ota_in_progress)
        {
            uint16_t block = (rx_buf[2] << 8) | rx_buf[3];
            int data_len = len - 4;

            if (block != expected_block)
            {
                WARN("Unexpected block %u (expected %u)",
                     block, expected_block);
                continue;
            }

            if (esp_ota_write(ota_handle, &rx_buf[4], data_len) != ESP_OK)
            {
                tftp_send_error(sock, &client, 3, "OTA write failed");
                esp_ota_abort(ota_handle);
                ota_in_progress = false;
                continue;
            }

            tftp_send_ack(sock, &client, block);
            expected_block++;

            /* Last packet < 512 bytes */
            if (data_len < TFTP_BLOCK_SIZE)
            {
                INFO("OTA image received, finalizing");

                if (esp_ota_end(ota_handle) == ESP_OK &&
                    esp_ota_set_boot_partition(ota_part) == ESP_OK)
                {

                    INFO("OTA successful, rebooting");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    esp_restart();
                }
                else
                {
                    ERROR("OTA finalize failed");
                }

                ota_in_progress = false;
            }
        }

        /* ---------- Unsupported ---------- */
        else
        {
            tftp_send_error(sock, &client, 4, "Unsupported operation");
        }
    }

    // Clean shutdown: close the socket (frees TFTP_PORT) and delete the task.
    close(sock);
    actor->_ota_task_handle = nullptr;
    vTaskDelete(NULL);
}
