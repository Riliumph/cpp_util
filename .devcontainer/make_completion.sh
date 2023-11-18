#!/bin/bash

bash_completion=$HOME/.config/bash_completion
mkdir -p ${bash_completion}

if type docker &>/dev/null; then
    docker completion bash >${bash_completion}/docker
    echo 'source $HOME/.config/bash_completion/docker' >>$HOME/.bashrc
fi
