#!/bin/bash

# Script to compile and install kernel

# Update the dependencies
echo "Installing/Updating Dependencies ..."
sudo apt-get install build-essential ncurses-dev xz-utils libssl-dev bc

cd ~/linux-4.14.13

echo "Copying existing boot config file ..."
#cp /boot/config-$(uname -r) .config

echo "Building the kernel ..."

#make menuconfig
make -j4
sudo make modules_install -j4
sudo make install -j4

echo "Build Finished ..."

echo "Run command: ./systeminfo.sh to see the new OS info" 
