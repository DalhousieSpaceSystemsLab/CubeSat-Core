#ifndef __CLIENT_API_HPP__
#define __CLIENT_API_HPP__
#ifdef __cplusplus

// Standard C++ Libraries
#include <map>
#include <string>

// Standard C++ classes
using std::exception;
using std::pair;

// Client API (C++)
namespace dss
{
namespace ipc
{
// Methods
void        init(std::string name);
void        send(std::string dest, std::string msg);
void        send_map(const char *dest, std::map<std::string, std::string> *map);
void        send_map(const char *dest, std::map<std::string, int> *map);
void        send_map(const char *dest, std::map<int, int> *map);
std::string recv(std::string src);
void        close();
}; // namespace ipc
}; // namespace dss

#endif /* #ifdef __cplusplus */
#endif /* __CLIENT_API_HPP__ */
