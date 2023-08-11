# File Driver to create a device /dev/scream like the /dev/zero

### QuickStart

When installed, running:
```bash
cat /dev/scream | head -c 100
```

Prints:
```text
aHAAhhaHHAAHaAaAAAAhhHhhAAaAAAhAaaAAAaHHAHhAaaaaAaHahAaAHaAAHaaHhAHhHaHaAaHAAHaAhhaHaAaAA
```
### Install

Download file

```bash
git clone https://github.com/matlink/dev_scream && cd dev_scream
make build
sudo make install
sudo make load
```

### Uninstall
```bash
sudo make clean
```

### Util

```bash
# Installed modules, see #7
lsmod  # List modules
sudo modprobe one  # Load one driver => creates /dev/one
sudo depmod  # Re-create the module dependency list
sudo modprobe -r one  # Load one driver => removes /dev/one

# Keys
sudo mokutil --list-new  # List key that will be added at boot
sudo mokutil --reset  # Delete future keys
sudo cat /proc/keys  # View your installed keys
dmesg -wH  # Kernel log like tail -f
```

### Source

*  Based on https://github.com/tinmarino/dev_one
*  Device Driver: https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os
*  Signing driver: https://gist.github.com/Garoe/74a0040f50ae7987885a0bebe5eda1aa
*  Mok: https://ubuntu.com/blog/how-to-sign-things-for-secure-boot


### Licence

This project, DevOne, is licensed under the [GPL v2.0 or later](https://spdx.org/licenses/GPL-2.0-or-later.html)
Copyright &copy; 2020-2022 Martin Tourneboeuf (https://tinmarino.github.io)
