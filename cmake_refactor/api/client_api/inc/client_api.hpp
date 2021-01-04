#ifndef __CLIENT_API_HPP__
#define __CLIENT_API_HPP__
#ifdef __cplusplus

// Standard C++ Libraries
#include <map>
#include <iterator>
#include <string>
#include <exception>

// Standard C++ classes
using std::exception;
using std::map;
using std::pair;
using std::string;

// Client API (C++)
namespace dss
{
namespace ipc
{
// Methods
void   init(string name);
void   send(string dest, string msg);
void   send_map(char dest[3], map<string, string> *map);
void   send_map(char dest[3], map<string, int> *map);
void   send_map(char dest[3], map<int, int> *map);
string recv(string src);
void   close();
}; // namespace ipc
}; // namespace dss

#endif /* #ifdef __cplusplus */
#endif /* __CLIENT_API_HPP__ */
