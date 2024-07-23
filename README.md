# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

# FRDM33926PNBEVM: Driver and demo app for MC33926 motor controller using MCXA153 and MCXN947 baseboards
The FRDM-33926PNBEVM evaluation board (EVB) provides a development platform
that exercises all the functions of the MC33926 H-bridge IC using MCU. 

The 33926 is a SMARTMOS monolithic H-bridge power IC designed primarily for
automotive electronic throttle control, but is applicable to any low-voltage DC servo motor
control application within the current and voltage limits.

This example uses the FRDM boards(MCXA153 & MCXN947) to show how the 33926 interfaces with an MCU.
### FRDM-33926PNBEVM Block Diagram

[<img src="./images/block_diag.PNG" width="500"/>](block_diag.PNG)

### Key Features of FRDM-33926PNBEVM LED Driver

- 5.0 V to 28 V continuous operation (transient operation from 5.0 V to 40 V)
- Overcurrent limiting (regulation) via an internal constant-off-time PWM
- Output short-circuit protection (short to VPWR or ground)
- Temperature dependent current limit threshold reduction
- All inputs have an internal source/sink to define the default (floating input) states.

#### Boards: FRDM-MCXA153, FRDM-MCXN947
#### Categories: Motor Control
#### Peripherals: ADC, GPIO, PWM
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [Support](#step6)
6. [Release Notes](#step7)

## 1. Software<a name="step1"></a>
- [IoT Sensing SDK (ISSDK) v1.8](https://nxp.com/iot-sensing-sdk) offered as middleware in MCUXpresso SDK for supported platforms
- [MCUXpresso IDE v11.9.0](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE)

## 2. Hardware<a name="step2"></a>
- FRDM-MCXN947 and FRDM-MCXA153 MCU board
- [FRDM-33926PNBEVM](https://www.nxp.com/design/design-center/development-boards-and-designs/analog-toolbox/freedom-kit-mc33926-5-0-a-throttle-h-b:FRDM33926PNBEVM?fpsp=1#documentation) 
- Personal Computer
- Mini/Micro USB Type C USB cable
- Jumper pins/wires
- DC Motor
- Volatge supplier

## 3. Setup<a name="step3"></a>
### 3.1 Step 1: Download and Install required Software(s)
- Install MCUXpresso IDE 11.9.0
- Download and Install [MCUXpresso SDK v2.14.0 for FRDM-MCXN947](https://mcuxpresso.nxp.com/en/builder?hw=FRDM-MCXN947). Make sure to select ISSDK  middleware while building SDK.
- Download and Install [MCUXpresso SDK v2.14.2 for FRDM-MCXA153](https://mcuxpresso.nxp.com/en/builder?hw=FRDM-MCXA153). Make sure to select ISSDK  middleware while building SDK.
- Install Git v2.39.0 (for cloning and running west commands).
- Install Putty/Teraterm for UART.
 
### 3.2 Step 2: Clone the APP-CODE-HUB/dm-frdm33926pnbevm-motor-controller-with-demo-app
- Clone this repository to get the example projects:
- Change directory to cloned project folder:<br>
    cd *dm-frdm33926pnbevm-motor-controller-with-demo-app*
 
**Note:** If you are using Windows to clone the project, then please configure filename length limit using below command
**git config --system core.longpaths true**

### 3.3 Step 3: Build example projects
- Open MCUXpresso IDE and select a directory to create your workspace.
- Install MCXUpresso SDK 2.14.x for FRDM-MCX947, FRDM-MCXA153 (drag and drop SDK zip into "Installed SDK" view) into MCUXpresso IDE.
- Go to "Quickstart Panel" and click on "Import Project(s) from file system",
- Select "Project directory (unpacked)" and browse to the cloned project folder.
- Select example projects that you want to open and run.
- Right click on project and select build to start building the project.

## 4. Test Application Steps<a name="step4"></a>
- User needs to connect the input pin(J5) of shield board(33926pnbevm) to voltage supply and output pin (J6) to DC motor as shown below:

  [<img src="./images/motorcontrol.jpg" width="400"/>](motorcontrol.jpg)

- NOTE: For MCXN947 base board, two different types of rework needs to be done on two different versions of board. Scan the QR or check the mentioned SCH number and revision number on the MCXN947 board to find the correct REV version.

  This rework needs to be done because, in MXN947, by default INT1 & INT2 lines are not connected to PWM1 channel due to which we will be unable to generate PWM signals as an input.
  
  REV A - following shown shorting should be done on SJ3 & SJ4

  [<img src="./images/Image1.jpeg" width="400"/>](Image1.jpeg)

  REV B - following shown shorting should be done on SJ3 & SJ4
  
  [<img src="./images/Image2.jpg" width="400"/>](Image2.jpg)

- User needs to check COM port after connecting USB cable between Host PC and Target Board via device manager.

[<img src="./images/device_manager.PNG" width="400"/>](device_manager.PNG)

- Open PUTTY/Teraterm application installed on your Windows PC with Baudrate 115200 and assigned COM port as mentioned in above step.

[<img src="./images/Putty_Serial_Terminal.PNG" width="400"/>](Putty_Serial_Terminal.PNG)

- After right click on project and select "Debug As", Demo application will run in interactive mode. When the demo runs successfully, you can see the logs printed on the terminal.

## 4.1 Logs Results<a name="step4"></a>

**Main Menu will look like this**

[<img src="./images/log1.PNG" width="600"/>](log1.PNG)


**Enter #1 to set disable pin(D2B)** 

- User can either disable the motor device by setting it low or make the motor operational by setting the pin high.   

  [<img src="./images/log2.PNG" width="600"/>](log2.PNG)


**Enter #2 to set another disable pin(D1)** 

- User can either disable the motor device by setting it high or make the motor operational by setting the pin low.   

  [<img src="./images/log3.PNG" width="600"/>](log3.PNG)


**Enter #3 to change the direction of motor** 

- User can reverse/forward the moving motor by entering #3. 

  [<img src="./images/log4.PNG" width="600"/>](log4.PNG)


**Enter #4 to set the SLEW pin** 

- User can set the slew pin either high or low.(Tho, it has no impact to the output of motor) 

  [<img src="./images/log9.PNG" width="600"/>](log9.PNG)


**Enter #7 to set the input pins(INT1/INT2)**   

- User can write the INT1/INT2 high or low as per requirements.

- With INT1 high and INT2 low, motor will move in clockwise/forward direction. 
- With INT1 low and INT2 high, motor will move in anti-clockwise/reverse direction. 
  
  [<img src="./images/log5.PNG" width="600"/>](log5.PNG)


**To enable/disable the motor** 

- To move the motor, firstly set D1, D2B in operational mode. 
- Then, set the INT1/INT2 pins.
- Enable the motor by entering #5

  [<img src="./images/log6.PNG" width="600"/>](log6.PNG)

- To disable the motor, enter #6

  [<img src="./images/log8.PNG" width="600"/>](log8.PNG)


**To enable/disable the PWM** 

- User can move the motor by generating PWM at INT1/INT2.
- To move the motor, firstly set D1, D2B in operational mode. 
- Then, set the PWM from scratch by entering #8 ( log shown below).
- Enable the motor by entering #5

  [<img src="./images/log7.PNG" width="600"/>](log7.PNG)

- To change the speed of moving motor in between, enter #8 to change the duty cycle. 

  [<img src="./images/log10.PNG" width="600"/>](log10.PNG)

- To stop the PWM, enter #9. 

  [<img src="./images/log14.PNG" width="600"/>](log14.PNG)


**Enter #10 to read the current drawn by the motor(output)** 

- User can read the ADC value of feedback current and ADC count by entering #10

  [<img src="./images/log11.PNG" width="600"/>](log11.PNG)


**Error detection** 

- User can detect the undervoltage, overvoltage, shortcircuit, Active low D2B pin and Active high D1 pin by entering #11.

- D5 LED on shield board will also glow.  

  [<img src="./images/log12.PNG" width="600"/>](log12.PNG)

- If error is not there, following prompt will be shown.

  [<img src="./images/log13.PNG" width="600"/>](log13.PNG) 


## 5. Support<a name="step6"></a>

#### Project Metadata
<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXA153-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXA153+in%3Areadme&type=Repositories) [![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXN947-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXN947+in%3Areadme&type=Repositories)


<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-MOTOR%20CONTROL-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+motor_control+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-ADC-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+adc+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-GPIO-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+gpio+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-PWM-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+pwm+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Note**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 6. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        | Aug 30<sup>th</sup> 2024 |

