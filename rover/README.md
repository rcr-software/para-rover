# Overview

- `README.md` - this file
- `rover.ino` - main file, put all global state and control here.
- `Motors.cpp/.h` - namespace for motor control
- `Buzzer.cpp/.h` - class for making the rover beep a pattern once for warnings and forever for
  errors
- `packets.h` - Structs used to talk over the radio. This file is read by the GUI to interpret
  structs from the rover, so changes only have to happen here.

# Adding a class

1. Always include an "example usage" section near the top, even if it's obvious how to use it
2. Keep the project start shaped. That is, `rover.ino` should import other classes which only rely
on builtin or downloaded libraries. More complex import structure should be carefully debated. Stay
away from using externs.
    - For example, don't make a radio wrapper library, its concerns are dependent on code in `rover.ino`.
      The downloaded radio library requires minimal code already.

# Multi-file organization

Organizing code into multiple files is kind of a mess in Arduino. Buzzer was easy enough to abstract
into a class, using the arduino library tutorial. Motors gave more issues. Some of the initializers
required function pointers and since we don't have lambdas, those needed to be either static methods
or normal functions. Since static methods would defeat the purpose of OO in this use case, I instead
refactored Motors as a namespace.

## Buzzer

An honest to goodness library-grade class. Initialize in rover.ino like any other class.

## Motors

A namespace, which acts more of less like a static class. Call using namespace convention
(Motors::foo()) and also it has an init() function that has to be called.
