// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/offboard/offboard.proto)

#include "offboard/offboard.grpc.pb.h"
#include "plugins/offboard/offboard.h"

#include "mavsdk.h"
#include "lazy_plugin.h"
#include "log.h"
#include <atomic>
#include <cmath>
#include <future>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

namespace mavsdk {
namespace mavsdk_server {

template<typename Offboard = Offboard, typename LazyPlugin = LazyPlugin<Offboard>>
class OffboardServiceImpl final : public rpc::offboard::OffboardService::Service {
public:
    OffboardServiceImpl(LazyPlugin& lazy_plugin) : _lazy_plugin(lazy_plugin) {}

    template<typename ResponseType>
    void fillResponseWithResult(ResponseType* response, mavsdk::Offboard::Result& result) const
    {
        auto rpc_result = translateToRpcResult(result);

        auto* rpc_offboard_result = new rpc::offboard::OffboardResult();
        rpc_offboard_result->set_result(rpc_result);
        std::stringstream ss;
        ss << result;
        rpc_offboard_result->set_result_str(ss.str());

        response->set_allocated_offboard_result(rpc_offboard_result);
    }

    static std::unique_ptr<rpc::offboard::Attitude>
    translateToRpcAttitude(const mavsdk::Offboard::Attitude& attitude)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::Attitude>();

        rpc_obj->set_roll_deg(attitude.roll_deg);

        rpc_obj->set_pitch_deg(attitude.pitch_deg);

        rpc_obj->set_yaw_deg(attitude.yaw_deg);

        rpc_obj->set_thrust_value(attitude.thrust_value);

        return rpc_obj;
    }

    static mavsdk::Offboard::Attitude
    translateFromRpcAttitude(const rpc::offboard::Attitude& attitude)
    {
        mavsdk::Offboard::Attitude obj;

        obj.roll_deg = attitude.roll_deg();

        obj.pitch_deg = attitude.pitch_deg();

        obj.yaw_deg = attitude.yaw_deg();

        obj.thrust_value = attitude.thrust_value();

        return obj;
    }

    static std::unique_ptr<rpc::offboard::ActuatorControlGroup> translateToRpcActuatorControlGroup(
        const mavsdk::Offboard::ActuatorControlGroup& actuator_control_group)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::ActuatorControlGroup>();

        for (const auto& elem : actuator_control_group.controls) {
            rpc_obj->add_controls(elem);
        }

        return rpc_obj;
    }

    static mavsdk::Offboard::ActuatorControlGroup translateFromRpcActuatorControlGroup(
        const rpc::offboard::ActuatorControlGroup& actuator_control_group)
    {
        mavsdk::Offboard::ActuatorControlGroup obj;

        for (const auto& elem : actuator_control_group.controls()) {
            obj.controls.push_back(elem);
        }

        return obj;
    }

    static std::unique_ptr<rpc::offboard::ActuatorControl>
    translateToRpcActuatorControl(const mavsdk::Offboard::ActuatorControl& actuator_control)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::ActuatorControl>();

        for (const auto& elem : actuator_control.groups) {
            auto* ptr = rpc_obj->add_groups();
            ptr->CopyFrom(*translateToRpcActuatorControlGroup(elem).release());
        }

        return rpc_obj;
    }

    static mavsdk::Offboard::ActuatorControl
    translateFromRpcActuatorControl(const rpc::offboard::ActuatorControl& actuator_control)
    {
        mavsdk::Offboard::ActuatorControl obj;

        for (const auto& elem : actuator_control.groups()) {
            obj.groups.push_back(translateFromRpcActuatorControlGroup(
                static_cast<mavsdk::rpc::offboard::ActuatorControlGroup>(elem)));
        }

        return obj;
    }

    static std::unique_ptr<rpc::offboard::AttitudeRate>
    translateToRpcAttitudeRate(const mavsdk::Offboard::AttitudeRate& attitude_rate)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::AttitudeRate>();

        rpc_obj->set_roll_deg_s(attitude_rate.roll_deg_s);

        rpc_obj->set_pitch_deg_s(attitude_rate.pitch_deg_s);

        rpc_obj->set_yaw_deg_s(attitude_rate.yaw_deg_s);

        rpc_obj->set_thrust_value(attitude_rate.thrust_value);

        return rpc_obj;
    }

    static mavsdk::Offboard::AttitudeRate
    translateFromRpcAttitudeRate(const rpc::offboard::AttitudeRate& attitude_rate)
    {
        mavsdk::Offboard::AttitudeRate obj;

        obj.roll_deg_s = attitude_rate.roll_deg_s();

        obj.pitch_deg_s = attitude_rate.pitch_deg_s();

        obj.yaw_deg_s = attitude_rate.yaw_deg_s();

        obj.thrust_value = attitude_rate.thrust_value();

        return obj;
    }

    static std::unique_ptr<rpc::offboard::PositionNedYaw>
    translateToRpcPositionNedYaw(const mavsdk::Offboard::PositionNedYaw& position_ned_yaw)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::PositionNedYaw>();

        rpc_obj->set_north_m(position_ned_yaw.north_m);

        rpc_obj->set_east_m(position_ned_yaw.east_m);

        rpc_obj->set_down_m(position_ned_yaw.down_m);

        rpc_obj->set_yaw_deg(position_ned_yaw.yaw_deg);

        return rpc_obj;
    }

    static mavsdk::Offboard::PositionNedYaw
    translateFromRpcPositionNedYaw(const rpc::offboard::PositionNedYaw& position_ned_yaw)
    {
        mavsdk::Offboard::PositionNedYaw obj;

        obj.north_m = position_ned_yaw.north_m();

        obj.east_m = position_ned_yaw.east_m();

        obj.down_m = position_ned_yaw.down_m();

        obj.yaw_deg = position_ned_yaw.yaw_deg();

        return obj;
    }

    static std::unique_ptr<rpc::offboard::VelocityBodyYawspeed> translateToRpcVelocityBodyYawspeed(
        const mavsdk::Offboard::VelocityBodyYawspeed& velocity_body_yawspeed)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::VelocityBodyYawspeed>();

        rpc_obj->set_forward_m_s(velocity_body_yawspeed.forward_m_s);

        rpc_obj->set_right_m_s(velocity_body_yawspeed.right_m_s);

        rpc_obj->set_down_m_s(velocity_body_yawspeed.down_m_s);

        rpc_obj->set_yawspeed_deg_s(velocity_body_yawspeed.yawspeed_deg_s);

        return rpc_obj;
    }

    static mavsdk::Offboard::VelocityBodyYawspeed translateFromRpcVelocityBodyYawspeed(
        const rpc::offboard::VelocityBodyYawspeed& velocity_body_yawspeed)
    {
        mavsdk::Offboard::VelocityBodyYawspeed obj;

        obj.forward_m_s = velocity_body_yawspeed.forward_m_s();

        obj.right_m_s = velocity_body_yawspeed.right_m_s();

        obj.down_m_s = velocity_body_yawspeed.down_m_s();

        obj.yawspeed_deg_s = velocity_body_yawspeed.yawspeed_deg_s();

        return obj;
    }

    static std::unique_ptr<rpc::offboard::VelocityNedYaw>
    translateToRpcVelocityNedYaw(const mavsdk::Offboard::VelocityNedYaw& velocity_ned_yaw)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::VelocityNedYaw>();

        rpc_obj->set_north_m_s(velocity_ned_yaw.north_m_s);

        rpc_obj->set_east_m_s(velocity_ned_yaw.east_m_s);

        rpc_obj->set_down_m_s(velocity_ned_yaw.down_m_s);

        rpc_obj->set_yaw_deg(velocity_ned_yaw.yaw_deg);

        return rpc_obj;
    }

    static mavsdk::Offboard::VelocityNedYaw
    translateFromRpcVelocityNedYaw(const rpc::offboard::VelocityNedYaw& velocity_ned_yaw)
    {
        mavsdk::Offboard::VelocityNedYaw obj;

        obj.north_m_s = velocity_ned_yaw.north_m_s();

        obj.east_m_s = velocity_ned_yaw.east_m_s();

        obj.down_m_s = velocity_ned_yaw.down_m_s();

        obj.yaw_deg = velocity_ned_yaw.yaw_deg();

        return obj;
    }

    static std::unique_ptr<rpc::offboard::AccelerationNed>
    translateToRpcAccelerationNed(const mavsdk::Offboard::AccelerationNed& acceleration_ned)
    {
        auto rpc_obj = std::make_unique<rpc::offboard::AccelerationNed>();

        rpc_obj->set_north_m_s2(acceleration_ned.north_m_s2);

        rpc_obj->set_east_m_s2(acceleration_ned.east_m_s2);

        rpc_obj->set_down_m_s2(acceleration_ned.down_m_s2);

        return rpc_obj;
    }

    static mavsdk::Offboard::AccelerationNed
    translateFromRpcAccelerationNed(const rpc::offboard::AccelerationNed& acceleration_ned)
    {
        mavsdk::Offboard::AccelerationNed obj;

        obj.north_m_s2 = acceleration_ned.north_m_s2();

        obj.east_m_s2 = acceleration_ned.east_m_s2();

        obj.down_m_s2 = acceleration_ned.down_m_s2();

        return obj;
    }

    static rpc::offboard::OffboardResult::Result
    translateToRpcResult(const mavsdk::Offboard::Result& result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case mavsdk::Offboard::Result::Unknown:
                return rpc::offboard::OffboardResult_Result_RESULT_UNKNOWN;
            case mavsdk::Offboard::Result::Success:
                return rpc::offboard::OffboardResult_Result_RESULT_SUCCESS;
            case mavsdk::Offboard::Result::NoSystem:
                return rpc::offboard::OffboardResult_Result_RESULT_NO_SYSTEM;
            case mavsdk::Offboard::Result::ConnectionError:
                return rpc::offboard::OffboardResult_Result_RESULT_CONNECTION_ERROR;
            case mavsdk::Offboard::Result::Busy:
                return rpc::offboard::OffboardResult_Result_RESULT_BUSY;
            case mavsdk::Offboard::Result::CommandDenied:
                return rpc::offboard::OffboardResult_Result_RESULT_COMMAND_DENIED;
            case mavsdk::Offboard::Result::Timeout:
                return rpc::offboard::OffboardResult_Result_RESULT_TIMEOUT;
            case mavsdk::Offboard::Result::NoSetpointSet:
                return rpc::offboard::OffboardResult_Result_RESULT_NO_SETPOINT_SET;
        }
    }

    static mavsdk::Offboard::Result
    translateFromRpcResult(const rpc::offboard::OffboardResult::Result result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case rpc::offboard::OffboardResult_Result_RESULT_UNKNOWN:
                return mavsdk::Offboard::Result::Unknown;
            case rpc::offboard::OffboardResult_Result_RESULT_SUCCESS:
                return mavsdk::Offboard::Result::Success;
            case rpc::offboard::OffboardResult_Result_RESULT_NO_SYSTEM:
                return mavsdk::Offboard::Result::NoSystem;
            case rpc::offboard::OffboardResult_Result_RESULT_CONNECTION_ERROR:
                return mavsdk::Offboard::Result::ConnectionError;
            case rpc::offboard::OffboardResult_Result_RESULT_BUSY:
                return mavsdk::Offboard::Result::Busy;
            case rpc::offboard::OffboardResult_Result_RESULT_COMMAND_DENIED:
                return mavsdk::Offboard::Result::CommandDenied;
            case rpc::offboard::OffboardResult_Result_RESULT_TIMEOUT:
                return mavsdk::Offboard::Result::Timeout;
            case rpc::offboard::OffboardResult_Result_RESULT_NO_SETPOINT_SET:
                return mavsdk::Offboard::Result::NoSetpointSet;
        }
    }

    grpc::Status RequestOffboard(
        grpc::ServerContext* /* context */,
        const rpc::offboard::StartRequest* /* request */,
        rpc::offboard::StartResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->request_offboard();

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status Start(
        grpc::ServerContext* /* context */,
        const rpc::offboard::StartRequest* /* request */,
        rpc::offboard::StartResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->start();

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status Stop(
        grpc::ServerContext* /* context */,
        const rpc::offboard::StopRequest* /* request */,
        rpc::offboard::StopResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->stop();

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status IsActive(
        grpc::ServerContext* /* context */,
        const rpc::offboard::IsActiveRequest* /* request */,
        rpc::offboard::IsActiveResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->is_active();

        if (response != nullptr) {
            response->set_is_active(result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetAttitudeOnce(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetAttitudeRequest* request,
        rpc::offboard::SetAttitudeResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetAttitudeOnce sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_attitude_once(
            translateFromRpcAttitude(request->attitude()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetAttitude(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetAttitudeRequest* request,
        rpc::offboard::SetAttitudeResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetAttitude sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_attitude(
            translateFromRpcAttitude(request->attitude()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetActuatorControlOnce(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetActuatorControlRequest* request,
        rpc::offboard::SetActuatorControlResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetActuatorControlOnce sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_actuator_control_once(
            translateFromRpcActuatorControl(request->actuator_control()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetActuatorControl(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetActuatorControlRequest* request,
        rpc::offboard::SetActuatorControlResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetActuatorControl sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_actuator_control(
            translateFromRpcActuatorControl(request->actuator_control()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetAttitudeRateOnce(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetAttitudeRateRequest* request,
        rpc::offboard::SetAttitudeRateResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetAttitudeRateOnce sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_attitude_rate_once(
            translateFromRpcAttitudeRate(request->attitude_rate()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetAttitudeRate(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetAttitudeRateRequest* request,
        rpc::offboard::SetAttitudeRateResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetAttitudeRate sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_attitude_rate(
            translateFromRpcAttitudeRate(request->attitude_rate()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetPositionNedOnce(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetPositionNedRequest* request,
        rpc::offboard::SetPositionNedResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetPositionNedOnce sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_position_ned_once(
            translateFromRpcPositionNedYaw(request->position_ned_yaw()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetPositionNed(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetPositionNedRequest* request,
        rpc::offboard::SetPositionNedResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetPositionNed sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_position_ned(
            translateFromRpcPositionNedYaw(request->position_ned_yaw()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetVelocityBodyOnce(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetVelocityBodyRequest* request,
        rpc::offboard::SetVelocityBodyResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetVelocityBodyOnce sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_velocity_body_once(
            translateFromRpcVelocityBodyYawspeed(request->velocity_body_yawspeed()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetVelocityBody(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetVelocityBodyRequest* request,
        rpc::offboard::SetVelocityBodyResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetVelocityBody sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_velocity_body(
            translateFromRpcVelocityBodyYawspeed(request->velocity_body_yawspeed()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetVelocityNed(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetVelocityNedRequest* request,
        rpc::offboard::SetVelocityNedResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetVelocityNed sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_velocity_ned(
            translateFromRpcVelocityNedYaw(request->velocity_ned_yaw()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetPositionVelocityNed(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetPositionVelocityNedRequest* request,
        rpc::offboard::SetPositionVelocityNedResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetPositionVelocityNed sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_position_velocity_ned(
            translateFromRpcPositionNedYaw(request->position_ned_yaw()),
            translateFromRpcVelocityNedYaw(request->velocity_ned_yaw()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    grpc::Status SetAccelerationNed(
        grpc::ServerContext* /* context */,
        const rpc::offboard::SetAccelerationNedRequest* request,
        rpc::offboard::SetAccelerationNedResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::Offboard::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        if (request == nullptr) {
            LogWarn() << "SetAccelerationNed sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->set_acceleration_ned(
            translateFromRpcAccelerationNed(request->acceleration_ned()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    void stop()
    {
        _stopped.store(true);
        for (auto& prom : _stream_stop_promises) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        }
    }

private:
    void register_stream_stop_promise(std::weak_ptr<std::promise<void>> prom)
    {
        // If we have already stopped, set promise immediately and don't add it to list.
        if (_stopped.load()) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        } else {
            _stream_stop_promises.push_back(prom);
        }
    }

    void unregister_stream_stop_promise(std::shared_ptr<std::promise<void>> prom)
    {
        for (auto it = _stream_stop_promises.begin(); it != _stream_stop_promises.end();
             /* ++it */) {
            if (it->lock() == prom) {
                it = _stream_stop_promises.erase(it);
            } else {
                ++it;
            }
        }
    }

    LazyPlugin& _lazy_plugin;
    std::atomic<bool> _stopped{false};
    std::vector<std::weak_ptr<std::promise<void>>> _stream_stop_promises{};
};

} // namespace mavsdk_server
} // namespace mavsdk