#!/bin/env bash

git submodule update --recursive --init

# Update/upgrade
sudo DEBIAN_FRONTEND=noninteractive apt -y update && sudo DEBIAN_FRONTEND=noninteractive apt -y upgrade

# Install llvm.sh dependencies
sudo DEBIAN_FRONTEND=noninteractive apt -y install lsb-release wget software-properties-common gnupg
sudo .devcontainer/llvm.sh 21

# Update/upgrade again (llvm install script adds new apt repo)
sudo DEBIAN_FRONTEND=noninteractive apt -y update && sudo DEBIAN_FRONTEND=noninteractive apt -y upgrade

# Populate build/ (make sure CMake extension doesn't try to choose Ninja)
mkdir build ; cd build
cmake .. && make -j

# Refresh VSCode (tab) for the sake of clangd and CMake extensions
code -r $CODESPACE_VSCODE_FOLDER
