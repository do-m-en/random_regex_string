#ifndef ASCII_NODES_HPP_INCLUDED
#define ASCII_NODES_HPP_INCLUDED

#include "regex_node.hpp"

struct regex_param
{
  regex_param(std::string_view regex_) : regex{regex_} {}

  std::string_view regex;
  std::size_t consumed = 0;
  std::vector<bool> captured_groups;
};

namespace rand_regex {

constexpr char ascii_min = 0;
constexpr char ascii_max = 127;

// !('\t' | '\n' | '\v' | '\f' | '\r' | ' ')
auto any_non_whitespace_gen = [](regex_param& param, const auto& context, auto& node)
{
  /* whitespaces:
     '\t' // tab: 9
     '\n' // newline: 10
     '\v' // vertical tab: 11
     '\f' // formfeed: 12
     '\r' // carriage return: 13
     ' ' // space: 32 */
  node = new or_regex_node_{new range_random_regex_node_{ascii_min, '\t' - 1},
                            new range_random_regex_node_{'\r' + 1, ' ' - 1},
                            new range_random_regex_node_{' ' + 1, ascii_max}};
  return true;
};

// A-Z | a-z | 0-9 | _
auto any_alphanum_or_underscore_gen = [](regex_param& param, const auto& context, auto& node)
{
  node = new or_regex_node_{new range_random_regex_node_{'A', 'Z'},
                            new range_random_regex_node_{'a', 'z'},
                            new range_random_regex_node_{'0', '9'},
                            new literal_regex_node_{'_'}};
  return true;
};

// ! 0-9
auto any_non_digit_gen = [](regex_param& param, const auto& context, auto& node)
{
  node = new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                            new range_random_regex_node_{'9' + 1, ascii_max}};
  return true;
};

// !(0-9 | _)
auto any_not_alphanum_or_underscore_gen = [](regex_param& param, const auto& context, auto& node)
{
  node = new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                            new range_random_regex_node_{'9' + 1, 'A' - 1},
                            new range_random_regex_node_{'Z' + 1, '_' - 1},
                            new range_random_regex_node_{'_' + 1, 'a' - 1},
                            new range_random_regex_node_{'z' + 1, ascii_max}};
  return true;
};

// .
auto dot_gen = [](regex_param& param, const auto& context, auto& node)
{
  node = new or_regex_node_{new range_random_regex_node_{ascii_min, 9}, // \n - 1
                            new range_random_regex_node_{11, 12}, // \n + 1, \r - 1
                            new range_random_regex_node_{14, ascii_max}};
  return true;
};

}

#endif // ASCII_NODES_HPP_INCLUDED
