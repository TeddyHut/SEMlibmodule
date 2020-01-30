# SEMlibmodule
Common code for small microcontrollers in the SEM project. Also known as 'libmodule'.

## Overview
Originally it was a collection of utilities to assist in writing 'modules' for a previously planned modular system. It tries to be somewhat hardware independent, although there is still some dependence on avr-libc.

It would be coupled with a repository that provided the hardware specific implementation, targetted at a particular microcontroller. These repositories can be found at:
 - [ ] Add libmicavr
 - [ ] Add libarduino_m328

It primarily targets AVR processors, compiled using `avr-gcc`. It is written in C++, but `avr-gcc` only provides the C Standard Library. This means it is more "C with classes" than C++. C++ features up to C++14 are used, as Atmel Studio 7 only ships with GCC 5.4.0.

This repository is used as a submodule in other respositories for the SEM project. It can also be used as an Ardino IDE library.
