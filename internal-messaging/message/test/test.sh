#!/bin/bash

test_file="message_test.cc"
libdir="../../../lib"
libname="liblorismessage.a"


g++ -Wpedantic $test_file -L $libdir -lliblorismessage -z text
if [ $? -eq 0 ]
then
./main
fi
