Using lsusb -v, I was able to find the idVendor and idProduct

udevadm monitor

dmesg

udevadm test --action=add /devices/platform/ehci-omap.0/usb1/1-2/1-2.3/1-2.3:1.1/input/input10

udevadm info -a  /dev/input/by-path/pci-0000:00:14.0-usb-0:1:1.0-event-kbd
