FROM mcr.microsoft.com/devcontainers/cpp:debian

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y\
    libboost1.81-all-dev\
    libpqxx-dev\
    googletest\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*

# hadolint for docker
ARG HADOLINT_URL
RUN wget --progress=dot:giga -O /usr/local/bin/hadolint ${HADOLINT_URL} --no-check-certificate\
    && chmod +x /usr/local/bin/hadolint

# build googletest
WORKDIR /usr/src/googletest/
RUN cmake .\
    && make\
    && make install

# user defined in devcontainers
USER 1000
RUN printf "PS1='\[\$(tput setaf 4)\]\u\[\$(tput sgr0)\]@\[\$(tput setaf 2)\]\h(\$(hostname -i))\[\$(tput sgr0)\]:\[\$(tput setaf 3)\]\w\\n\[\$(tput sgr0)\]\\$ '" >> ~/.bashrc
WORKDIR /workspace

ENTRYPOINT [ "/bin/bash" ]
