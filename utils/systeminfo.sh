#!/bin/bash

# Script which collects important info about OS
# The contents can be viewed in file "systemInfo"

name="systemInfo"

echo "**********************************************************************" > $name;
echo "			Operating System Information								" >> $name;
echo "**********************************************************************" >> $name;

echo -e "User Info\t\t: $(uname -n)"										 >> $name;
echo -e "\nOS Type/brand\t\t: $(uname -o)"									 >> $name;
echo -e "\nOS Distribution\t\t: $(lsb_release -i -s)"						 >> $name;
echo -e "\nOS version\t\t: $(lsb_release -r -s)"							 >> $name;
echo -e "\nKernel version\t\t: $(uname -r)"									 >> $name;
echo -e "\nKernel gcc version build: $(gcc --version)"						 >> $name;
echo -e "\nKernel Build Time\t: $(uname -v)"								 >> $name;
echo -e "\nSystem Architecture Info: $(uname -m)"							 >> $name;
echo -e "\nFile System Memory Info:\n$(df -h)" 								 >> $name;

echo "**********************************************************************" >> $name;
