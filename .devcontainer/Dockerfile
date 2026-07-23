FROM mcr.microsoft.com/devcontainers/base:2-debian12 AS toolchain

ARG DOCKER_URL="https://download.docker.com/linux/debian"
ARG DOCKER_KEY_PATH="/etc/apt/keyrings/docker.asc"

ENV DEBIAN_FRONTEND="noninteractive"
RUN apt-get -qq update \
    && apt-get -qq -y install --no-install-recommends \
    ca-certificates \
    gnupg
RUN curl -fsSL "${DOCKER_URL}/gpg" -o "${DOCKER_KEY_PATH}"; \
    chmod a+r "${DOCKER_KEY_PATH}"; \
    echo "deb [arch=$(dpkg --print-architecture) signed-by=${DOCKER_KEY_PATH}] ${DOCKER_URL} $(. /etc/os-release && echo "$VERSION_CODENAME") stable" \
    > /etc/apt/sources.list.d/docker.list;
RUN apt-get -qq update \
    && apt-get -qq -y install --no-install-recommends \
    docker-ce \
    docker-ce-cli \
    containerd.io \
    docker-buildx-plugin \
    docker-compose-plugin \
    && rm -rf /var/lib/apt/lists/*

# hadolint
ARG HADOLINT_URL="https://github.com/hadolint/hadolint/releases/download/v2.12.0/hadolint-Linux-x86_64"
RUN curl -fsSL "${HADOLINT_URL}" -o /usr/local/bin/hadolint
# diff-highlight
WORKDIR /usr/share/doc/git/contrib/diff-highlight
RUN make
# completion
WORKDIR /etc/bash_completion.d
RUN docker completion bash > /etc/bash_completion.d/docker


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
RUN DEBIAN_FRONTEND=noninteractive \
    apt-get -qq update \
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
COPY --from=toolchain /usr/local/bin/hadolint /usr/local/bin/hadolint
# git
COPY --from=toolchain /usr/share/doc/git/contrib/diff-highlight/diff-highlight /usr/local/bin/diff-highlight
# completion
COPY --from=toolchain /etc/bash_completion.d/docker /etc/bash_completion.d/docker
