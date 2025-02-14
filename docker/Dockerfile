FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get -y update && apt-get install -y --no-install-recommends \
    curl gnupg software-properties-common autoconf cmake ninja-build gcc g++ \
    linux-tools-common linux-tools-generic libtool llvm-19-dev lld-19 clang-19 \
    libomp-19-dev libopenmpi-dev openmpi-bin git make imagemagick ffmpeg \
    && curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && apt-add-repository "deb http://apt.llvm.org/`lsb_release -c | cut -f2`/ llvm-toolchain-`lsb_release -c | cut -f2`-19 main" \
    && apt-get autoremove -y --purge \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Set up alternatives for LLVM tools
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 10 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-19 \
    && update-alternatives --install /usr/bin/opt opt /usr/bin/opt-19 10 \
    && update-alternatives --install /usr/bin/lld lld /usr/bin/lld-19 10 \
    && update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-19 10

# Create a symbolic link for ImageMagick
RUN ln -s /usr/bin/convert /usr/bin/magick

# Install Samply
RUN curl --proto '=https' --tlsv1.2 -LsSf https://github.com/mstange/samply/releases/download/samply-v0.13.1/samply-installer.sh | sh

# Reset frontend setting
ENV DEBIAN_FRONTEND=
