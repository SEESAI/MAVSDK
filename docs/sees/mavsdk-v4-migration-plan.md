# MAVSDK v4.0.1 migration, synchronised with PX4 v1.17 and MAVLink

Status: draft, 2026-09-29

The next internal release moves three repositories together:

| Repo | Fielded today | Target for next internal release |
|---|---|---|
| MAVSDK (`SEESAI/MAVSDK`) | `v1.4.11_dev` = v1.4.11 + 20 SEES commits | New `v4.0.1_dev` from upstream **v4.0.1** |
| PX4 (`SEESAI/Firmware`) | `v1.13.1_dev` = v1.13.1 + 41 SEES commits | `v1.17.0-dev` (migration in progress; see [SEES_V1.17_MIGRATION.md](https://github.com/SEESAI/Firmware/blob/vilas/fix/v1.17/docs/sees/SEES_V1.17_MIGRATION.md) and `SEES_V1.17_TEST_PLAN.md` in the same folder) |
| MAVLink (`SEESAI/mavlink`) | `460529e8` (upstream `3b52eac0` from 2022-04 + 4 SEES commits), pinned by both MAVSDK and PX4 v1.13 | SEES fork rebased onto a current upstream commit, pinned by both MAVSDK v4 and PX4 v1.17 |

The PX4 v1.17 work branch currently pins upstream MAVLink (`33af200d`) only because that
migration is not finished. It has to move back onto the SEES fork before release.

MAVLink is the contract between the other two repos, so this plan treats it as the first thing
to settle.

## 1. MAVLink: the shared contract

### What the SEES fork carries

All SEES MAVLink changes are extension fields on standard messages in `common.xml`:

| Change | Branch | In `460529e8`? | Used by |
|---|---|---|---|
| `GPS_INPUT.course_accuracy`, `GPS_INPUT.heading_accuracy` (inserted **before** the standard `yaw` extension) | `master`-line / `3b52eac_dev` | Yes | PX4 outbound `GPS_INPUT` stream (#52, #81) → MAVSDK `Telemetry::GpsInput` |
| `GPS_INPUT.time_utc_usec` (after `yaw`) | same | Yes | same |
| `GPS_INPUT.noise_per_ms` | `ws_gps_noise_per_ms_logging` (2026-02) | No | PX4 v1.17 already carries `noise_per_ms` another way (PX4 #99, NO_PORT). Probably drop. |
| `PARAM_VALUE.param_component_id` | `ws_parameter_value_comp_id` (2025-04) | No | Param tooling for UAVCAN components (PX4 #94/#96, DEFER_UNTIL_TEST); relates to MAVSDK param-by-component |

Upstream pins to align with:

- MAVSDK v4.0.1 pins upstream `d6a7eeaf` (2025-11-13).
- PX4 v1.17 work branch pins upstream `33af200d` (2025-09-25), an ancestor of `d6a7eeaf`,
  13 commits older on the same line.

### Decision: keep the current `GPS_INPUT` field order for this release

Because `course_accuracy` and `heading_accuracy` sit before `yaw`, our `GPS_INPUT` extension
layout differs from upstream MAVLink. Both options:

- **Keep the current order (recommended for this release).** A rebased fork then stays
  wire-compatible with fielded PX4 v1.13 and MAVSDK 1.4.11, so MAVSDK v4 can be tested against
  v1.13 hardware before PX4 v1.17 is ready (see §4, stage A). Known limitation, unchanged from
  today: a third-party device sending standard `GPS_INPUT` to our PX4 would have its `yaw`
  misread.
- **Move the SEES fields after `yaw`, or into a SEES dialect message.** Cleaner and
  upstream-compatible, but it breaks wire compatibility with everything fielded. That would make
  all three repos a hard lock-step cutover with no mixed-version testing. Do this in a later
  release, ideally by moving SEES fields into a `sees.xml` dialect (both MAVSDK and PX4 support
  selecting a dialect) or by upstreaming them.

### MAVLink tasks

1. Create `sees/v4` (name TBD) in `SEESAI/mavlink` from upstream `d6a7eeaf`. Re-apply the
   `GPS_INPUT` fields in the **same order** as `460529e8`.
2. Decide on `noise_per_ms` (likely drop) and on `PARAM_VALUE.param_component_id` (keep only if
   PX4 #94/#96 is ported and MAVSDK needs it).
3. Tag it. MAVSDK v4 and PX4 v1.17 both pin that tag.
4. Add a CI check that generates C headers and compares the `GPS_INPUT` / `PARAM_VALUE` CRC
   extras and field offsets against `460529e8`. This catches accidental layout changes.

## 2. What MAVSDK changes today

Our 20 commits add +1,865 / −110 lines across 33 files. None of the SEES plugin additions are in
`proto/`, so they are C++-only; `mavsdk_server`/gRPC users are affected only by the core
changes.

**Core**

- Heartbeats start when the first system is discovered and stop only when *all* systems have
  timed out.
- Default configuration is `CompanionComputer` instead of `GroundStation`.
- `System::auto_pilot_time()`: autopilot clock via timesync.
- PX4 `AUTO_PRECLAND` sub-mode and `FlightMode::PrecisionLand`.
- Raw heartbeat `base_mode` / `custom_mode` stored (feeds `Telemetry::ModeInfo`).
- `get_param_int/float` accept an optional component ID.
- Param `receive_timeout` clears `_all_params_callback` so it cannot fire twice.

**mavsdk_server:** `SubscribeConnectionState` reports only `systems[0]` (the flight controller).

**Plugins**

- **Action:** `precision_land()` / `precision_land_async()`.
- **Offboard:** `AccelerationBodyYawspeed` setpoint (`SET_POSITION_TARGET_LOCAL_NED` in
  `MAV_FRAME_BODY_NED`); `request_offboard()`; `*_once()` single-shot variants of the setpoint
  setters.
- **Telemetry:** GPS2 (`gps_2_info`, `raw_gps_2`), `GpsInput`, `GpsRtcmData` (received),
  `RadioStatus`, `ServoOutputRaw`, `LandingTargetPosition`, `ModeInfo`, `VehicleStatus` (reuses
  `SYS_STATUS.errors_count1..4`), battery `current_a` / `mah_consumed`, `Imu` `abs_pressure` /
  `pressure_alt` / `fields_updated`.
- **Transponder:** ADSB `flags`.
- **LogFiles:** `get_entries()` completes once all entries arrive, retries up to 5 times, always
  calls back once.

**CI:** `.github/workflows/sees.yml` builds a static `mavsdk_server` on Ubuntu 22.04 and drafts a
release on `v*` tags.

**Known bugs in our patches (fix, don't port):**

- `*_once()` doesn't reset `_mode`, so a later `set_position_ned()` never restarts streaming.
- `ServoOutputRaw` truncates `time_usec` to 32 bits and leaves fields uninitialised.
- `LandingTargetPosition::MavFrame` mislabels frame 0 (GLOBAL) as "Undef".

**Unmerged MAVSDK branches to decide on:** `ws_rtcm_length_update` (RTK send rework, Ubuntu
24.04 build fixes), `RP_nix`, `ws_sls_mavsdk_server_fix`, `ws_precision_land_yaw_update`.
`michael/v3.2.0-dev` and `michael/rebase` are stale 2024 ports, useful only as conflict-resolution
references.

## 3. Cross-repo dependencies

Each MAVSDK feature below depends on PX4 sending something. The PX4 column is the verdict from
the PX4 v1.17 migration doc; the last column is what that means for MAVSDK.

| MAVSDK feature | PX4 side (v1.17 status) | Consequence for MAVSDK |
|---|---|---|
| `Telemetry::GpsInput` (incl. `course_accuracy`, `heading_accuracy`, `time_utc_usec`) | Outbound `GPS_INPUT` stream #52: PORT. Accuracy fields #81: DEFER_UNTIL_TEST; the PX4 test plan asks whether MAVSDK depends on them. | **Answer to PX4: yes, MAVSDK decodes all of them.** #81 should flip to "port with adaptation", and #52 must build against the SEES MAVLink fork, not upstream. Confirm with SI2 which fields it actually uses. |
| `Telemetry::VehicleStatus` (`SYS_STATUS.errors_count1..4`: control-signal loss, Mav joystick count, RC count, desired source) | Control-source selector #38/#58/#62/#68/#72/#97: PORT, H/H/H, full reimplementation; PX4's highest-priority feature | MAVSDK must match whatever PX4 reimplements. Agree the field mapping in the PX4 requirements review **before** PX4 implements it. Consider a proper SEES message instead of reusing `errors_count` (next release, with the dialect). |
| `Telemetry::RadioStatus` | `RADIO_STATUS` stream #29: PORT (test TLM-03) | Keep; verify field scaling against the recreated stream. |
| `Telemetry::GpsRtcmData` (received) | `GPS_RTCM_DATA` onboard stream #37: PORTED (#104) | Keep. |
| `raw_gps_2` / `gps_2_info` (`GPS2_RAW`) | Dual CAN GPS instance ordering #31/#50/#69: DEFER/PORT | Which receiver is "GPS2" depends on PX4's instance ordering. Re-verify after PX4 lands that group. |
| Offboard `AccelerationBodyYawspeed` (`MAV_FRAME_BODY_NED`) | Yaw-only rotation for accel setpoints #84 → #102: PORTED, NOT MERGED (test NAV-06). Keep offboard-accel alive on position loss #88: DEFER_UNTIL_TEST | Keep sending `MAV_FRAME_BODY_NED`; the semantics are "yaw-only, level frame", same as velocity. Only valid once #102 is merged. Add a MAVSDK-side SITL test mirroring NAV-06. |
| `Action::precision_land()`, `Telemetry::LandingTargetPosition` | Precision-land #40, #47, #86: NO_PORT, feature unused | **Drop from MAVSDK** once SI2 confirms it doesn't call them. |
| Param by component ID | `UAVCAN_COMPID_1/2` #94/#96: DEFER_UNTIL_TEST; MAVLink `PARAM_VALUE.param_component_id` branch | Use v4 `Param::select_component()`. Only port MAVLink `param_component_id` if the PX4 feature is ported. |
| `Telemetry::ModeInfo` (raw `custom_mode`) | PX4 v1.17 adds new modes | Clients decoding `custom_mode` must be checked against v1.17's main/sub-mode values. |
| STATUSTEXT reception without a GCS connected | #66/#67: PORTED (#109; test SYS-07) | No MAVSDK change; include in joint tests. |
| MAVSDK identity (`CompanionComputer`, onboard link) | 4 MAVLink instances #63: PORTED (#108); onboard-mode instance | Configure MAVSDK / `mavsdk_server` sysid/compid explicitly so it matches PX4's onboard instance config. |

## 4. Plan

The release is built in three stages so each repo can be validated before everything changes at
once.

### Stage A: MAVSDK v4 against fielded PX4 v1.13

Goal: validate the MAVSDK migration on its own, against firmware we already trust.

1. **Scope.** List every consumer of the fork (SI2 and any other apps; C++ vs `mavsdk_server`),
   and get answers on the §3 open items: `GpsInput` field usage, precision land, `VehicleStatus`.
   Decide on the unmerged MAVSDK branches.
2. **Behaviour tests on `v1.4.11_dev`.** Add SITL tests for each SEES feature: every added
   telemetry stream, accel-body offboard, the `*_once` behaviour, connection state with multiple
   systems, heartbeat start/stop, param by component, log listing. They must run unchanged on the
   new stack.
3. **MAVLink fork rebase** (§1, tasks 1–4), preserving the current `GPS_INPUT` layout.
4. **New branch `v4.0.1_dev` from `v4.0.1`.** Point `MAVLINK_URL` / `MAVLINK_HASH` at the rebased
   fork. Check that `cpp/third_party/mavlink/mavlink.patch` still applies. Keep the `common` fields
   working under v4's default `ardupilotmega` dialect (it includes `common`).
5. **Minimal fork patches.** Port only what has no upstream route: the FC-only connection state
   and `auto_pilot_time()`. Drop what v4 already covers:

   | Our change | v4.0.1 replacement |
   |---|---|
   | Heartbeat start/stop fix | Built in (sends while any system is connected; `always_send_heartbeats`, watchdog) |
   | Param by component ID | `Param::select_component()` |
   | Param double-callback guard, LogFiles timeout | Rewritten upstream (confirm with step 2 tests) |
   | Battery current / consumed | `current_battery_a`, `capacity_consumed_ah` (**Ah, not mAh**) |
   | `CompanionComputer` default | Explicit `Configuration`; `mavsdk_server --sysid/--compid` |

6. **SEES extension library** in the app repo, built on `MavlinkPassthrough` (compiled dialect,
   low overhead, preferred for high-rate messages) or `MavlinkDirect` (JSON, also over gRPC).
   It provides the telemetry streams, accel-body and single-shot offboard setpoints, and
   `request_offboard` (`MAV_CMD_DO_SET_MODE`). Drop precision land if §3 confirms it is unused.
   Fix the known bugs from §2 here.
7. **Port consumer apps.** Expect: `.h` → `.hpp` headers; `Mavsdk` requires a `Configuration`;
   handle-based subscriptions with `unsubscribe_*`; `first_autopilot()` for discovery;
   `Param::select_component()`; battery Ah units.
8. **CI.** Rewrite `sees.yml` for v4: sources under `cpp/`, CMake ≥ 3.22.1, Ubuntu 24.04 runners,
   static `mavsdk_server` artifact and draft release on tag.
9. **Validate against PX4 v1.13** in SITL and on the bench, using the step 2 tests. This works
   only because §1 keeps the wire layout. Stage A exits when MAVSDK v4 matches v1.4.11 behaviour
   against v1.13.

### Stage B: PX4 v1.17 onto the SEES MAVLink fork

Owned by the PX4 migration; listed here because MAVSDK depends on it.

10. Re-point the PX4 v1.17 `src/modules/mavlink/mavlink` submodule from upstream `33af200d` to the
    same fork tag as MAVSDK.
11. Port the PX4 items MAVSDK depends on, in the order the PX4 plan already sets:
    - control-source selector and its `SYS_STATUS` counters, with the `VehicleStatus` mapping
      agreed with MAVSDK;
    - `GPS_INPUT` stream (#52) plus accuracy fields (#81);
    - `RADIO_STATUS` (#29);
    - merge the offboard accel frame fix (#102).
12. Run the MAVSDK stage-A test suite against PX4 v1.17 SITL alongside the PX4 test plan
    (TLM-03, NAV-06, SYS-06, SYS-07, MC-04).

### Stage C: synchronised release

13. **Release candidate.** MAVSDK `v4.0.1_dev` tag + PX4 `v1.17.0-dev` tag + MAVLink fork tag,
    recorded together in one release manifest. Tag names TBD.
14. **Joint validation.** SITL → bench HIL → flight tests, covering:
    - the control-source handover (PX4 MC-01…MC-11) observed through MAVSDK `VehicleStatus`;
    - offboard acceleration (NAV-06);
    - GPS_INPUT and dual GPS;
    - RTK/RTCM;
    - log download.
15. **Rollout.** Ship all three together. Mixed versions stay wire-compatible thanks to §1, but
    only this exact combination is supported. Keep `v1.4.11_dev`, `v1.13.1_dev`, `460529e8` and
    their binaries as the rollback set until the new stack has flight hours.

## 5. Risks

- **Control-source selector (`VehicleStatus`).** PX4 rates the reimplementation H/H/H and it has a
  field incident history. MAVSDK's `VehicleStatus` is meaningless until PX4 lands it with an agreed
  mapping. This is the critical path for the release.
- **`GPS_INPUT` layout.** An accidental reorder during the MAVLink rebase silently corrupts `yaw`
  and the accuracy fields across the whole fleet. §1 task 4 guards against this.
- **PX4 #81 currently deferred.** If PX4 drops the accuracy fields, `Telemetry::GpsInput` loses
  data that SI2 may rely on.
- **API churn in consumer apps.** Three MAVSDK major versions of changes; likely the largest
  MAVSDK-side effort.
- **v4.0 is new** (v4.0.0 released 2026-09-22, v4.0.1 on 2026-09-29). Pin the exact tag, check for
  v4.0.x patches before stage C, and keep v3.17.4 as a fallback.
- **High-rate streams over `MavlinkDirect`** are delivered as JSON; use `MavlinkPassthrough` for
  messages like `HIGHRES_IMU` in C++ consumers.
