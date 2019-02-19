include ../env.mk

GPPFLAGS = -std=c++11 $(OPT) -Wall

TARGET = reset_mio

all: $(TARGET)

ifeq ($(ARCH), aarch64-linux-gnu)
reset_mio: src/reset_mio_c3.cpp
	$(GPP) $(GPPFLAGS) -o $@ $^
endif

clean:
	rm -fr $(TARGET)
