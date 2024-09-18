# Open Smart Energy Converter ‘SEC-B80-8A' - Firmware #
SEC-B80-8A is a smart DC/DC converter designed for Maximum Power Point Tracking and battery charge applications in electric vehicles with integrated solar modules. The hardware is based on Gallium-Nitride (GaN) switches which make the product highly efficient and lightweight; only 0.5% of energy is lost due to heat, and the weight is reduced by 50% compared to existing solutions.
Open-sourcing this project's hardware and software serves TPEE’s aim to increase collaboration in the high-tech sector and make high-end components more accessible to smaller organizations. With smart power conversion, we hope to get one step closer to an energy-efficient future!

# About TPEE #
This project is fully developed by TPEE, an engineering company focused on the design of power electronics for the high-tech industry and student solar teams. Created out of the passion for technology and people, we design products that make the difference in simplifying the adoption of innovative technologies. With our experience, we hope to support people with our products in their sustainable tech ventures.
TPEE can supply assembled and tested units, or you can continue exploring the GitHub page if you want to build an SEC-B80-8A yourself. Visit tpee.nl for more information or to get in touch.  

# Hardware #
The Open Smart Energy Converter firmware runs on different hardware types. Currently, the SEC-B80-8A can be used for voltages up to 80V and is fully open source. The SEC-B175-7A can be used for systems up to 175V and is available through TPEE. Customized solutions to suit your needs are also available. The hardware is optimized for efficiency and weight using TPEE's power electronics models. These tools ensure the hardware is some of the most efficient power electronics available and enable TPEE to create customized hardware while keeping the design process quick and precise. Check out [tpee.nl/products](https://www.tpee.nl/roducts) to get your hands on the hardware!

![IMPPT](Pictures/MPPT-STRAIGHT.png)

# Getting started #
This repository contains the main firmware. Alongside this repository, there are two more repositories:

* [ReboostTool](https://github.com/TjitteS/ReboostTool): Configuration tool.
* [ReboostV2 Hardware](https://github.com/TjitteS/ReboostV2-Hardware): Hardware repository.


The minimum tools required to build an Open-SEC are:
* Assembled hardware
* A tool for flashing STM32 microcontroller, like ST-link
* [TAG-Connect](https://www.tag-connect.com/debugger-cable-selection-installation-instructions/st-link-v2) programming cable

The firmware is written in C and is built using the STM32CubeIDE environment. In the environment, two build configurations are defined: Debug and Simulation. the simulation build configuration is used to test firmware. In this build, a simulator is added in instead of the real hardware power electronics. This way it can simulate the control algorithm and other things, without having the risk of destroying the hardware. When enough confidence is built up during testing, the Debug configuration is used to turn the real thing on. Fingers crossed!

After uploading the (stable) firmware using an ST-link with tag connect, the configuration tool can be used to set the hardware settings. Once these are set correctly, the device should be ready for use.

In the tool, an accurate lab power supply can be used to calibrate the voltage and current sensors on the converter. After doing this, the accuracy is much higher and lower tolerance components can be used. Note that this is not strictly necessary for the converter to work.

![Tool](Pictures/tool02.png)

### Measurement and control ###
The control system for this converter is a fully digital system based on the stm32G474 MCU. In the software, relatively simple proportional controllers are used to stabilize the system, and current and voltage limits are implemented such that the system can (hopefully) never destroy itself. 

The digital controller presents the biggest limitation to the minimal power inductor value and the bandwidth of the converter. Hence the faster the controller is, the faster it can respond to disturbances. That is why the controller is made relatively simple, at the cost of being less accurate. Future work to the controller includes making it bi-directional, making use of hardware acceleration, and more accurate controllers. 

# Limitations #
### voltage rating ###
The converter used power GaN FETs that are rated for 100V. Even though I did test the converter up to 90V, it is advised to keep a save margin in your system. I would recommend using this converter hardware in systems till 75 Volt nominally. A lot of choices on this converter are made with higher voltages in mind, but that does require a different power stage.

### Low side current sensing ###
This hardware uses current sensors in the ground path. That means that the two ground connections cannot be shorter together, or the device may break. This also means that the converter cannot be placed in parallel with other converters.

### Licence ###
The project is released under the GNU General Public License version 3.0

### Acknowledgement ###

* This project has its roots in this [open-source MPPT](https://github.com/DieBieEngineering/DieBieMPPT) hardware.
* The configuration tool and interface are loosely based on software from the open-source VESC project, which can be found [here](https://github.com/vedderb/vesc_tool).
