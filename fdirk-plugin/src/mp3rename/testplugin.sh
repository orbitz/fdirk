#!/bin/sh
export LD_LIBRARY_PATH="../../../src/libfdirk/"
../../../src/fdirktest -d=./ -p=mp3rename random.mp3 -N="Modest Mouse" -T=2 -A="Moons And Antartica" -S="3rd Planet" -G=22 -Y=1996 -C="comment jfief"
