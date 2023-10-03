#include "rtk_impl.h"
#include "unused.h"

namespace mavsdk {

RtkImpl::RtkImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

RtkImpl::RtkImpl(std::shared_ptr<System> system) : PluginImplBase(std::move(system))
{
    _parent->register_plugin(this);
}

RtkImpl::~RtkImpl()
{
    _parent->unregister_plugin(this);
}

void RtkImpl::init() {}

void RtkImpl::deinit() {}

void RtkImpl::enable() {}

void RtkImpl::disable() {}

Rtk::Result RtkImpl::send_rtcm_data(Rtk::RtcmData rtcm_data)
{
    constexpr size_t field_len = MAVLINK_MSG_GPS_RTCM_DATA_FIELD_DATA_LEN;
    
    mavlink_gps_rtcm_data_t mavlink_rtcm_data;
    
    memset(&mavlink_rtcm_data, 0, sizeof(mavlink_gps_rtcm_data_t));
    
    if (rtcm_data.data.size() < field_len) {
        mavlink_rtcm_data.len = rtcm_data.data.size();
        mavlink_rtcm_data.flags = (_sequence & 0x1F) << 3;
        memcpy(&mavlink_rtcm_data.data, rtcm_data.data.data(), rtcm_data.data.size());
        mavlink_message_t message;
        mavlink_msg_gps_rtcm_data_pack(
            _parent->get_own_system_id(), 
            _parent->get_own_component_id(), 
            &message, 
            mavlink_rtcm_data.flags, 
            mavlink_rtcm_data.len, 
            mavlink_rtcm_data.data);
    } else {
        uint8_t fragment_id = 0;
        int start = 0;
        while (start < rtcm_data.data.size()) {
            int length = std::min(rtcm_data.data.size() - start, field_len);
            mavlink_rtcm_data.flags = 1;
            mavlink_rtcm_data.flags |= fragment_id++ << 1;
            mavlink_rtcm_data.flags |= (_sequence & 0x1F) << 3;
            mavlink_rtcm_data.len = length;
            memcpy(&mavlink_rtcm_data.data, rtcm_data.data.data() + start, length);
            mavlink_message_t message;
            mavlink_msg_gps_rtcm_data_pack(
            _parent->get_own_system_id(), 
            _parent->get_own_component_id(), 
            &message, 
            mavlink_rtcm_data.flags, 
            mavlink_rtcm_data.len, 
            mavlink_rtcm_data.data);
            start += length;
        }
    }
    ++_sequence;
    return Rtk::Result::Success;
}

} // namespace mavsdk
