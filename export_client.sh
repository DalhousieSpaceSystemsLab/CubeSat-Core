#!/bin/bash
cmake src -B build 
cmake --build build 
mkdir EXPORT_CLIENT
cp build/libCLIENT_API.a EXPORT_CLIENT 
cp build/libLIB_IPC.a EXPORT_CLIENT
mkdir EXPORT_CLIENT/include 
cp src/api/client_api/inc/client_api.h EXPORT_CLIENT/include 
cp -r src/api/libipc/inc/* EXPORT_CLIENT/include
echo ""
echo ""
echo "Done!"
echo ""
echo ""
echo "You can find the libraries in the EXPORT_CLIENT directory and the required headers in the EXPORT_CLIENT/include directory."