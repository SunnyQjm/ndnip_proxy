#!/usr/bin/env bash
ip=$1
port=$2
sudo nlsrc withdraw /NDN-IP/FILE_SLICE/${ip}/${port}
sudo nlsrc withdraw /NDN-IP/FILE_INFO/${ip}/${port}