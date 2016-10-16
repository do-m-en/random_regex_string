#ifndef UNICODE_BLOCKS_HPP_INCLUDED
#define UNICODE_BLOCKS_HPP_INCLUDED

#include <string>

namespace unicode { // unicode 8.0
namespace blocks {

// TODO add seed as first parameter
char32_t get_random_char();
const char32_t* get_random_char(const std::string& block_name);
const char32_t* get_random_char(const std::string& begin, const std::string& end);
const char32_t* get_random_char(char32_t begin, char32_t end);

}} // namespace

#endif // UNICODE_BLOCKS_HPP_INCLUDED
