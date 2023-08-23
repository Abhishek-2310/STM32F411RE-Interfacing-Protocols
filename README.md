# STM32F411RE Interfacing Protocols

This repository contains example code and documentation for interfacing protocols with the STM32F411RE microcontroller. The STM32F411RE is a powerful ARM Cortex-M4 microcontroller and supports various communication protocols for seamless interaction with other devices and systems.

## Table of Contents

- [Supported Protocols](#supported-protocols)
- [Getting Started](#getting-started)

## Supported Protocols

This repository provides examples and documentation for the following communication protocols:

- [SPI (Serial Peripheral Interface)](./SPI)
- [I2C (Inter-Integrated Circuit)](./I2C)
- [UART (Universal Asynchronous Receiver-Transmitter)](./UART)
- [CAN (Controller Area Network)](./CAN)

Each protocol directory contains sample code, wiring diagrams, and explanations to help you quickly get started with implementing the respective communication protocol. Each of these protocols could be interfaced via DMA.

## Getting Started
Do the following steps to install a working Linux environment for yourself.  
Note: These instructions assume you have a computer that can execute a 64 bit version of Ubuntu, either natively or in a Virtual Machine.  If you have a 32 bit only computer, you may have to build the compiler from scratch, ad there doesn't appear to be a 32 bit build of the compiler available any more.

1) Download and install Oracle Virtual Box

2) Download and the Ubuntu 20.04.1 LTS  'Desktop' installation .iso file from Canonical, the folks who create the distribution.

3) Install Ubuntu into the VM.  Start Virtual box, and create a new VM.  When you create the virtual disk, allocate at least 20 Gigabytes, the default of 10 Gigabytes is not enough to install all the tools, and have space left to work. Select the .iso file you downloaded as the boot media, and follow the onscreen prompts to install Ubuntu.

4) Boot up the VM.

5) Log in using the credentials you created during installation.

6) Open a terminal window, by hitting CTRL-ALT-T.

7) Type the following commands in order, answer 'Yes' to installing the software and dependencies. If any commands report errors, then stop, and consult with your professor. Replace <your username> with the user name you use to log into the VM. Installing openocd automatically installs the required udev scripts to make the debugger interface accessible by users in the plugdev group.

- sudo apt update
- sudo apt install build-essential
- sudo apt install openocd
- sudo apt install dfu-util
- sudo apt install minicom
- sudo apt install git
- sudo apt install libncurses5
- sudo apt remove modemmanager
- sudo usermod -a -G dialout <your username> (if using VM)

8) Install the compiler. Ubuntu has a package arm-none-eabi-gcc, DO NOT INSTALL IT! While the compiler works, there are versions of the C library missing that are needed for Cortex-M4 development. Instead download the official ARM version of the compiler installation file 'gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2' from the eConestoga course shell, or the latest from Launchpad gcc. (NOTE, this file may be updated from time to time, and may have a slightly different name, adjust paths below to accommodate.) The latest version can be downloaded from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

9) We need to unpack the compiler, and then copy the files into the right location on our system. If you have put the downloaded file in the default location, 'Downloads'. Type the following commands, in order:

```sh
cd Downloads
tar xvf gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2
cd gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux
sudo cp -rv * /usr/local/
cd ..
rm -r gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux
```
10) From the Virtual Box 'Devices'->'Media' menu select 'Mount Guest additions CD', follow the prompts on screen to install the 'Extras'. This will allow better integration between the VM and the Host operating system. Feel free to configure file sharing between the two OS'es, this may help with moving files between the VM and the host operating system.

11) Log out of your session, and log in again. This will make the group selection you added above with usermod active.

12) Plug your development board into a free USB port, and then from the Virtual Box 'Devices'->'USB' menu, select the 'ST-Link...' device to be connected to the VM.

13) With a device plugged in, you should be able to configure minicom to start up and connect with the STM32 target automatically.  To do so, you need to set up the system wide default for minicom to open /dev/ttyACM0, instead of /dev/modem.  Start minicom as the root user like this:
```sh
sudo minicom -s
```
Once minicom starts, select the menu option to change the ports, press the letter key that corresponds to the item that shows '/dev/modem', and change it to '/dev/ttyACM0'.  Press Enter, then hit Escape to return to the previous configuration menu.  Select 'Save as dfl', then exit the program.  From this point on, you should NEVER have to type 'sudo' when you run minicom.  If you find you do, see the last command in step 7.

14) You should now be able to download, unpack, and compile simple monitor as shown in class. The following command, when executed from the src directory will program the target:
```sh
make program
```
15) You can interact with the target by typing:
```sh
minicom -D /dev/ttyACM0
```
or just simply:
```sh
minicom
```
if you have configured the default port correctly, and your target is attached.

To begin using these interfacing protocols with the STM32F411RE microcontroller, follow these steps:

Clone this repository to setup your environment:

   ```sh
   git clone https://github.com/Abhishek-2310/STM32F411RE-Interfacing-Protocols.git
   ```
## Things to Note
### SSD1331 OLED DISPLAY SPI CONNECTIONS
<p align="center">
   <img src = "https://github.com/Abhishek-2310/STM32F411RE-Interfacing-Protocols/blob/master/pics/stm32_spi_conn.jpg" width = "450" height = "300">
   <img src = "https://github.com/Abhishek-2310/STM32F411RE-Interfacing-Protocols/blob/master/pics/oled.jpg" width = "300" height = "300"><br>
   PA5->SCK, PA7->MOSI, PA8-> RES, PA9->CS, PA10->DC 
</p>
