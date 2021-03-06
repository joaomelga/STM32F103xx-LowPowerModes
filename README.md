# STM32F103xx-LowPowerModes

Copyright (C) 2020 João Melga & Marcelo Braga

Created on: mar-08-2021

Institution: Universidade Federal de Minas Gerais - UFMG

### Introduction
This repository was developed as an assignment of the Embedded Systems Programming discipline at UFMG (Universidade Federal de Minas Gerais) - Prof. Ricardo de Oliveira Duarte - Department of Electronic Engineering.

Embedded systems are designed to perform a specific task handling limited resources and accomplishing a list of prerequisites, where some of these are directly impacted by the microcontroller power consuming characteristics. Behind various electronic products, there is a microcontroller sourced by batteries, and when it is important to ensure your system will not go down by lack of energy, it is also important to know how you can optimize the power consumption.
This repository was created to give an overview and an example application of the low-power modes of STM32F1 microcontroller family.

### Content
This repository contains:
- An Application Note PDF file ["Using low power modes in STM32F1 microcontrollers"](https://github.com/joaomelga/STM32F103xx-LowPowerModes/blob/main/Using%20low%20power%20modes%20in%20STM32F1%20microcontrollers.pdf) on the STM32F1 Low-power modes topic,  with the following contents:
1.  Run mode
2.	Low-power modes 
3. 	Low power modes application example
4.	Estimating power consumption with PCC tool in CubeMX
5.	Low-power modes with FreeRTOS

- A demo application, detailed in "3. Low power modes application example" developed under the stack SW4STM32 + CubeMX + HAL api.
