# DV SDK

## Get repositories

Use the following commands to get all SDK repositories.
```console
$ git clone https://github.com/DigitalMediaProfessionals/dv-sdk
$ cd dv-sdk
$ git submodule init
$ git submodule update --remote
```

## Build

All modules in the SDK can be directly built on the distributed Ubuntu environment on the board. If one wants to cross compile on a PC, please edit the env.mk and set the first variable to where the binaries of cross compiler are located.

## Wiki

Please see the wiki pages for [quick starting guides](https://github.com/DigitalMediaProfessionals/dv-sdk/wiki/Quick-Start) and [user manuals](https://github.com/DigitalMediaProfessionals/dv-sdk/wiki/User-manual).
