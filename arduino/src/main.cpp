#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// --- Configuration ---
const char* ssid = "Merckx3";
const char* password = "LievenMarletteEwoutRonald";

// Networking Constants
const char* DEVICE_NAME = "heater";
const char* MULTICAST_IP = "239.0.0.1"; // Standard local multicast range
const uint16_t MULTICAST_PORT = 50000; // Port for multicast communication
const uint16_t UNICAST_PORT = 50001; // Fixed port for this device to receive requests

WiFiUDP udpMulticast;
WiFiUDP udpUnicast;

unsigned long lastAliveTime = 0;
const unsigned long aliveInterval = 5000; // Send "Alive" every 5 seconds

// --- Protocol Functions ---

void sendAlive() {
    JsonDocument doc;
    doc["src"] = DEVICE_NAME;
    doc["typ"] = "AliveEvent";
    JsonObject msg = doc["msg"].to<JsonObject>();
    
    JsonArray services = msg["services"].to<JsonArray>();
    services.add("HeaterRequest");
    
    JsonArray publishes = msg["publishes"].to<JsonArray>();
    publishes.add("HeaterEvent");
    
    msg["subscribes"].to<JsonArray>();

    char buffer[256];
    serializeJson(doc, buffer);

    udpMulticast.beginPacket(MULTICAST_IP, MULTICAST_PORT);
    udpMulticast.write((uint8_t*)buffer, strlen(buffer));
    udpMulticast.endPacket();
    Serial.printf("%d Sent Multicast Alive: %s\n", millis(), buffer);
}

void sendReply(IPAddress remoteIP, uint16_t remotePort, const char* dst) {
    JsonDocument doc;
    doc["src"] = DEVICE_NAME;
    doc["dst"] = dst;
    doc["typ"] = "HeaterReply";
    JsonObject msg = doc["msg"].to<JsonObject>();
    msg["rc"] = 0;
    msg["txt"] = "OK";

    char buffer[256];
    serializeJson(doc, buffer);

    udpUnicast.beginPacket(remoteIP, remotePort);
    udpUnicast.write((uint8_t*)buffer, strlen(buffer));
    udpUnicast.endPacket();
}

void handleUnicast() {
    int packetSize = udpUnicast.parsePacket();
    if (packetSize) {
        char packetBuffer[512];
        int len = udpUnicast.read(packetBuffer, 511);
        packetBuffer[len] = 0;

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, packetBuffer);

        if (!error && doc["typ"] == "HeaterRequest") {
            Serial.print("Received Request from: ");
            Serial.println(doc["src"].as<const char*>());
            
            // Logic: Update heater parameters here
            float temp = doc["msg"]["target_temp"];
            Serial.printf("Setting Temp to: %.2f\n", temp);

            // Send Unicast Reply back to the sender's dynamic port
            sendReply(udpUnicast.remoteIP(), udpUnicast.remotePort(), doc["src"]);
        }
    }
}

// --- Setup & Loop ---

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);


    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
        // show local IP address and start the server
    Serial.println("Connecting to WiFi...");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Start Unicast Listener
    udpUnicast.begin(UNICAST_PORT);

    // Start Multicast Listener/Sender
    udpMulticast.beginMulticast(IPAddress(239, 1, 2, 3), MULTICAST_PORT);
    
    Serial.printf("Listening for Unicast on port %d\n", UNICAST_PORT);
}

void loop() {
    // 1. Send periodic Heartbeat
    if (millis() - lastAliveTime > aliveInterval) {
        sendAlive();
        lastAliveTime = millis();
    }

    // 2. Check for incoming Unicast Requests
    handleUnicast();
}