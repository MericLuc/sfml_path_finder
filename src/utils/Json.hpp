/**
 * @file Json.hpp
 * @brief JSON related utilities
 * @author lhm
 */

#ifndef SRC_UTILS_JSON_HPP
#define SRC_UTILS_JSON_HPP

// Standard headers
#include <map>
#include <string>

namespace JSON {
class Object;
}

/*****************************************************************************/
bool
json_test_struct(const JSON::Object& obj, const std::map<std::string, char>& test);

#endif // SRC_UTILS_JSON_HPP
