OPT=-O3 -fno-strict-aliasing -fwrapv

ARCH=$(shell gcc -print-multiarch)

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

# For Cross-compiling setup GPP and GCC variables
# Depending on the corss-compiler it might be required to add -static-libstdc++ or -static
GPP=aarch64-linux-gnu-g++
GCC=aarch64-linux-gnu-gcc

endif

endif
