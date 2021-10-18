
#DEBUG	= -g -O0
DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include -I.
CFLAGS	= $(DEBUG) -DPLATFORM_LINUX -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

SRC	= main.cpp Unicorn.cpp ../syntheyes.cpp ../ColourWheel.cpp ../NeopixelDriver.cpp ../PosixTiming.cpp ../MicDriver.cpp ../Timing.cpp ../PanelDriver.cpp ../config.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	$(SRC:.cpp=)

eyes:	main.o Unicorn.o ../syntheyes.o ../ColourWheel.o ../NeopixelDriver.o ../PosixTiming.o ../MicDriver.o ../Timing.o ../PanelDriver.o ../config.o
	$Q echo [link]
	$Q $(CC) -o $@ main.o Unicorn.o ../syntheyes.o ../ColourWheel.o ../NeopixelDriver.o ../PosixTiming.o ../MicDriver.o ../Timing.o ../PanelDriver.o ../config.o $(LDFLAGS) $(LDLIBS)

.cpp.o:
	$Q echo [CC] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS) eyes

tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)

depend:
	makedepend -Y $(SRC)

# DO NOT DELETE
