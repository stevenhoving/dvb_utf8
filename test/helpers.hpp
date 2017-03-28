#ifndef helpers_h__
#define helpers_h__

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <initializer_list>

static
std::string to_utf8_string(const std::initializer_list<uint8_t> &bytes)
{
    std::string result;
    result.reserve(bytes.size());
    for (auto letter : bytes)
        result.push_back(letter);

    return result;
}

#endif // helpers_h__
