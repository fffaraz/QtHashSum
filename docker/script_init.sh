#!/bin/bash
set -euxo pipefail

mkdir -p /opt
git clone https://github.com/fffaraz/QtHashSum.git /opt/QtHashSum

cd /opt/QtHashSum/src
qmake
make -j$(nproc)
