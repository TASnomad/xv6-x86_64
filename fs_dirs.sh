#!/bin/sh

for x in $@
do
	out/opfs fs.img mkdir $x
done;