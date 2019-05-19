//
// Created by Spencer Axford on 2019-05-16.
//

#include "../cc/MessageSenderInterface.cc"
int main(int argc, char *argv[])
{
    MessageSenderInterface ms(3025893);
    ms.SendMessage("Hello");
    return 0;
}