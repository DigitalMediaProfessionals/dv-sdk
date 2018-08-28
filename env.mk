OPT=-O3

ARCH=$(shell gcc -print-multiarch)

# For Xilinx ZC706
BDDEF=DMP_ZC706

ifeq ($(ARCH), arm-linux-gnueabihf)

# For on-board compiling (32-bit ARM)
GPP=g++ -mfp16-format=ieee -march=native -mtune=native
GCC=gcc -mfp16-format=ieee -march=native -mtune=native

else

ifeq ($(ARCH), aarch64-linux-gnu)

# For on-board compiling (64-bit ARM)
GPP=g++ -march=native -mtune=native
GCC=gcc -march=native -mtune=native

else

# For Cross-compiling using Xilinx software
SDK=/usr/local/tools/xilinx/Vivado_2018.1_0405_1/SDK/2018.1
SUFFIX=gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin
ARMGCC=$(SDK)/$(SUFFIX)
GPP=$(ARMGCC)/arm-linux-gnueabihf-g++ -static-libstdc++ -fPIC -mfp16-format=ieee
GCC=$(ARMGCC)/arm-linux-gnueabihf-gcc -fPIC -mfp16-format=ieee

endif

endif

# For Arria 10
#ARMGCC := /home/steven.olney/fpgaExpt/a10DevKit/gcc-linaro-arm-linux-gnueabihf-4.8-2014.03_linux/bin/
#GPP := $(ARMGCC)/arm-linux-gnueabihf-g++ -fPIC -mfp16-format=ieee
#BDDEF := DMP_ARRIA10
