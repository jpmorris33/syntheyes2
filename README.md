# syntheyes2
A more advanced eye display for Raspberry Pi and Pico


V1.00 - Initial PUT


V1.01 - Improved blushing/overlay support, status LED on pins 39 and 40, enable all expressions for Xerian

        In v1.01, the following pin assignments are used on the Pi version, which rationalises things a bit better:

                Pin 40 - Startled (Xerian) or OwO (Arthi)
                Pin 38 - "Fault" - blinking X-eyes (both characters)
                Pin 37 - Blushing (both characters)
                Pin 36 - Annoyed (both characters)
                Pin 33 - Eye roll (Xerian only)
                Pin 32 - Happy (both characters)

                Pin 35 on the Transmitter will also be set low to echo the 'Fault'
                expression, e.g. to a separate processor for the status lights.

                In addition, an LED can be soldered between pins 39 and 40 of the Receiver, which will flash to
                indicate to the wearer that an expression change has been selected.
                Make sure the short leg (-ve) is connected to pin 39 (Ground) or it won't work

                The status LED duration can be overridden in eyeconfig.txt,
                e.g. "ackcount: 2"  ...the default value is 4.

V1.02 - Dynamic pin assignment via eyeconfig.txt on Pi, cooldown timer, eye off mode for testing

V1.03 - Pi version can now switch to Raptor's Den sprites via eyeconfig.txt, e.g. "sprites: raptor"
        Added extra animations so Xerian supports OwO and Raptor supports startled and happy.
        Raptor doesn't yet have a roll-eye animation, and will run the annoyed face instead

The Pico drivers use some code from the Pico examples, which are 3-clause BSD licensed.

Makefiles are provided for the Pico with Hub08 driver, and the Pi with a Unicorn Hat HD driver.
The Linux Makefile is for testing and should not be used.


The Pi version can have the eye colour set at runtime by using an eyeconfig.txt file.
This should be put in the /boot partition of the Raspberry Pi SD card so that it can be edited from Windows.
If the file is not present it will default to the compile-time settings.

The Pi version requires the WiringPi library, and is intended for use with two Raspberry Pi Zeroes, each driving a single Unicorn HD board.
One must be set as the controller by grounding pin 29 (BCM5), or passing in a commandline parameter if you prefer.
The two units are linked together via the TX and RX pins (TX on the controller links to RX on the receiver) and the receiver will listen
for commands via this link.  Don't forget to ensure that there's a ground connection between both units or it won't work.

It is strongly recommended to have the same version of the software on both units.

The controller should also have a few GPIO pins spare for controlling the expression.  I use pins 31-40 for this - momentarily connecting
them to ground will trigger the expression change.

Because the Unicorn is a HAT board, you will need to modify the headers so that the pins we need are sticking out the bottom of the Pi and
not blocked by the display module.
The Zero is ideal for this, not only because it's compact but also because it usually comes with the headers unpopulated.  This means that
you can use a pair of needle-nosed pliers or a similar tool to push the pins down into the plastic spacers prior to soldering so they poke
out the bottom of the board.  This trick is particularly useful for the RX/TX/GND lines needed to link the two Pi units, though it does
make the pin shorter than normal.
For pins 29-40 it is probably better to cut the header in two at that point so the GPIO pins can be installed on the rear of the board.

To install the software, it is probably best to modify /etc/rc.local and add the following line to the end so the software is run on boot:
/bin/nice -20 /boot/eyes

...this will give you a 30 second boot time (which I would definitely like to improve on) but also means that the executable can be
copied to the /boot partition, and therefore the binary and config can be updated directly from windows if the end-user needs to do this.

I would strongly recommend editing /ets/fstab so that the root partition is mounted read-only - this will preserve the life of the SD card
and also helps protect the OS from filesystem corruption when the power is cut to the eye module.

V1.04 - Add error message if the comms port cannot be opened
        Override serial port via config with e.g. serial: /dev/tty1234

V1.05 - Add new rainbow and colour cycle effects (experimental, not tested on Pico yet!)
        To enable this, add one of the following lines to the config file:
                effect: rainbow_h
                effect: rainbow_v
                effect: cycle
        Speed can be set with
                rainbowspeed: 10
        ...10 is the default.  Larger numbers will slow it down, smaller numbers will speed it up but increase the risk of glitching

V1.06 - Fix display glitches, hopefully for good
        Rewrote effects engine to use patterns
        Added new effect: rainbow_o		

V1.07 - New sprite set with Raptors Den eyes raised 4 pixels
	Use fonts for version number
	Use fixed timing engine from V3
V1.08 - Add "rotate180: true" config option to rotate display
