#! /bin/bash

SHELL_FOLDER=$(
    cd "$(dirname "$0")"
    pwd
)

set -e

# echo "remap the device serial port(ttyUSBX) to  rplidar"
# echo "rplidar usb connection as /dev/rplidar , check it using the command : ls -l /dev|grep ttyUSB"
# echo "autolabor usb connection as /dev/autolabor_pro1 , check it using the command : ls -l /dev|grep ttyUSB"
# echo "start copy udev rules to  /etc/udev/rules.d/"
# echo "$SHELL_FOLDER/udev/98-sensors.rules"
# sudo cp $SHELL_FOLDER/udev/98-sensors.rules  /etc/udev/rules.d
# sudo cp $SHELL_FOLDER/udev/99-inputs.rules /etc/udev/rules.d
# echo " "
# echo "Restarting udev"
# echo ""

# sudo service udev reload
# sudo service udev restart

echo "update your system"
sudo apt update
sudo apt -y upgrade

echo "install ros"
sudo apt install -y ros-melodic-desktop-full

echo "uninstall some ubuntu software"
sudo apt remove -y --purge totem empathy gnome-mahjongg aisleriot gnome-mines cheese gnome-sudoku transmission-common \
    gnome-orca webbrowser-app landscape-client-ui-install deja-dup libreoffice-common libreoffice unity-webapps-common \
    thunderbird evolution quake

echo "install some needed libs"
sudo apt install -y \

echo "install some ros-dependencies"
sudo apt install -y \
    ros-melodic-navigation \

echo "install some debug tools, which is optional for onboard computers"
sudo apt install -y \

# sudo snap install cloudcompare

echo "install python dependencies"
sudo apt install -y python-pip
sudo -H pip install pylint pandas numpy openpyxl pyqtgraph

# echo "compile csm"
# cd $SHELL_FOLDER/../../tools/csm-master
# if [ -d "build" ]; then
#     rm -rf build
# fi
# mkdir build
# cd ./build
# cmake ..
# make
# sudo make install

echo "feel good about the keyboard"
sudo chmod 666 /dev/input/by-path/platform-i8042-serio-0-event-kbd

echo "now enjoy!!"
