#include "system.h"
#include "global_include.h"
#include "mavsdk_impl.h"
#include "mavlink_include.h"
#include "system_impl.h"
#include "plugin_impl_base.h"
#include <functional>
#include <algorithm>
#include "px4_custom_mode.h"

// Set to 1 to log incoming/outgoing mavlink messages.
#define MESSAGE_DEBUGGING 0

namespace mavsdk {

using namespace std::placeholders; // for `_1`

System::System(MavsdkImpl& parent) : _system_impl(std::make_shared<SystemImpl>(parent)) {}

System::~System() {}

void System::init(uint8_t system_id, uint8_t component_id, bool connected) const
{
    return _system_impl->init(system_id, component_id, connected);
}

bool System::is_standalone() const
{
    return _system_impl->is_standalone();
}

bool System::has_autopilot() const
{
    return _system_impl->has_autopilot();
}

bool System::has_camera(int camera_id) const
{
    return _system_impl->has_camera(camera_id);
}

bool System::has_gimbal() const
{
    return _system_impl->has_gimbal();
}

bool System::is_connected() const
{
    return _system_impl->is_connected();
}

uint8_t System::get_system_id() const
{
    return _system_impl->get_system_id();
}

void System::subscribe_is_connected(IsConnectedCallback callback)
{
    return _system_impl->subscribe_is_connected(callback);
}

void System::register_component_discovered_callback(DiscoverCallback callback) const
{
    return _system_impl->register_component_discovered_callback(callback);
}

void System::enable_timesync()
{
    _system_impl->enable_timesync();
}

uint64_t System::auto_pilot_time() const
{
    return _system_impl->get_autopilot_time().now().time_since_epoch().count();
}

} // namespace mavsdk
