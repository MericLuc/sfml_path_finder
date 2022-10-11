/**
 * @file Json.cpp
 * @brief Implementation of \a Json.hpp
 * @author lhm
 */

// Standard headers

// Project headers
#include "Json.hpp"

// External headers
#include <JSON.hpp>

/*****************************************************************************/
bool
json_test_struct(const JSON::Object& obj, const std::map<std::string, char>& test)
{
    for (const auto& [field, type] : test) {
        if (!obj[field])
            return false;

        switch (type) {
            case 's':
                if (!obj[field].isString())
                    return false;
                break;
            case 'o':
                if (!obj[field].isObject())
                    return false;
                break;
            case 'a':
                if (!obj[field].isArray())
                    return false;
                break;
            case 'i':
                if (!obj[field].isInt())
                    return false;
                break;
            default:
                return true;
        }
    }

    return true;
}
