#!/bin/bash

odir="build/"
libdir="lib/"
libname="liblorismessage.a"
ofiles="liblorismessage.a Message.o KeyValuePairContainer.o KeyValuePairContainerInterface.o"
ofiles="$ofiles KeyIntPair.o KeyFloatPair.o Key.o"

make
cd $odir
ar ru $libname $ofiles
ranlib $libname
cd ../
mv $odir/$libname $libdir/
