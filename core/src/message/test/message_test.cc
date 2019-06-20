#include <iostream>
using std::cout;
using std::endl;

#include "../header/Message.h"

int main(){
cout << "HEllo! This is a message creation test!" << endl;
Message message(0,0);
char str[128];
message.ToString(str,128);
for(int i=0;i<128;i++){
  cout << str[i];
}
return 0;
}