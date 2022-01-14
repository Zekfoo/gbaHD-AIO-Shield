
# gbaHD AIO Shield

![](https://github.com/Zekfoo/gbaHD-AIO-Shield/blob/main/images/front_angle_w_game.jpg)

This is a Spartan Edge Accelerator shield which implements all the hardware connections needed for zwenergy's [gbaHD](https://github.com/zwenergy/gbaHD), no wires required! The PCB incorporates the GBA motherboard, Arduino Nano, and SNES controller port used in a typical gbaHD installation, and interfaces directly with the Spartan Edge Accelerator board via pin header connections. 

![](https://github.com/Zekfoo/gbaHD-AIO-Shield/blob/main/images/back_angle.jpg)

## Disclaimer

Assembly of this PCB requires donor parts from a Game Boy Advance motherboard, and therefore also requires advanced soldering skills. Do not attempt this project unless you are comfortable and confident in your ability to desolder and solder both surface-mount and through-hole components, otherwise you risk irreparable damage to your original console and to this PCB. I claim no responsibility and accept no blame for any failed attempts.

![](https://github.com/Zekfoo/gbaHD-AIO-Shield/blob/main/images/kicad_front.png)

## Ordering the board

**READ THE DISCLAIMER SECTION ABOVE BEFORE CONTINUING**

The PCB can be ordered via the following links:

**[PCBWay](https://www.pcbway.com/project/shareproject/gbaHD_AIO_Shield_8eb5e84a.html)** (Full disclosure - this link gives me a small commission from each order)

**[OSHPark](https://oshpark.com/shared_projects/GWejn5tp)**

Or feel free to order from your preferred PCB fab using the gerbers on this repo.

Board thickness of 1.2mm or thinner is recommended. 1.6mm will work, but soldering of some of the through-hole components may be more difficult.

## Bill of materials

There are a couple *optional* features on this PCB to keep in mind before ordering your parts:

**Choice between an onboard ATmega328P or an external Arduino Nano**

The onboard Arduino offers a cleaner look for the assembled PCB, but since ATmega chips are becoming prohibitively expensive during this  chip shortage, there is also support for plugging in your own Arduino Nano via pin headers. *These options are mutually-exclusive, pick one.*

**Master power switch**

This may be useful for consolizer-style installations that use an enclosure, such that a toggle switch can be mounted to the case to switch power to the entire assembly while leaving the USB-C power supply plugged in. This is completely optional, since the power can be switched off using the Spartan Edge Accelerator's own switch or by unplugging the power supply.

The BOM below is separated between donor parts, required parts, and parts for optional features. Read carefully since the optional features may require components that are also listed in earlier sections.

Please source your own passive components, only values and footprints are listed for these.


|  | Reference(s) | Count | Description | Part Number | Comments | Purchase Link |
|---|---|---|---|---|---|---|
| **From donor GBA** | U1 | 1 | AGB CPU | - | - | - |
|  | U2 | 1 | AGB SRAM | - | - | - |
|  | P1 | 1 | Link port | - | - | - |
|  | P2 | 1 | Cart connector | - | - | - |
|  | EM1, EM2 | 2 | Common-mode choke | ACM4532-102-3P | - | - |
|  |  |  |  |  |  |  |
| **Required** | C1, C5, C6, C7, C8, C9, C10, C11,   C12, C13, C14, C16, C17, C21, C26 | 15 | 0.1uF 0603 | - | - | - |
|  | C2, C3, C4 | 3 | 100pF 0603 | - | - | - |
|  | C15, C22, C27 | 3 | 10uF 0805 | - | - | - |
|  | D1, D4, D6 | 3 | Green LED 0603 | LTST-C191KGKT | Or generic | https://lcsc.com/product-detail/Light-Emitting-Diodes-LED_Lite-On-LTST-C191KGKT_C125098.html |
|  | D2, D3 | 2 | Schottky diode | B5817WS | - | https://lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_Yangzhou-Yangjie-Elec-Tech-B5817WS_C699102.html |
|  | F1, F2, F3 | 3 | 1.25A fuse 0603 | - | - | - |
|  | P3 | 1 | SNES controller port | - | - | https://aliexpress.com/item/32828768824.html |
|  | R1, R6, R8, R12 | 4 | 100kOhm 0603 | - | - | - |
|  | R2, R3, R4, R5 | 4 | 330Ohm 0603 | - | - | - |
|  | R7 | 1 | 270Ohm 0603 | - | - | - |
|  | R9, R10 | 2 | 47Ohm 0603 | - | - | - |
|  | R11 | 1 | 18kOhm 0603 | - | - | - |
|  | R13 | 1 | 15Ohm 0603 | - | - | - |
|  | R14 | 1 | 1kOhm 0603 | - | - | - |
|  | S1 | 1 | Spartan Edge Accelerator | - | - | - |
|  | - | 2 | 10-pin male header |  | Or generic, cut to size | https://lcsc.com/product-detail/Pin-Headers_XKB-Connectivity-X6511WV-10H-C30D60_C706882.html |
|  | - | 2 | 8-pin male header |  | Or generic, cut to size | https://lcsc.com/product-detail/Pin-Headers_XKB-Connectivity-X6511WV-08H-C30D60_C706880.html |
|  | - | 1 | 6-pin male header |  | Or generic, cut to size | https://lcsc.com/product-detail/Pin-Headers_XKB-Connectivity-X6511WV-06H-C30D60_C706878.html |
|  | - | 2 | 10-pin female header |  | Or generic, cut to size | https://lcsc.com/product-detail/Female-Headers_XKB-Connectivity-X6511FV-10-C85D32_C718235.html |
|  | - | 2 | 8-pin female header |  | Or generic, cut to size | https://lcsc.com/product-detail/Female-Headers_XKB-Connectivity-X6511FV-08-C85D32_C718237.html |
|  | - | 1 | 6-pin female header |  | Or generic, cut to size | https://lcsc.com/product-detail/Female-Headers_XKB-Connectivity-X6511FV-06-C85D32_C718239.html |
|  |  |  |  |  |  |  |
| **Onboard Arduino Only** | C18, C19 | 2 | 1uF 0603 | - | - | - |
|  | C20 | 1 | 10uF 0805 | - | Total 4 pieces including "Required" count | - |
|  | C23, C30, C31, C32 | 4 | 0.1uF 0603 | - | Total 19 pieces including "Required" count | - |
|  | C24, C25 | 2 | 18pF 0603 | - | - | - |
|  | D5 | 1 | Red LED 0603 | LTST-C191KRKT | Or generic | https://lcsc.com/product-detail/Light-Emitting-Diodes-LED_Lite-On-LTST-C191KRKT_C125099.html |
|  | P4 | 1 | USB-C Port | U262-161N-4BVC11 | - | https://lcsc.com/product-detail/USB-Connectors_XKB-Connectivity-U262-161N-4BVC11_C319148.html |
|  | P5 | 1 | 2x3-pin male header | X6521WV-2x03H-C30D60 | Or generic, cut to size | https://lcsc.com/product-detail/Pin-Headers_XKB-Connectivity-X6521WV-2x03H-C30D60_C706893.html |
|  | R15, R16, R17 | 3 | 2.2kOhm 0603 | - | - | - |
|  | R19 | 1 | 100kOhm 0603 | - | Total 5 pieces including "Required" count | - |
|  | R22, R24 | 2 | 5.1kOhm 0603 | - | - | - |
|  | R23, R25 | 2 | 1kOhm 0603 | - | Total 3 pieces including "Required" count | - |
|  | SW1 | 1 | Pushbutton | TS-1101-C-W | - | https://lcsc.com/product-detail/Tactile-Switches_XKB-Connectivity-TS-1101-C-W_C318938.html |
|  | U3 | 1 | 2.5V linear regulator | TLV71325PDBV | - | https://lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_Texas-Instruments-TLV71325PDBVR_C90841.html |
|  | U4 | 1 | Microcontroller | ATmega328P-AU | - | https://lcsc.com/product-detail/ATMEL-AVR_Microchip-Tech-ATMEGA328P-AU_C14877.html |
|  | U5 | 1 | USB-to-serial IC | CH340C | - | https://lcsc.com/product-detail/USB-ICs_WCH-Jiangsu-Qin-Heng-CH340C_C84681.html |
|  | Y1 | 1 | 16MHz crystal | Q24FA20H0023200 | - | https://lcsc.com/product-detail/Crystals_Seiko-Epson-Q24FA20H0023200_C255948.html |
|  |  |  |  |  |  |  |
| **External Arduino Only** | A1 | 1 | Arduino Nano | - | - | - |
|  | - | 2 | 15-pin female header | B-2200S15P-A120 | Or generic, cut to size | https://lcsc.com/product-detail/Female-Headers_Ckmtw-Shenzhen-Cankemeng-B-2200S15P-A120_C124408.html |
|  |  |  |  |  |  |  |
| **Master Power Switch Only** | C28, C29 | 2 | 1uF 0603 | - | Total 4 pieces including "Onboard Arduino" count | - |
|  | Q1 | 1 | P-Channel MOSFET SOT23 | PJM3415PSA | - | https://lcsc.com/product-detail/MOSFETs_PJSEMI-PJM3415PSA_C411717.html |
|  | R20 | 1 | 100kOhm 0603 | - | Total 6 pieces including "Required" count | - |
|  | R21 | 1 | 2.2kOhm 0603 | - | Total 4 pieces including "Onboard Arduino" count | - |
|  | SW2 | 1 | Rocker Switch | RS601HL-1020011BB | Or use any switch of your choice | https://lcsc.com/product-detail/Rocker-Switches_HCTL-RS601HL-1020011BB_C2885783.html |

## Programming the Spartan Edge Accelerator
TO-DO. For now, refer to the documentation on zwenergy's repo.

## Programming the Arduino bootloader (onboard Arduino only)

If you assemble this board with an onboard Arduino, the ATmega328p will not come with the Arduino bootloader installed, so it will not be programmable from the Arduino IDE right out of the gate. Follow the [official documentation](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP) to program the bootloader via the ICSP header on the board (a second Arduino is required).

## Programming the Arduino

The Arduino is used to interface between the SNES controller port and the GBA for button inputs and the reset function. It also interfaces with the Spartan Edge Accelerator for on-screen display controls. Upload this [sketch](https://github.com/Zekfoo/gbaHD-AIO-Shield/blob/main/controller/controller.ino).

By default, the button combos are L+R+X+Y to toggle the OSD, and L+R+START+SELECT to reset the GBA. This can be configured by modifying the `` `define`` macros in the Arduino sketch. 

## Acknowledgements
As always, thanks to the [Gameboy Discord](https://discord.gg/gameboy) community for their continued support.
Special thanks to [zwenergy](https://github.com/zwenergy) for his hard work on the gbaHD
