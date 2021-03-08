/**
 * @file server_engine_native.cpp
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Example implementation of server engine for native machine
 * @version 0.1
 * @date 2021-01-04
 *
 * @copyright Copyright (c) 2021 Carl Mattatall
 */

#include <iostream>
#include <string>

#include "server_engine.hpp"

class emulated_server_engine : server_engine
{
  public:
    int  transmit(const char *bytes, unsigned int blen);
    int  receive_callback(void);
    bool receive_complete(void);
    bool transmit_complete(void);
    bool is_busy(void);
};


int emulated_server_engine::transmit(const char *bytes, unsigned int blen)
{
    // okay so admittedly there is a far better implementation but this
    // is just an example...
    std::string msg = "";
    for (unsigned int i = 0; i < blen; i++)
    {
        msg += bytes[i];
    }
    std::cout << msg << std::endl;
}


int emulated_server_engine::receive_callback(void)
{
    /* Do nothing, OS will manage concurrency in read/write for us */
    return 0;
}


bool emulated_server_engine::receive_complete(void)
{
    return true;
}


bool emulated_server_engine::transmit_complete(void)
{
    return true;
}


bool emulated_server_engine::is_busy(void)
{
    return this->receive_complete() && this->transmit_complete();
}
