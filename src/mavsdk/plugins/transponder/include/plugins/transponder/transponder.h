// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/main/protos/transponder/transponder.proto)

#pragma once

#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "plugin_base.h"

#include "handle.h"

namespace mavsdk {

class System;
class TransponderImpl;

/**
 * @brief Allow users to get ADS-B information
 * and set ADS-B update rates.
 */
class Transponder : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto transponder = Transponder(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Transponder(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto transponder = Transponder(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit Transponder(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~Transponder() override;

    /**
     * @brief ADSB classification for the type of vehicle emitting the transponder signal.
     */
    enum class AdsbEmitterType {
        NoInfo, /**< @brief No emitter info.. */
        Light, /**< @brief Light emitter.. */
        Small, /**< @brief Small emitter.. */
        Large, /**< @brief Large emitter.. */
        HighVortexLarge, /**< @brief High vortex emitter.. */
        Heavy, /**< @brief Heavy emitter.. */
        HighlyManuv, /**< @brief Highly maneuverable emitter.. */
        Rotocraft, /**< @brief Rotorcraft emitter.. */
        Unassigned, /**< @brief Unassigned emitter.. */
        Glider, /**< @brief Glider emitter.. */
        LighterAir, /**< @brief Lighter air emitter.. */
        Parachute, /**< @brief Parachute emitter.. */
        UltraLight, /**< @brief Ultra light emitter.. */
        Unassigned2, /**< @brief Unassigned2 emitter.. */
        Uav, /**< @brief UAV emitter.. */
        Space, /**< @brief Space emitter.. */
        Unassgined3, /**< @brief Unassigned3 emitter.. */
        EmergencySurface, /**< @brief Emergency emitter.. */
        ServiceSurface, /**< @brief Service surface emitter.. */
        PointObstacle, /**< @brief Point obstacle emitter.. */
    };

    /**
     * @brief Stream operator to print information about a `Transponder::AdsbEmitterType`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, Transponder::AdsbEmitterType const& adsb_emitter_type);

    /**
     * @brief Altitude type used in AdsbVehicle message
     */
    enum class AdsbAltitudeType {
        PressureQnh, /**< @brief Altitude reported from a Baro source using QNH reference. */
        Geometric, /**< @brief Altitude reported from a GNSS source. */
    };

    /**
     * @brief Stream operator to print information about a `Transponder::AdsbAltitudeType`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, Transponder::AdsbAltitudeType const& adsb_altitude_type);

    /**
     * @brief ADSB Vehicle type.
     */
    struct AdsbVehicle {
        uint32_t icao_address{}; /**< @brief ICAO (International Civil Aviation Organization) unique
                                    worldwide identifier */
        double latitude_deg{}; /**< @brief Latitude in degrees (range: -90 to +90) */
        double longitude_deg{}; /**< @brief Longitude in degrees (range: -180 to +180). */
        AdsbAltitudeType altitude_type{}; /**< @brief ADSB altitude type. */
        float absolute_altitude_m{}; /**< @brief Altitude in metres according to altitude_type */
        float heading_deg{}; /**< @brief Course over ground, in degrees */
        float horizontal_velocity_m_s{}; /**< @brief The horizontal velocity in metres/second */
        float vertical_velocity_m_s{}; /**< @brief The vertical velocity in metres/second. Positive
                                          is up. */
        std::string callsign{}; /**< @brief The callsign */
        AdsbEmitterType emitter_type{}; /**< @brief ADSB emitter type. */
        uint32_t squawk{}; /**< @brief Squawk code. */
        uint32_t tslc_s{}; /**< @brief Time Since Last Communication in seconds. */
        uint16_t flags{};  /**< @brief ADSB flags validity */
    };

    /**
     * @brief Equal operator to compare two `Transponder::AdsbVehicle` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const Transponder::AdsbVehicle& lhs, const Transponder::AdsbVehicle& rhs);

    /**
     * @brief Stream operator to print information about a `Transponder::AdsbVehicle`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, Transponder::AdsbVehicle const& adsb_vehicle);

    /**
     * @brief Possible results returned for transponder requests.
     */
    enum class Result {
        Unknown, /**< @brief Unknown result. */
        Success, /**< @brief Success: the transponder command was accepted by the vehicle. */
        NoSystem, /**< @brief No system connected. */
        ConnectionError, /**< @brief Connection error. */
        Busy, /**< @brief Vehicle is busy. */
        CommandDenied, /**< @brief Command refused by vehicle. */
        Timeout, /**< @brief Request timed out. */
    };

    /**
     * @brief Stream operator to print information about a `Transponder::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, Transponder::Result const& result);

    /**
     * @brief Callback type for asynchronous Transponder calls.
     */
    using ResultCallback = std::function<void(Result)>;

    /**
     * @brief Callback type for subscribe_transponder.
     */
    using TransponderCallback = std::function<void(AdsbVehicle)>;

    /**
     * @brief Handle type for subscribe_transponder.
     */
    using TransponderHandle = Handle<AdsbVehicle>;

    /**
     * @brief Subscribe to 'transponder' updates.
     */
    TransponderHandle subscribe_transponder(const TransponderCallback& callback);

    /**
     * @brief Unsubscribe from subscribe_transponder
     */
    void unsubscribe_transponder(TransponderHandle handle);

    /**
     * @brief Poll for 'AdsbVehicle' (blocking).
     *
     * @return One AdsbVehicle update.
     */
    AdsbVehicle transponder() const;

    /**
     * @brief Set rate to 'transponder' updates.
     *
     * This function is non-blocking. See 'set_rate_transponder' for the blocking counterpart.
     */
    void set_rate_transponder_async(double rate_hz, const ResultCallback callback);

    /**
     * @brief Set rate to 'transponder' updates.
     *
     * This function is blocking. See 'set_rate_transponder_async' for the non-blocking counterpart.
     *
     * @return Result of request.
     */
    Result set_rate_transponder(double rate_hz) const;

    /**
     * @brief Copy constructor.
     */
    Transponder(const Transponder& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Transponder& operator=(const Transponder&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<TransponderImpl> _impl;
};

} // namespace mavsdk