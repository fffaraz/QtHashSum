#!/bin/bash
set -euxo pipefail

cd /opt
exec /opt/QtHashSum/src/QtHashSum "$@"
