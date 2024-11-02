#!/bin/bash
# Copyright (C) 2024 LJ and licensed under GPL3.0

linux_install() {
    sudo mv -f kim /usr/bin
    sudo mv -f .kimrc /etc
    sudo mv -f kim.desktop /usr/share/applications
    sudo mv -f kim_icon.png /usr/share/icons
}

bsd_install() {
    sudo mv -f kim /usr/bin
    sudo mv -f .kimrc /etc
    sudo mv -f kim.desktop /usr/local/share/applications
    sudo mv -f kim_icon.png /usr/local/share/icons
}

detect_os=$(uname -o)
if [ $detect_os == "GNU/Linux" ]; then 
    linux_install
elif [ $detect_os == "FreeBSD" ]; then 
    bsd_install
else
    echo "Error: OS unknown"
fi

