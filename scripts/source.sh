#! /usr/bin/zsh

SHELL_FOLDER=$(
    cd "$(dirname "$0")"
    pwd
)


if [ -d "/home/zs/zsGit/zsROS2/build/thirdparty/k4a-install/lib" ] ; then
    LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/zs/zsGit/zsROS2/build/thirdparty/k4a-install/lib"
fi

if [ -d "/home/zs/zsGit/zsROS2/build/thirdparty/k4a-install/bin" ] ; then
    PATH="$PATH:/home/zs/zsGit/zsROS2/build/thirdparty/k4a-install/bin"
fi

export PATH LD_LIBRARY_PATH
