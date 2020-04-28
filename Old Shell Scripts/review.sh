#!/bin/bash

dirs="core/ emulation/ groundstation/"

#cppcheck
for dir in $dirs
do
	cppcheck $dir
done

#Clang

