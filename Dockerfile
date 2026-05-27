FROM ubuntu:25.10

ENV DEBIAN_FRONTEND=noninteractive
RUN apt -y update && apt -y upgrade \
    && apt -y install sudo git build-essential doxygen python3 python3-requests lcov plantuml default-jdk

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# The following is apparently the recommended install method for CMake
RUN apt -y install ca-certificates gpg wget \
    && test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt -y update \
    && apt -y install cmake
# End install cmake
