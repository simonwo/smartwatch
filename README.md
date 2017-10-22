# The W-0 Smartwatch software

This is a project to create a low-power, device independent smartwatch. This repository contains the software that will run on the watch.

## Features
* Low-power, sleeping between RTC interrupts to update an e-paper display
* Device independent, not requiring a companion app or device to update
* Highly customisable at runtime, displaying information in the most contextually helpful way

## Applications
* Dashboard (time, date and customisable symbols)
* Lists (tasks, notes, address book)
* Calendar

## Deliberately _missing_ anti-features
* Display of SMS, phone calls, emails
* Heart-rate monitor or step counter
* GPS/maps

## Supported hardware
* mbed LPC1768

## Dependencies
* `GDSP0150C1/` - from https://developer.mbed.org/users/ivo_n/code/GDEP015OC1/#1f1e705a5f02
* `mbed/` - (not supplied) generated from an mbed online compiler export for your chip from https://os.mbed.com/users/mbed_official/code/mbed/export
  At the moment, Makefile assumes LPC1768
