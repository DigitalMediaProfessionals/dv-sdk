include ../../env.mk

GPPFLAGS = -std=c++11 $(OPT) -Wall

ifeq ($(ARCH), arm-linux-gnueabihf)
temp_sensor: temp_sensor_c2.cpp
	$(GPP) $(GPPFLAGS) -o $@ $^
endif

clean:
	rm -fr temp_sensor
