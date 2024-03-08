FROM ubuntu:jammy

ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
    bash-completion \
    build-essential \
    ca-certificates \
    ccache \
    cppcheck \
    file \
    gcovr \
    gdb \
    git \
    gpg-agent \
    lcov \
    libgtest-dev \
    python3 \
    python3-pip \
    software-properties-common \
    ssh \
    sudo \
    valgrind \
    wget

RUN export DEBIAN_FRONTEND=noninteractive \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 19 all

RUN export DEBIAN_FRONTEND=noninteractive \
    && find /usr/lib/llvm-19/bin \( -type f -o -type l \) -print0 \
    | xargs -0 -I %% \
    bash -c 'update-alternatives --install /usr/bin/$(basename "${1}") $(basename "${1}") "${1}" 100' \
    -- '%%'

# https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test
RUN export DEBIAN_FRONTEND=noninteractive \
    && add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    gcc-13 \
    g++-13

# https://apt.kitware.com/
RUN export DEBIAN_FRONTEND=noninteractive \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
    | gpg --dearmor - \
    | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' \
    | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt-get update -qq \
    && apt-get install -y --no-install-recommends \
    cmake

# Set locale
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y \
    locales \
    locales-all

ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8

# Clean
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create the user
RUN export DEBIAN_FRONTEND=noninteractive \
    && groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
WORKDIR /home/$USERNAME
