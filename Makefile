RGB_LIB_DISTRIBUTION=/home/pi/rpi-rgb-led-matrix

all : MorphingClock
CXXFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter

MorphingClock: Digit.cpp MorphingClock.cpp
	g++ -I$(RGB_LIB_DISTRIBUTION)/include $(CXXFLAGS) Digit.cpp MorphingClock.cpp -o $@ -L$(RGB_LIB_DISTRIBUTION)/lib -lrgbmatrix -lrt -lm -lpthread

clean:
	rm -f MorphingClock
