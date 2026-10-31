# TLI49012 angle measurement bring-up

Source: `infineon-tlx49012-usermanual-en.pdf`, rev. 1.00, sections
5.1.2 (p.29), 7.1.1 (p.40), 8.1 (p.47), 8.2.11 (p.72).

## Configuration

NUCLEO-H723ZG, Arduino framework. SPI1: SCK PA5, MISO PA6, MOSI PD7.
CS PA4 is software controlled. PC9 SYNC is held LOW; synchronized
registers are not used. Common GND and a 3.3 V sensor supply are expected.

The firmware automatically reads `angle_pred_reg` at address `0x0C` at
100 Hz and logs at 10 Hz. Scheduling is polling based, not a precision timer.
Mode 1, MSB first, nominal 1 MHz. MISO PA6 uses an internal pull-down.
CS stays LOW for all four bytes, transferred in one SPI buffer call.
The read request without clearing status is `18 00 00 E5` (manual p.29).
The displayed angle is `raw * 360 / 65536`, range [0, 360) degrees.
This is the predicted/interpolated angle, not the separate `angle_reg` at 0x0F.

## Run

1. Build and upload using PlatformIO.
2. Open the ST-LINK virtual COM serial monitor at 115200 baud, LF line ending.
3. Observe automatic output: `angle_deg`, `raw`, `status`, `CRC`, status flags,
   and the cumulative `crc_errors` count. No serial commands are implemented.
4. Turn the magnet slowly. Confirm continuous angle changes and wrap near
   360 to 0 degrees. Returning to the same physical position should return
   near the previous angle. This verifies basic function, not absolute accuracy.

Timing constants are `SAMPLE_INTERVAL_MS` and `LOG_INTERVAL_MS` in `src/main.cpp`.
The loop uses `millis()` instead of a blocking delay. Output shows the latest
sample every 100 ms; intermediate samples are not logged. Each sample performs
one frame, with one immediate retry if its CRC fails.

## Interpreting output

`CRC=OK` means the received frame CRC passed; it does not alone establish
measurement health. A failed CRC is retried once. `crc_errors` counts failed
frames, including failed retries. A failed read never supplies a displayed angle.

`angle_deg=INVALID` means CRC failed, LFOK is zero, or DIAG/MAG/SUP/OT is set.
Status bits are not trustworthy when CRC fails. LFOK describes the previous
MOSI frame. OFC records historical communication errors; RST is a reset
indication; neither alone suppresses the displayed angle. CAL indicates
SysCal state, not general sample validity.

Status warnings can latch; this firmware does not clear them automatically.
A MAG warning calls for checking magnet position, strength, and configured
magnetic range. Status flags printed with `CRC=FAIL` are untrusted.

## Power wiring

Verify voltage at the IC, not only the PCB connector: pin 4 (VDD) to pin 2 (GND)
must have the intended 3.3 V supply. Pin 3 is VPROG, not VDD; in normal operation
use the datasheet's GND or unconnected configuration for VPROG. The previous PCB
connected 3.3 V to VPROG while leaving VDD unpowered, which prevented SPI operation.

All-zero/all-one responses, repeated CRC failures, or LFOK remaining zero call
for checking supply, common ground, CS, Mode 1, and wiring. A logic analyzer
should show 32 clocks per CS LOW and request bytes `18 00 00 E5`.

Firmware build success does not verify electrical communication or angle
accuracy. No NVM programming or sensor configuration writes are performed.
