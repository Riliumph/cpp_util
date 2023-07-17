# FROM debian:bookworm-slim
FROM ubuntu:latest

ARG USERNAME
ARG UID
ARG GID
ARG WORKDIR

RUN sed -i 's@archive.ubuntu.com@ftp.jaist.ac.jp/pub/Linux@g' /etc/apt/sources.list
RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update &&\
    apt-get install --no-install-recommends -y \
    build-essential \
    gdb \
    libboost-dev \
    libpqxx-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# New user
RUN groupadd -g "$GID" "$USERNAME" &&\
    useradd -l -m -u "$UID" -g "$GID" "$USERNAME" -s /bin/bash
# Login new user
USER $USERNAME
WORKDIR $WORKDIR
RUN chown -R "$USERNAME" "$WORKDIR"
RUN printf "PS1='\u@\h(\$(hostname -i)):\w \\n\\$ '\n" >> ~/.bashrc

CMD ["/bin/bash"]
