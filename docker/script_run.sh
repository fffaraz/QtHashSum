#!/bin/bash
set -euxo pipefail

cd /opt
[ -d /data ] && cd /data
exec /opt/QtHashSum/src/QtHashSum "$@"
