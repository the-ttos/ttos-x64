# ttos-x64

## Required software
- [QEMU](https://www.qemu.org/download/)

## Compiling the OS
Make sure to install the build tools by running

`sudo apt-get install build-essential mtools`

Everytime you change the source code, make sure you are in the `kernel` directory and run

`make bake`
## Emulating the OS
In the `kernel` directory, run `make run` if you are emulating it on Linux or just click `run.bat` if you are on Windows.
## Credits
EFI API and QEMU templates by [Absurdponcho](https://github.com/absurdponcho)
