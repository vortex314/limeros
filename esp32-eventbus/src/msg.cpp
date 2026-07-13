#include <msg.h>

UdpMessage::UdpMessage() : Msg(UdpMessage::name_value)
{
    src = std::nullopt;
    dst = std::nullopt;
    msg_type = std::nullopt;
    req_id = std::nullopt;
    payload = std::nullopt;
}

EndpointAnnounce::EndpointAnnounce() : Msg(EndpointAnnounce::name_value)
{
    endpoint_id = std::nullopt;
    endpoint_name = std::nullopt;
}

EndpointAnnounceReply::EndpointAnnounceReply() : Msg(EndpointAnnounceReply::name_value)
{
    broker_id = std::nullopt;
}
