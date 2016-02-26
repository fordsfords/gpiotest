# gpiotest
CHIP program to test gpio I/O lines.

## License

Copyright 2016 Steven Ford http://geeky-boy.com and licensed
"public domain" style under
[CC0](http://creativecommons.org/publicdomain/zero/1.0/): 
![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png "CC0")

To the extent possible under law, the contributors to this project have
waived all copyright and related or neighboring rights to this work.
In other words, you can use this code for any purpose without any
restrictions.  This work is published from: United States.  The project home
is https://github.com/fordsfords/gpiotest/tree/gh-pages

To contact me, Steve Ford, project owner, you can find my email address
at http://geeky-boy.com.  Can't see it?  Keep looking.

## Introduction

The [CHIP](http://getchip.com/) single-board computer has two sets of general-purpose I/O lines: 8 "XIO" lines and 8 "CSI" lines.  The "CSI" lines are directly handled by the CPU chip; the "XIO" lines are handled by a separate chip which communicates with the CPU over an I2C serial bus.  This project contains two C programs which measures the speed that the I/O lines can be manipulated in software.

### XIO Measurements

* input: 127 microseconds (lseek/read)
* output: 122 microseconds (write)

### CSI Measurements

* input: 6.7 microseconds (lseek/read)
* output: 2.8 microseconds (write)

You can find gpiotest at:

* User documentation (this README): https://github.com/fordsfords/gpiotest/tree/gh-pages

Note: the "gh-pages" branch is considered to be the current stable release.  The "master" branch is the development cutting edge.

## Quick Start

1. If you haven't set up your CHIP to be able to compile C programs, perform [these instructions](http://wiki.geeky-boy.com/w/index.php?title=CHIP_do_once) up to and including installing gcc.

2. Get the files onto CHIP:

        mkdir gpiotest
        cd gpiotest
        wget http://fordsfords.github.io/gpiotest/xiotest.c
        wget http://fordsfords.github.io/gpiotest/csitest.c
        wget http://fordsfords.github.io/gpiotest/bld.sh

3. Build the package:

        ./bld.sh

4. Test the package:

        sudo ./xiotest
        sudo ./csitest

Yes, I could/should have written a single program which accepts the GPIO number as a command-line argument, but I wrote it before I knew about CSI lines, and once I found out about them, I just did a quick-and-dirty clone of the source file.  This kind of test program is usually used and discarded once the measurements are taken; I'm only saving them as a record of how the measurements were taken.  And yes, I'm feeling guilty and rationalizing laziness.  :-)

## Release Notes

* 0.1 (31-Jan-2016)

    Initial pre-release.
