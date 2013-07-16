# xTitan

eXtreme Toolkit of Intrusive Test AutomatioN

## Driver

### Build Dependency

This project is a C++11 project.
The following toolkits are tested on each platforms.

* Windows: [Microsoft Visual C++] >= 2010
* GNU/Linux: [GNU Compiler Collection] >= 4.6
* Mac OS X: [XCode] >= 4.5
* [CMake] >= 2.6
* [Qt toolkit] >= 4.7

### How To Build

First, use [CMake] to generate build script (e.g.: VC solution, Makefile ...
etc.). There are some usable variables:

* CMAKE_INSTALL_PREFIX: install to this path
* CMAKE_BUILD_TYPE: Release or Debug. VC don't need this.

### Run-time Dependency

It needs **Agent** to work.
The path to **Agent** is configurable.
For more information, please see the fallowing secions.

## Agent

### Build Dependency

Unfortunately, this is a Java project.
You need [Eclipse] to open it.

Install the fallowing libraries, then add them to build path in the Eclipse
project.

* [Sikuli] >= 1.0, as User Library named **Sikuli**
* [Gson], as User Library named **Gson**
* [Commons Codec], as User Library named **Commons Codec**

#### Note for GNU/Linux

If you are using packages from distributions, you may also need to add [Jython]
library. Because package maintainers usually prefer to split them into
different packages.

### Run-time Dependency

[Sikuli] supports [Java Runtime Environment] 6 and 7, but JRE 7 is recommended.


[Commons Codec]: http://commons.apache.org/codec/
[CMake]: http://www.cmake.org/
[Eclipse]: http://www.eclipse.org/
[GNU Compiler Collection]: http://gcc.gnu.org/
[Gson]: http://code.google.com/p/google-gson/
[Java Runtime Environment]: http://www.oracle.com/technetwork/java/javase/downloads/index.html
[Jython]: http://www.jython.org/
[Microsoft Visual C++]: http://msdn.microsoft.com/en-us/vstudio/aa718325
[Qt toolkit]: http://qt.nokia.com/
[Sikuli]: http://sikuli.org/
[XCode]: https://developer.apple.com/xcode/
