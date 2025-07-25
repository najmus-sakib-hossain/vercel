ARG DISTRO_VERSION="12"
FROM docker.io/library/debian:${DISTRO_VERSION}

# Install Dependencies
RUN DEBIAN_FRONTEND="noninteractive" apt-get -qq update && \
    apt-get -qq -y --no-install-recommends install \
    # Build Tools
    build-essential \
    curl \
    libbz2-dev \
    libonig-dev \
    libxml2-utils \
    lintian \
    lsb-release \
    libxml2-utils \
    pandoc \
    # Ghostty Dependencies
    libadwaita-1-dev \
    libgtk-4-dev && \
    # TODO: Add when this is updated to Debian 13++
    # gtk4-layer-shell
    # Clean up for better caching
    rm -rf /var/lib/apt/lists/*

# work around the fact that Debian 12 doesn't ship a pkg-config file for bzip2
RUN . /etc/os-release; if [ $VERSION_ID -le 12 ]; then ln -s libbz2.so /usr/lib/$(gcc -dumpmachine)/libbzip2.so; fi

# Install zig
# https://ziglang.org/download/

COPY . /src

WORKDIR /src

RUN export ZIG_VERSION=$(sed -n -e 's/^.*requireZig("\(.*\)").*$/\1/p' build.zig) && curl -L -o /tmp/zig.tar.xz "https://ziglang.org/download/$ZIG_VERSION/zig-linux-$(uname -m)-$ZIG_VERSION.tar.xz" && \
    tar -xf /tmp/zig.tar.xz -C /opt && \
    rm /tmp/zig.tar.xz && \
    ln -s "/opt/zig-linux-$(uname -m)-$ZIG_VERSION/zig" /usr/local/bin/zig

# Debian 12 doesn't have gtk4-layer-shell, so we have to manually compile it ourselves
RUN zig build \
    -Doptimize=Debug \
    -Dcpu=baseline \
    -Dapp-runtime=gtk \
    -fno-sys=gtk4-layer-shell

RUN ./zig-out/bin/ghostty +version

