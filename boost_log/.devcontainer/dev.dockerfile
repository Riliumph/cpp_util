FROM ubuntu:latest

RUN apt-get update \
    && apt-get install --no-install-recommends -y\
    sudo\
    wget\
    build-essential\
    libboost-all-dev\
    gdb\
    googletest\
    cmake\
    && apt-get clean\
    && rm -rf /var/lib/apt/lists/*
# hadolint for docker
ARG HADOLINT_URL
RUN wget --progress=dot:giga -O /bin/hadolint ${HADOLINT_URL} --no-check-certificate
# build googletest
WORKDIR /usr/src/googletest/
RUN cmake .\
    && make\
    && make install

# New user
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN groupadd -g 1000 docker\
    && useradd -l -u 1000 -g docker -G sudo -m -s /bin/bash docker\
    && echo 'docker:docker' | chpasswd\
    && echo 'ALL ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER docker
RUN printf "PS1='\u@\h(\$(hostname -i)):\w \n\\$ '" >> ~/.bashrc
WORKDIR /workspace

ENTRYPOINT [ "/bin/bash" ]
