#!/bin/env bash

git submodule update --recursive --init

sudo DEBIAN_FRONTEND=noninteractive apt -y update && sudo DEBIAN_FRONTEND=noninteractive apt -y upgrade

sudo DEBIAN_FRONTEND=noninteractive apt -y install lsb-release wget software-properties-common gnupg # llvm.sh dependencies
sudo .devcontainer/llvm.sh 21

sudo DEBIAN_FRONTEND=noninteractive apt -y update && sudo DEBIAN_FRONTEND=noninteractive apt -y upgrade
