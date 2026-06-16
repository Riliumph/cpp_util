FROM debian:12-slim AS hadolint
ARG HADOLINT_URL
RUN apt-get update -qq && apt-get install -y wget && \
    wget -q -O /hadolint ${HADOLINT_URL} --no-check-certificate

FROM mcr.microsoft.com/devcontainers/cpp:2-debian12

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
    chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # c++ libraries
    libboost-all-dev\
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
    # bear\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # utilities
    peco\
    silversearcher-ag\
    vim\
    # network tools
    telnet\
    netcat-openbsd\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

# hadolint for docker
COPY --from=hadolint /hadolint /usr/local/bin/hadolint

WORKDIR /usr/share/doc/git/contrib/diff-highlight
RUN make\
    && chmod o+x diff-highlight\
    && ln -s /usr/share/doc/git/contrib/diff-highlight/diff-highlight /usr/local/bin
