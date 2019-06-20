#!/bin/bash

test_file="message_test.cc"
libdir="../../../lib/"
libname="liblorismessage.a"


g++ -o main $test_file $libdir$libname

./main