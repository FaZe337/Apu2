# Pesci-apu


**Disclaimer: This code is for educational purposes only.**

# Credits
arturzxc, Koelion, unguhelvu, Azreol, Acknowledge

unknowncheats: basic knowledge and offsets

# Installation
**1. Install dependencies**

    sudo apt-get install -y libudev-dev
    sudo apt install cmake xorg-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    sudo apt-get install build-essential
    sudo apt-get install libx11-dev
    sudo apt-get install libxtst-dev

**2. Build glfw**

     git clone https://github.com/glfw/glfw.git
     cd glfw
     mkdir build
     cd build
     cmake ..
     make
     sudo make install

**3. Clone, Build and Run repo**

     git clone https://github.com/Pesci-Apu/Apu/
     cd Apu/Pesci
     cmake .
     make
     sudo ./apu
    
**5. Press Insert to toggle the Menu (You can not click outside of the menu when it is open)**
