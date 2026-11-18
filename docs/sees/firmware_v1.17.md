# MAVSDK v4.2.1 for Firmware v1.17

| | |
|---|---|
| **Status** | Branches created on 2026-11-18, local only, not yet pushed. No Sees changes ported yet. Work starts after the Firmware `SEES_GPS_RAW` changes are merged. |
| **Plan** | [`docs/sees/sees_gps_raw.md`](https://github.com/SEESAI/Firmware/blob/vilas/v1.17.0/mavlink/docs/sees/sees_gps_raw.md) in SEESAI/Firmware, section *Rollout plan across repos*, Step 3. That doc is the source of truth; this note only records what is happening in this repo. |
| **Paired branches** | Firmware `v1.17.0-dev`, SeesInterface2 `vilas/feat/firmware_v1.17` |

## What and why

SeesInterface2 currently uses this fork's `v1.4.11_dev` branch: upstream v1.4.11 plus 20 Sees commits. Firmware v1.17 stops sending the patched `GPS_INPUT` and sends the new `SEES_GPS_RAW` message instead. Receiving it needs the `MavlinkDirect` plugin, which only exists in MAVSDK v4.

So this fork moves to upstream **v4.2.1**, the latest release (2026-11-18). v4.2.1 is v4.2.0 plus two bug fixes:

- `action`: don't arm after a failed switch to Hold (#3221)
- `info`: fix binary-to-hex string truncation and trailing null (#3220)

MAVSDK changed its API in v2, v3 and v4, so the Sees commits are re-applied on top of v4.2.1 rather than merged.

The MAVSDK and SeesInterface2 changes are done together, because SeesInterface2 can't build against v4 without its own API migration, and the MAVSDK audit decides which Sees features move into SeesInterface2.

## Branches

| Branch | Based on | Purpose |
|---|---|---|
| `v4.2.1_dev` | upstream tag `v4.2.1` (`417d1ccd3`), unchanged | Long-lived base, the successor of `v1.4.11_dev`. Receives the feature branch once v1.17 is released. |
| `vilas/feat/firmware_v1.17` | `v4.2.1_dev` | Work branch. Sees commits are ported here. Same name as the SeesInterface2 branch, so the pair is easy to find. |
| `v1.4.11_dev` | upstream v1.4.11 | Unchanged. Stays in use by SeesInterface2 `master` for the v1.13 fleet. |

While the work is in progress, SeesInterface2's vcpkg port (`REF` in `BuildTools/vcpkg/ports/mavsdk/portfile.cmake`) points at commits on `vilas/feat/firmware_v1.17`. At release, the feature branch is merged into `v4.2.1_dev`, tagged (e.g. `v4.2.1-sees.1`), and SeesInterface2 re-pins to the tag.

Don't force-push `vilas/feat/firmware_v1.17` once SeesInterface2 pins one of its commits, or fresh clones of SeesInterface2 won't be able to fetch it.

## To do

1. Push both branches to SEESAI/MAVSDK.
2. Audit the 20 Sees commits (`git log --no-merges v1.4.11..origin/v1.4.11_dev`). Decide **drop**, **SI2** (reimplement in SeesInterface2 with `MavlinkDirect`) or **port** for each. The Firmware doc has the table with the expected decision for each commit. `michael/v3.2.0-dev` shows how 15 of them were resolved against v3.2.0.
3. Port the commits marked "port" onto `vilas/feat/firmware_v1.17`, one commit each.
4. Choose how `SEES_GPS_RAW` reaches SeesInterface2 (Firmware doc, Step 3c). With the recommended Option A, SeesInterface2 receives it through `MavlinkDirect`, this repo needs no GPS change, and the old `GPS_INPUT` additions (#15, #20) are dropped. Option B ports those additions and decodes `SEES_GPS_RAW` here instead.
5. Update the status row above as work progresses.

## Notes

- `docs/sees/mavsdk-v4-migration-plan.md` is an earlier draft (2026-09-29) that targeted v4.0.1 and kept the patched `GPS_INPUT`. This note and the Firmware doc replace it.
- Switching from `v1.4.11_dev` leaves the old `src/third_party/gtest` submodule checkout behind as an untracked `src/` directory. v4 keeps its sources under `cpp/`, so it's safe to delete.
