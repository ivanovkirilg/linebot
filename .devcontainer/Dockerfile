FROM registry.gitlab.com/ivanovkirilg/linebot

ARG USERNAME=linebotter

# Create a non-root user
RUN usermod --login $USERNAME --move-home --home /home/$USERNAME ubuntu \
    && groupmod --new-name $USERNAME ubuntu \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && sed -i 's/#force_color/force_color/' /home/${USERNAME}/.bashrc

# Install (pre-requisites and) clang
RUN apt -y update && apt -y upgrade \
    && apt -y install lsb-release wget software-properties-common gnupg
RUN --mount=type=bind,source=llvm.sh,target=/tmp/llvm.sh \
    /tmp/llvm.sh 21
# llvm install script adds new apt repo
RUN apt -y update && apt -y upgrade

USER $USERNAME
