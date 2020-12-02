# ttos-x64

## Required software
### Compiling
- [Make](http://gnuwin32.sourceforge.net/packages/make.htm) (if on Windows)
### Emulating
- [QEMU](https://www.qemu.org/)
## Compiling the OS
It's actually pretty simple. On your terminal, `cd` into the root folder and type:

```
cd kernel
make setup
make kernel
cd ../gnu-efi
make bootloader
cd ../kernel
make buildimg
```

## Emulating the OS

On the `kernel` folder, run `make run` if you're emulating it on Linux or just click `run.bat` if you're on Windows.

