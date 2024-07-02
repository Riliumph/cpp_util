FROM mcr.microsoft.com/devcontainers/cpp:debian

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y\
    libboost1.81-all-dev\
    libgtest-dev\
    libpqxx-dev\
    nlohmann-json3-dev\
    peco\
    silversearcher-ag\
    vim\
    # network
    telnet\
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

