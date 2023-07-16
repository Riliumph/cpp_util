FROM debian:bookworm-slim

RUN apt-get update &&\
    apt-get install --no-install-recommends -y \
    postgresql-client-15 &&\
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/*

RUN echo "PS1='\u@\h(\$(hostname -i)):\w \\$ '" >> ~/.bashrc

# CMD psql -h 192.168.10.1 -p 5432 -U postgres
