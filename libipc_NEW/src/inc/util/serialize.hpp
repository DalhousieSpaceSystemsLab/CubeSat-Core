/*
* serialize.hpp
*
*   purpose:  serialize a map into a string for simple sending through the IPC, and the reverse
*   author:   Yann Guerin
*
*/

#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

namespace dss
{
  namespace ipc 
  {
    namespace util
    {
      // Convert map into serialize string
      string serialize(map<string,string> map);
      string serialize(map<string,int> map);
      string serialize(map<int,string> map);
      string serialize(map<int,int> map);
      map<string,string> deserialize(string message, int mapSize);
    }
  }
}