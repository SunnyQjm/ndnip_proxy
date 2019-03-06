#!/usr/bin/env bash
ip=$1
port=$2
sudo nlsrc advertise /NDN-IP/FILE_SLICE/${ip}/${port}
sudo nlsrc advertise /NDN-IP/FILE_INFO/${ip}/${port}
