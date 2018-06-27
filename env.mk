SDK=/usr/local/tools/xilinx/Vivado_2018.1_0405_1/SDK/2018.1
SUFFIX=gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin
ARMGCC=$(SDK)/$(SUFFIX)
GPP=$(ARMGCC)/arm-linux-gnueabihf-g++ -static-libstdc++ -fPIC -mfp16-format=ieee -fstrict-volatile-bitfields
GCC=$(ARMGCC)/arm-linux-gnueabihf-gcc -fPIC -mfp16-format=ieee -fstrict-volatile-bitfields
BDDEF=DMP_ZC706

#ARMGCC=/home/steven.olney/fpgaExpt/a10DevKit/gcc-linaro-arm-linux-gnueabihf-4.8-2014.03_linux/bin/
#GPP=$(ARMGCC)/arm-linux-gnueabihf-g++ -fPIC -mfp16-format=ieee 
#BDDEF=DMP_ARRIA10

