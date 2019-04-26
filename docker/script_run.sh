#!/bin/bash

cd /opt
[ -d /data ] && cd /data
exec /opt/QtHashSum "$@"
