#!/bin/bash
for name in $(sort < Names.txt)
do
	echo $name
	if [ $name = "ScribeEzra" ]
	then
		echo "(That's Me)"
	fi
done
