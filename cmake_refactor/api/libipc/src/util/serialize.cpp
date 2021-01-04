/*
 * serialize.cc
 *
 *   purpose:  serialize a map into a string for simple sending through the IPC,
 * and the reverse author:   Yann Guerin
 *
 */
#include <iterator>
#include <vector>
#include <iostream>
#include <unistd.h>


#include "serialize.hpp"

//std::string serialize(std::map<std::string, std::string> map);
//std::string serialize(std::map<std::string, int> map);
//std::string serialize(std::map<int, std::string> map);
//std::string serialize(std::map<int, int> map);
//std::map<std::string, std::string> deSerialize(std::string msg, int mapSize);

using namespace dss::ipc::util;


int main()
{
    // Create map placeholder
    std::map<std::string, int> coordinates;

    // Below is testing data for functions
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
    coordinates.insert(std::pair<std::string, int>("initialize", 00));
    coordinates.insert(std::pair<std::string, int>("shutdown", 01));
    coordinates.insert(std::pair<std::string, int>("take picture", 02));
    coordinates.insert(std::pair<std::string, int>("settings", 03));
    coordinates.insert(std::pair<std::string, int>("format", 10));
    coordinates.insert(std::pair<std::string, int>("filename", 11));
    coordinates.insert(std::pair<std::string, int>("quality", 12));
    coordinates.insert(std::pair<std::string, int>("filepath", 13));
    //
    // Testing turning above maps into string and then back to map form
    std::string command = serialize(coordinates);
    std::cout << "The map in string form is: \n" << command << "\n";
    std::map<std::string, std::string> backto = deserialize(command, 8);
    std::cout << "And back into a map but iterated through is: \n";
    std::map<std::string, std::string>::iterator iter;
    for (iter = backto.begin(); iter != backto.end(); ++iter)
    {
        std::cout << iter->first << " , " << iter->second << std::endl;
    }

    // done
    return 0;
}

std::string serialize(std::map<std::string, std::string> map)
{
    // Create iterator
    std::map<std::string, std::string>::iterator iter;
    std::string                                  message = "";
    // Iterate through key-value pairs
    for (iter = map.begin(); iter != map.end(); ++iter)
    {
        message = message + iter->first + "::" + iter->second + "::";
    }
    return message;
}

std::string serialize(std::map<std::string, int> map)
{
    // Create iterator
    std::map<std::string, int>::iterator iter;
    std::string                          message = "";
    // Iterate through key-value pairs
    for (iter = map.begin(); iter != map.end(); ++iter)
    {
        std::string cast = std::to_string(iter->second);
        message          = message + iter->first + "::" + cast + "::";
    }
    return message;
}


std::string serialize(std::map<int, std::string> map)
{
    // Create iterator
    std::map<int, std::string>::iterator iter;
    std::string                          message = "";
    // Iterate through key-value pairs
    for (iter = map.begin(); iter != map.end(); ++iter)
    {
        std::string cast = std::to_string(iter->first);
        message          = message + cast + "::" + iter->second + "::";
    }
    return message;
}

std::string serialize(std::map<int, int> map)
{
    // Create iterator
    std::map<int, int>::iterator iter;
    std::string                  message = "";
    // Iterate through key-value pairs
    for (iter = map.begin(); iter != map.end(); ++iter)
    {
        std::string cast  = std::to_string(iter->first);
        std::string cast2 = std::to_string(iter->second);
        message           = message + cast + "::" + cast2 + "::";
    }
    return message;
}


std::map<std::string, std::string> deSerialize(std::string message, int mapSize)
{
    // create placeholders for map to be returned and vector
    std::map<std::string, std::string> themap;
    std::vector<std::string>           word_list;
    // initialize the size of the word list and counter
    int theSize = 2 * mapSize;
    int j       = 0;
    // loop through each word in the string list
    for (int x = 0; x < theSize; x++)
    {
        std::string word;
        // loop through each character of the word
        for (int i = j; i < (message.length() - 1); i++)
        {
            // check if the following two characters are "::" suggesting the end
            // of a word
            std::string check;
            std::string check1;
            std::string check2;
            check1 = message.at(i);
            check2 = message.at(i + 1);
            check  = check1 + check2;
            if (check.compare("::") == 0) // add word to word_list
            {
                word_list.push_back(word);
                j = i + 2;
                break;
            }
            else
            { // add character to the word
                word = word + message.at(i);
                continue;
            }
        }
    }
    // iterate through vector list
    std::vector<std::string>::iterator iter;
    for (iter = word_list.begin(); iter != word_list.end();
         std::advance(iter, 2))
    {
        // add each pair of words in vector word list to themap
        std::string toadd1 = *iter;
        std::string toadd2 = *(iter + 1);
        themap.insert(std::pair<std::string, std::string>(toadd1, toadd2));
    }
    // return themap
    return themap;
}
