#!/bin/bash

# Run the build script
./build.sh x86

# Export contents to EXPORT_CLIENT
mkdir EXPORT_CLIENT
cp bin/x86/libCLIENT_API.a EXPORT_CLIENT
cp bin/x86/libLIB_IPC.a EXPORT_CLIENT
mkdir EXPORT_CLIENT/include
cp src/api/client_api/inc/client_api.h EXPORT_CLIENT/include
cp -r src/api/libipc/inc/* EXPORT_CLIENT/include
echo ""
echo ""
echo "Done!"
echo ""
echo ""
echo "You can find the libraries in the EXPORT_CLIENT directory and the required headers in the EXPORT_CLIENT/include directory."
