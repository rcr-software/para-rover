
Organizing code into multiple files is kind of a mess in Arduino. Buzzer was easy enough to abstract
into a class, using the arduino library tutorial. Motors gave more issues. Some of the initializers
required function pointers and since we don't have lambdas, those needed to be either static methods
or normal functions. Since static methods would defeat the purpose of OO in this use case, I instead
refactored Motors as a namespace.

# Buzzer

An honest to goodness library-grade class. Static initialize in rover.ino like any other class.

# Motors

A namespace, which acts more of less like a static class. Call using namespace convention
(Motors::foo()) and also it has an init() function that has to be called.
