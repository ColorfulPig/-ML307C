# GNSS/LBS CodeGraph Analysis

## Scope

This note summarizes the current `codegraph` pass for the GNSS/LBS path in `custom_main`, focused on the observed symptom:

- `cm_lbs_location ret=-100`

The goal here is not to fix the issue yet, but to pin down which layer owns the failure and which code paths should be instrumented next.

## Key conclusion

Current evidence points to a failure in the `OneOS LBS` request path, not in GNSS or AGNSS itself.

- `GNSS/AGNSS` code is separate from the `LBS` request path.
- `cm_lbs_location()` is invoked from the custom LBS wrapper after PDP activation.
- `-100` maps to `CM_LBS_UNKNOW_ERR` in the SDK header.

## Important files

- `custom/custom_main/src/custom_main.c`
- `custom/custom_main/src/custom_network.c`
- `custom/custom_main/src/custom_lbs.c`
- `custom/custom_main/src/custom_gnss.c`
- `custom/custom_main/src/custom_profile.c`
- `include/cmiot/cm_lbs.h`
- `examples/oneos_lbs/src/oneos_lbs.c`

## Entry flow

### 1. Main initialization

`codegraph` identifies the project entry as:

- `cm_opencpu_entry` in `custom/custom_main/src/custom_main.c:23`

This entry initializes both network and location-related modules:

- `custom_lbs_init()`
- `custom_network_init()`
- `custom_gnss_init()`

This means GNSS and LBS are initialized as parallel subsystems under the same app startup, but they are not the same execution path.

### 2. Network thread owns the LBS trigger

`codegraph` identifies:

- `custom_network_task` in `custom/custom_main/src/custom_network.c:177`
- `custom_network_event_callback` in `custom/custom_main/src/custom_network.c:14`

`codegraph` directly reports:

- caller of `custom_network_event_callback`: `custom_network_task`

From the source path, the important behavior is:

- the task waits for SIM ready
- waits for network registration
- waits for PDP activation
- after `NETWORK_EVENT_PDP_ACTIVED`, it triggers `custom_lbs_start(CM_LBS_PLAT_ONEOSPOS)`

So the LBS request is coupled to PDP activation timing.

### 3. LBS request path

`codegraph` identifies:

- `custom_lbs_start` in `custom/custom_main/src/custom_lbs.c:79`
- `custom_lbs_cb` in `custom/custom_main/src/custom_lbs.c:11`

Within the `CM_LBS_PLAT_ONEOSPOS` branch, the effective path is:

1. `custom_profile_getString(CONFIG_ITEM_LBS_ONEOS_PID, pid)`
2. `cm_async_dns_set_priority(0)`
3. `cm_lbs_init(CM_LBS_PLAT_ONEOSPOS, &noeospos_cfg)`
4. `cm_lbs_get_attr(...)`
5. `cm_lbs_location(custom_lbs_cb, NULL)`

If `cm_lbs_location()` returns non-zero, the code immediately executes:

- `cm_lbs_deinit()`

If the async callback runs, `custom_lbs_cb()` also does:

- `cm_lbs_deinit()`
- `s_lbs_started = 0`

## GNSS path is separate

`codegraph` also finds:

- `custom_gnss_enable` in `custom/custom_main/src/custom_gnss.c:125`
- `gnss_location_info_t` in `custom/custom_main/inc/custom_gnss.h:19`

The GNSS module manages:

- GNSS open/close
- AGNSS enable/disable
- NMEA configuration
- location parsing from GNSS data

This is structurally separate from `custom_lbs_start()`. Based on code structure alone, GNSS success does not prove LBS success, and LBS failure does not imply GNSS failure.

## Error code ownership

The SDK header `include/cmiot/cm_lbs.h` defines:

- `CM_LBS_UNKNOW_ERR = -100`

That is the strongest current indicator that the error belongs to the LBS layer, not to the higher-level custom wrapper.

## Comparison with official OneOS LBS example

The SDK example in `examples/oneos_lbs/src/oneos_lbs.c` uses this sequence:

1. wait until `cm_modem_get_pdp_state(1) == 1`
2. delay again after PDP is ready
3. set DNS preference to IPv4 with `cm_async_dns_set_priority(0)`
4. call `cm_lbs_init(...)`
5. call `cm_lbs_location(...)`
6. wait for async callback result

This matches the custom project in broad order, but there is one operational difference worth noting:

- the example is built around a dedicated test loop
- the custom project only triggers LBS on the first PDP activation event

That makes the custom flow more sensitive to timing. If the first request is sent too early, there is no built-in retry path in the current logic.

## CodeGraph findings

Direct `codegraph` results used here:

- `custom_lbs_start` is a primary entry point
- `custom_network_event_callback` is a primary entry point
- `custom_network_task` calls `custom_network_event_callback`
- `custom_lbs_start` directly calls `custom_profile_getString`
- `custom_gnss_enable` exists as a separate GNSS control path

## Practical interpretation

At the current stage, the cleanest working conclusion is:

- network registration is not the root issue
- GNSS and AGNSS are not the root issue
- LBS launch timing or LBS-side request readiness is the most likely fault domain
- `cm_lbs_location ret=-100` happens before any successful location callback is delivered

## Recommended next verification

The next debug step should collect evidence at the LBS boundary, not the GNSS boundary:

1. log the exact values before `cm_lbs_location()`
   - PDP state
   - current DNS priority
   - PID string length/content
   - `cm_lbs_init()` return
   - `cm_lbs_get_attr()` return and content
2. retry LBS once after an extra delay if the first request returns `-100`
3. run the SDK `oneos_lbs` example on the same module/SIM/location to separate:
   - project integration issue
   - platform or environment issue

## Working diagnosis

If a second delayed attempt succeeds, the likely root cause is:

- first-shot request timing after PDP activation is too aggressive

If the SDK example also returns `-100`, the likely root cause shifts to:

- OneOSPos platform-side condition
- base-station environment data quality
- lower-level HTTP/DNS or platform request readiness not visible from the wrapper
