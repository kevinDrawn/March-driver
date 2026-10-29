# TLI49012 angle measurement bring-up

Source: `infineon-tlx49012-usermanual-en.pdf`, rev. 1.00, sections
5.1.2 (p.29), 7.1.1 (p.40), 8.1 (p.47), 8.2.11 (p.72).

## Configuration

NUCLEO-H723ZG, Arduino framework. SPI1: SCK PA5, MISO PA6, MOSI PD7.
CS PA4 is software controlled. PC9 SYNC is held LOW; synchronized
registers are not used. Common GND and a 3.3 V sensor supply are expected.

The firmware automatically reads `angle_pred_reg` at address `0x0C` at
100 Hz and logs at 10 Hz. Scheduling is polling based, not a precision timer.
Mode 1, MSB first, nominal 1 MHz. CS stays LOW for all four bytes.
The read request without clearing status is `18 00 00 E5` (manual p.29).
The displayed angle is `raw * 360 / 65536`, range [0, 360) degrees.
This is the predicted/interpolated angle, not the separate `angle_reg` at 0x0F.

## Run

1. Build and upload using PlatformIO.
2. Open the ST-LINK virtual COM serial monitor at 115200 baud, LF line ending.
3. Observe automatic output. Send `h` followed by Enter for command help.
4. Turn the magnet slowly. Confirm continuous angle changes and wrap near
   360 to 0 degrees. Returning to the same physical position should return
   near the previous angle. This verifies basic function, not absolute accuracy.

Commands (each followed by Enter):

| Command | Action |
| --- | --- |
| `a` | Resume automatic angle polling at address 0x0C |
| `s` | Stop polling |
| `r HEX` | Poll an in-frame register as raw data; address is hexadecimal |
| `d` | Read diagnostic registers 0x00/0x01/0x03/0x05 and field magnitude 0x15 |
| `c` | Explicitly clear latched OFC/DIAG/MAG/SUP/RST/OT status |
| `h` | Print help |

## Interpreting output

`crc=OK` means the received frame CRC passed; it does not alone establish
measurement health. A failed CRC is retried once. `crc_errors` counts failed
frames, `recovered` counts successful retries, and `failed` counts reads for
which both attempts failed. A failed read never supplies a displayed angle.

`angle_deg=INVALID` means CRC failed, LFOK is zero, or DIAG/MAG/SUP/OT is set.
Status bits are not trustworthy when CRC fails. LFOK describes the previous
MOSI frame. OFC records historical communication errors; RST is a reset
indication; neither alone suppresses the displayed angle. CAL indicates
SysCal state, not general sample validity.

Status warnings can latch. Record `d` output before using `c`; the subsequent
read, not the clear-response frame, shows the resulting status. A persistent
fault can reassert. A MAG warning calls for checking magnet position, strength,
and configured magnetic range. Register 0x15 field magnitude converts as
`raw * 0.0064` mT when its CRC passes.

All-zero/all-one responses, repeated CRC failures, or LFOK remaining zero call
for checking supply, common ground, CS, Mode 1, and wiring. A logic analyzer
should show 32 clocks per CS LOW and request bytes `18 00 00 E5`.

Firmware build success does not verify electrical communication or angle
accuracy. No NVM programming or sensor configuration writes are performed.
