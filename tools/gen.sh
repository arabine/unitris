#!/bin/sh

rm *.out

for i in *.bmp
do
	/cygdrive/z/ENERIUM/bmp2c/bmp2c.exe ${i%%.*}.bmp ${i%%.*}.out -six_pixels
done 

# On concatène tous les fichiers de sortie

cat *.out > pictos.c
