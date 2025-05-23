FROM mcr.microsoft.com/devcontainers/cpp:debian

RUN echo "INSTALL UTILITIES"
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends -y\
    # utilities
    peco\
    silversearcher-ag\
    vim\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

RUN echo "INSTALL NETWORK TOOLS"
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y\
    telnet\
    netcat-openbsd\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

RUN echo "INSTALL C++ TOOLS"
RUN apt-get update -qq \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qq --no-install-recommends -y\
    # c++ libraries
    libboost1.81-all-dev\
    libgtest-dev\
    libpqxx-dev\
    nlohmann-json3-dev\
    libspdlog-dev\
    # clang
    clang\
    clang-format\
    clang-tools\
    # static analysis
    cppcheck\
    clang-tidy\
    bear\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

# hadolint for docker
ARG HADOLINT_URL
RUN wget -q -O /usr/local/bin/hadolint ${HADOLINT_URL} --no-check-certificate\
    && chmod +x /usr/local/bin/hadolint

WORKDIR /usr/share/doc/git/contrib/diff-highlight
RUN make\
    && chmod o+x diff-highlight\
    && ln -s /usr/share/doc/git/contrib/diff-highlight/diff-highlight /usr/local/bin

