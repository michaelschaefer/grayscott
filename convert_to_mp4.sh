#! /bin/bash

avconv -r 30 -i "./temp/temp-%03d.png" -c:v libx264 -crf 23 -pix_fmt yuv420p $1.mp4
