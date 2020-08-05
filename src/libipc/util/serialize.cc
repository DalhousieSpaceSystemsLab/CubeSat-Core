/*
* serialize.cc
*
*   purpose:  serialize a map into a string for simple sending through the IPC, and the reverse
*   author:   Yann Guerin
*
*/

#include "ipc/util/serialize.hpp"

using namespace std;

string serialize(map<string,string> map);
string serialize(std::map<string, int> map);
string serialize(std::map<int, string> map);
string serialize(std::map<int, int> map); 
map<string, string> deSerialize(std::string message, int mapSize);


int main()
{
  // Create map placeholder
  map<string, int> coordinates;

//Below is testing data for functions
/*
  // Insert key-value pairs
  coordinates.insert(pair<string, string>("initialize",   "00"));
  coordinates.insert(pair<string, string>("shutdown",     "01"));
  coordinates.insert(pair<string, string>("take picture", "02"));
  coordinates.insert(pair<string, string>("settings",     "03"));
  coordinates.insert(pair<string, string>("format",       "10"));
  coordinates.insert(pair<string, string>("filename",     "11"));
  coordinates.insert(pair<string, string>("quality",      "12"));
  coordinates.insert(pair<string, string>("filepath",     "13"));
*/
  // Insert key-value pairs
  coordinates.insert(pair<string, int>("initialize",   00));
  coordinates.insert(pair<string, int>("shutdown",     01));
  coordinates.insert(pair<string, int>("take picture", 02));
  coordinates.insert(pair<string, int>("settings",     03));
  coordinates.insert(pair<string, int>("format",       10));
  coordinates.insert(pair<string, int>("filename",     11));
  coordinates.insert(pair<string, int>("quality",      12));
  coordinates.insert(pair<string, int>("filepath",     13));
//
//Testing turning above maps into string and then back to map form
  string command = serialize(coordinates);
  cout << "The map in string form is: \n" << command << "\n";
  map<string,string> backto = deSerialize(command, 8);
  cout << "And back into a map but iterated through is: \n";
  map<string, string>::iterator iter;
  for(iter = backto.begin(); iter != backto.end(); ++iter)
  {
    cout << iter->first << " , " << iter->second << endl;
  }

  // done
  return 0;
}

string serialize(std::map<string, string> map)
{
  // Create iterator
  std::map<string, string>::iterator iter;
  string message = "";
  // Iterate through key-value pairs
  for(iter = map.begin(); iter != map.end(); ++iter)
  {
    message = message + iter->first + "::" + iter->second + "::";
  }
  return message;
}

string serialize(std::map<string, int> map)
{
  // Create iterator
  std::map<string, int>::iterator iter;
  std::string message = "";
  // Iterate through key-value pairs
  for(iter = map.begin(); iter != map.end(); ++iter)
  {
    std::string cast = std::to_string(iter->second);
    message = message + iter->first + "::" + cast + "::";
  }
  return message;
}


string serialize(std::map<int, string> map)
{
  // Create iterator
  std::map<int, string>::iterator iter;
  string message = "";
  // Iterate through key-value pairs
  for(iter = map.begin(); iter != map.end(); ++iter)
  {
    string cast = std::to_string(iter->first);
    message = message + cast + "::" + iter->second + "::";
  }
  return message;
}

string serialize(std::map<int, int> map)
{
  // Create iterator
  std::map<int, int>::iterator iter;
  string message = "";
  // Iterate through key-value pairs
  for(iter = map.begin(); iter != map.end(); ++iter)
  {
    string cast = std::to_string(iter->first);
    string cast2 = std::to_string(iter->second);
    message = message + cast + "::" + cast2 + "::";
  }
  return message;
}


map<string, string> deSerialize(std::string message, int mapSize)
{
  //create placeholders for map to be returned and vector
  map<string, string> themap;
  std::vector<string> word_list;
  //initialize the size of the word list and counter
  int theSize = 2 * mapSize;
  int j = 0;
  //loop through each word in the string list
  for(int x = 0; x < theSize; x++)
  {
    string word;
    //loop through each character of the word
    for(int i = j; i<(message.length() -1); i++)
    {
      //check if the following two characters are "::" suggesting the end of a word
      string check, check1, check2;
      check1 = message.at(i);
      check2 = message.at(i+1);
      check = check1 + check2;
      if (check.compare("::") == 0) //add word to word_list
      {
        word_list.push_back(word);
        j = i + 2;
        break;
      } else { //add character to the word
        word = word + message.at(i);
        continue;
      }
    }
  }
  //iterate through vector list
  std::vector<string>::iterator iter;
  for(iter = word_list.begin(); iter != word_list.end(); std::advance(iter, 2))
  {
    //add each pair of words in vector word list to themap
    string toadd1 = *iter;
    string toadd2 = *(iter+1);
    themap.insert(pair<string, string>(toadd1, toadd2));
  }
  //return themap
  return themap;
}
