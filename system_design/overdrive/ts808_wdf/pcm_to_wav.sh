#!/bin/sh
ffmpeg -y -f s16le -ac 1 -ar 44100 -i "$1" -acodec pcm_s16le "$2"
