//
// Created by Spencer Axford on 2019-05-07.
//

#include "UDClientExample.h"

UDClientExample::UDClientExample(char *sun_path)
        : UnixDomainStreamSocketClient(sun_path) {}

