/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

Hardware requirements
===================
- Mini/micro C USB cable
- FRDM-MCXA153 board
- FRDM-33926PNBEVM
- DC Motor
- Power supply
- Personal Computer

Board settings
============
Connect pins 1-2 on jumpers JP1, JP2, JP3, JP4, JP5, JP6, JP7, JP8, JP9, JP10
of frdm-33926pnbevm shield board.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Connect J5 with power supply( VPWR with maximum 28v and GND with ground).
3.  Connect J6 with DC motor.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, you can see the logs printed on the terminal based on the menu options user chooses.

LOGS:
=============

 MC33926 Motor controller Example !

 Successfully Applied PCF9957 Configuration

 *********** Main Menu ***************

 1.  Set D2B pin level (Active Low)

 2.  Set D1 pin level (Active high)

 3.  Set INV pin level (Active High)

 4.  Set Slew pin level

 5.  Enable output

 6.  Disable output

 7.  Set Input (IN1 & IN2) pins level

 8.  Set PWM

 9.  Stop PWM

 10. Read feedback current

 11. Check error pin

 Enter your choice :-

main menu will look like this. 
====================================

 Enter your choice :- 1

  Set high for motor operation

  Setting low will put output in tri-state

 Enter 1 for High and 0 for Low :- 1

 D2B pin set to 1

 Please press enter to goto main menu

To set the D2(disable 2) pin.
======================================

 Enter your choice :- 2

  Set low for motor operation

 Setting high will put output in tri-state

 Enter 1 for High and 0 for Low :- 0

 D1 pin set to 0

 Please press enter to goto main menu

To set the D1(disable 1) pin.
=======================================

 Enter your choice :- 3

  Setting this pin from Low to High  will reverse motor rotation

 Enter 1 for High and 0 for Low :- 0

 INV pin set to 0

 Please press enter to goto main menu

To change the direction of moving motor, set INV(inverse) pin. 
=========================================

 Enter your choice :- 4

 Enter 1 for High and 0 for Low :- 0

 SLEW pin set to 0

 Please press enter to goto main menu

To set the SLEW pin either high or low.
=========================================

 Enter your choice :- 7

 1. Write IN1 Pin

 2. Write IN2 Pin

 Enter your choice :- 2

 Enter 1 to set high and 0 to set low :-0

 Please press enter to goto main menu

To set the INT1/INT2 high or low, enter #7.
With INT1 high and INT2 low, motor will move in clockwise/forward direction. 
With INT1 low and INT2 high, motor will move in anti-clockwise/reverse direction. 
=========================================

 Enter your choice :- 5

 Output Enabled
 Please press enter to goto main menu

To let the motor move either forward/backward, enter #5 to enable
=========================================

 Enter your choice :- 6

 Output Disabled
 Please press enter to goto main menu

To disable the motor, enter #6. 
=========================================

 Enter your choice :- 8

 1.  Setup PWM channel from scratch

 2.  Change duty cycle for

 Enter your choice :- 1

 Enter PWM Channel Number (0 for INT1 or 1 for INT2) :- 0

 duty cycle (0 to 100 in multiples of 10) :- 30

 Enter Freq in HZ (1500/2000/3000/4000) :- 1500

 Enter Non PWM Channel Level (0/1) :- 0

 PWM Generated

 Please press enter to goto main menu
 
 Enter your choice :- 5

 Output Enabled
 Please press enter to goto main menu
 

To set the PWM, enter #8 and 
then to enable/start the motor, enter #5. 
===========================================

 Enter your choice :- 8

 1.  Setup PWM channel from scratch

 2.  Change duty cycle only

 Enter your choice :- 2

 duty cycle (0 to 100 in multiples of 10) :- 40

 PWM Generated

 Please press enter to goto main menu

To change the speed of moving motor in between, set the duty cycle by entering #8.

============================================

 Enter your choice :- 9
 PWM stopped

 Please press enter to goto main menu
 
 To stop the PWM, enter #9.
 ===========================================
 
 Enter your choice :- 10
Press Enter to start ADC conversion

 ADC value: 3697
 ADC interrupt count: 1

 Please press enter to goto main menu
 
To draw the value of current and count, enter #10
============================================

 Enter your choice :- 11

  Error detected

 Please press enter to goto main menu

To detect the errror(undervoltage, overvolatge, shortcircuit and Active D1/D2)
enter #11.
=============================================

Note: This example provides APIs to configure 33926PNBEVM Motor contoller. It also provides APIs to perform below mentioned operations: 

1. set D1/D2
2. set input voltage
3. set PWM
4. stop PWM
5. set SLEW pin
6. Read feedback current
7. Error detection
8. enable/disable Output
