#ifndef __SERIALIZE_HPP__
#define __SERIALIZE_HPP__
#ifdef __cplusplus

/*
 * serialize.hpp
 *
 *   purpose:  serialize a map into a string for simple sending through the IPC,
 * and the reverse author:   Yann Guerin
 *
 */

#include <map>
#include <string>

namespace dss
{
namespace ipc
{
namespace util
{
// Convert map into serialize string
std::string serialize(std::map<std::string, std::string> map);
std::string serialize(std::map<std::string, int> map);
std::string serialize(std::map<int, std::string> map);
std::string serialize(std::map<int, int> map);
std::map<std::string, std::string> deserialize(std::string msg, int mapSize);
} // namespace util
} // namespace ipc
} // namespace dss

#endif /* #ifdef __cplusplus */
#endif /* __SERIALIZE_HPP__ */
