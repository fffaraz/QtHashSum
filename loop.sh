#!/bin/bash
set -ux

if [ $# -ne 2 ]; then
	echo "Usage: loop.sh WatchDir OutFile"
	exit 1
fi

WatchDir=$1
OutFile=$2

if [ ! -d "$WatchDir" ]; then
	echo "$WatchDir directory doesn't exist"
	exit 1
fi

WatchDir=$(readlink -f "$WatchDir") # realpath, cd "$WatchDir"; pwd

[ -f "$OutFile" ] && touch "$OutFile"

GitRepo=$(dirname "$OutFile")
GitRepo=$(readlink -f "$GitRepo")
IsGitRepo=0
git -C "$GitRepo" rev-parse --is-inside-work-tree > /dev/null 2>&1
[ $? -eq 0 ] && IsGitRepo=1

while :
do
	date
	docker run --rm -i -v "$WatchDir":/data fffaraz/qthashsum . > "$OutFile"
	date
	if [ $IsGitRepo -ne 0 ]; then
		if [ -n "$(git -C "$GitRepo" status --porcelain)" ]; then
			git -C "$GitRepo" add "$OutFile"
			git -C "$GitRepo" commit -m "QtHashSum $(date)"
		fi
	fi
	sleep 3600
done
