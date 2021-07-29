#pragma once

#include <mutex>

#include "mavlink_include.h"
#include "plugins/offboard/offboard.h"
#include "plugin_impl_base.h"
#include "system.h"

namespace mavsdk {

class OffboardImpl : public PluginImplBase {
public:
    explicit OffboardImpl(System& system);
    explicit OffboardImpl(std::shared_ptr<System> system);
    ~OffboardImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    Offboard::Result request_offboard();
    Offboard::Result start();
    Offboard::Result stop();

    void request_offboard_async(Offboard::ResultCallback callback);
    void start_async(Offboard::ResultCallback callback);
    void stop_async(Offboard::ResultCallback callback);

    bool is_active();

    Offboard::Result set_position_ned_once(Offboard::PositionNedYaw position_ned_yaw);
    Offboard::Result set_position_ned(Offboard::PositionNedYaw position_ned_yaw);
    Offboard::Result set_velocity_ned(Offboard::VelocityNedYaw velocity_ned_yaw);
    Offboard::Result set_position_velocity_ned(
        Offboard::PositionNedYaw position_ned_yaw, Offboard::VelocityNedYaw velocity_ned_yaw);
    Offboard::Result set_acceleration_ned(Offboard::AccelerationNed acceleration_ned);
    Offboard::Result set_velocity_body_once(Offboard::VelocityBodyYawspeed velocity_body_yawspeed);
    Offboard::Result set_velocity_body(Offboard::VelocityBodyYawspeed velocity_body_yawspeed);
    Offboard::Result set_attitude_once(Offboard::Attitude attitude);
    Offboard::Result set_attitude(Offboard::Attitude attitude);
    Offboard::Result set_attitude_rate_once(Offboard::AttitudeRate attitude_rate);
    Offboard::Result set_attitude_rate(Offboard::AttitudeRate attitude_rate);
    Offboard::Result set_actuator_control_once(Offboard::ActuatorControl actuator_control);
    Offboard::Result set_actuator_control(Offboard::ActuatorControl actuator_control);

    OffboardImpl(const OffboardImpl&);
    OffboardImpl& operator=(const OffboardImpl&) = delete;

private:
    Offboard::Result send_position_ned();
    Offboard::Result send_velocity_ned();
    Offboard::Result send_position_velocity_ned();
    Offboard::Result send_acceleration_ned();
    Offboard::Result send_velocity_body();
    Offboard::Result send_attitude_rate();
    Offboard::Result send_attitude();
    Offboard::Result send_actuator_control();
    Offboard::Result send_actuator_control_message(const float* controls, uint8_t group_number = 0);

    void process_heartbeat(const mavlink_message_t& message);
    void receive_command_result(
        MavlinkCommandSender::Result result, const Offboard::ResultCallback& callback);

    static Offboard::Result
    offboard_result_from_command_result(MavlinkCommandSender::Result result);

    void stop_sending_setpoints();

    Time _time{};

    mutable std::mutex _mutex{};
    enum class Mode {
        NotActive,
        PositionNed,
        VelocityNed,
        PositionVelocityNed,
        AccelerationNed,
        VelocityBody,
        Attitude,
        AttitudeRate,
        ActuatorControl
    } _mode = Mode::NotActive;
    Offboard::PositionNedYaw _position_ned_yaw{};
    Offboard::VelocityNedYaw _velocity_ned_yaw{};
    Offboard::AccelerationNed _acceleration_ned{};
    Offboard::VelocityBodyYawspeed _velocity_body_yawspeed{};
    Offboard::Attitude _attitude{};
    Offboard::AttitudeRate _attitude_rate{};
    Offboard::ActuatorControl _actuator_control{};
    dl_time_t _last_started{};

    void* _call_every_cookie = nullptr;

    const float SEND_INTERVAL_S = 0.05f;
};

} // namespace mavsdk
