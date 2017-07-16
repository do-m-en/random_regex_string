#ifndef ASCII_NODES_HPP_INCLUDED
#define ASCII_NODES_HPP_INCLUDED

#include "regex_node.hpp"

namespace rand_regex {

// !('\t' | '\n' | '\v' | '\f' | '\r' | ' ')
inline regex_node_* any_non_whitespace_node()
{
  constexpr char ascii_min = 0;
  constexpr char ascii_max = 127;

  /* whitespaces:
     '\t' // tab: 9
     '\n' // newline: 10
     '\v' // vertical tab: 11
     '\f' // formfeed: 12
     '\r' // carriage return: 13
     ' ' // space: 32 */
  return new or_regex_node_{new range_random_regex_node_{ascii_min, '\t' - 1},
                            new range_random_regex_node_{'\r' + 1, ' ' - 1},
                            new range_random_regex_node_{' ' + 1, ascii_max}};
}

// A-Z | a-z | 0-9 | _
inline regex_node_* any_alphanum_or_underscore_node()
{
  return new or_regex_node_{new range_random_regex_node_{'A', 'Z'},
                            new range_random_regex_node_{'a', 'z'},
                            new range_random_regex_node_{'0', '9'},
                            new literal_regex_node_{'_'}};
}

// ! 0-9
inline regex_node_* any_non_digit_node()
{
  constexpr char ascii_min = 0;
  constexpr char ascii_max = 127;

  return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                            new range_random_regex_node_{'9' + 1, ascii_max}};
}

// !(0-9 | _)
inline regex_node_* any_not_alphanum_or_underscore_node()
{
  constexpr char ascii_min = 0;
  constexpr char ascii_max = 127;

  return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                            new range_random_regex_node_{'9' + 1, 'A' - 1},
                            new range_random_regex_node_{'Z' + 1, '_' - 1},
                            new range_random_regex_node_{'_' + 1, 'a' - 1},
                            new range_random_regex_node_{'z' + 1, ascii_max}};
}

}

#endif // ASCII_NODES_HPP_INCLUDED
