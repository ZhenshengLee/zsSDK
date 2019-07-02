#!/usr/bin/env bash

dir="`dirname \"$0\"`"

# echo ${dir}

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${dir}/../build/thirdparty/fastrtps-install/lib
