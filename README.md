# SEMlibmodule
Common code for small microcontrollers in the SEM project. Also known as 'libmodule'.

## Overview
Originally it was a collection of utilities to assist in writing 'modules' for a previously planned modular system. It tries to be somewhat hardware independent, although there is still some dependence on avr-libc.

It would be coupled with a repository that provided the hardware specific implementation, targetted at a particular microcontroller. These repositories can be found at:
 - [ ] Add libmicavr
 - [ ] Add libarduino_m328

It primarily targets AVR processors, compiled using `avr-gcc`. It is written in C++, but `avr-gcc` only provides the C Standard Library. This means it is more "C with classes" than C++. C++ features up to C++14 are used, as Atmel Studio 7 only ships with GCC 5.4.0.

This repository is used as a submodule in other respositories for the SEM project. It can also be used as an Ardino IDE library.

## Using with the Arduino IDE
These instructions are for Windows. Adjust as necessary for other systems.

#### Set Arduino to use C++14
The line:\
`compiler.cpp.extra_flags=-std=gnu++14`\
needs to be added to the file `C:\Program Files (x86)\Arduino\hardware\arduino\avr\platform.local.txt`.

You can also just paste the `platform.local.txt` file in the root of this repository to the path above.

#### Create Library Zip
Add `keywords.txt`, `library.properties`, and `src/` to a zip archive. We'll call it `libmodule.zip`.
- [ ] Add the zip archive as a release.

#### Add Zip to Arduino IDE
In the Arduino IDE, navigate to Sketch -> Include Library -> Add .ZIP Library.
Select `libmodule.zip`.

#### Add libmodule to the Arduino Project
In the Arduino IDE, navigate to Sketch -> Include Library. You should see `SEMlibmodule vx.x.x`. Select it, and ensure that the line
```cpp
#include <libmodule.h>
```
is before the include statements for any libraries that depend on libmodule.
