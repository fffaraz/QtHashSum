#!/bin/bash
set -euxo pipefail

mkdir -p /opt
git clone https://github.com/fffaraz/QtHashSum.git /opt/repo

cd /opt/repo/src
qmake
make -j$(nproc)

cd /opt
mv /opt/repo/src/QtHashSum /opt/QtHashSum
rm -rf /opt/repo
