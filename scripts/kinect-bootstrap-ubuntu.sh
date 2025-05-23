#!/usr/bin/env bash

# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

# Setup machine to build amd64 and i386
sudo dpkg --add-architecture amd64

# Update list of packages
sudo apt update

# Install tools needed to build
sudo apt install -y \
    pkg-config \
    ninja-build \
    doxygen \
    clang \
    gcc-multilib \
    g++-multilib \
    python3 \
    git-lfs \
    nasm \
    cmake

# Install libraries needed to build
sudo apt install -y \
    libgl1-mesa-dev \
    libsoundio-dev \
    libvulkan-dev \
    libx11-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxrandr-dev \
    libusb-1.0-0-dev \
    libssl-dev \
    libudev-dev \
    mesa-common-dev \
    uuid-dev

curl https://packages.microsoft.com/keys/microsoft.asc | sudo apt-key add -
# 下下来自己添加
sudo apt-add-repository https://packages.microsoft.com/ubuntu/18.04/prod
sudo apt-get update
sudo apt install k4a-tools libk4a1.3 libk4a1.3-dev
