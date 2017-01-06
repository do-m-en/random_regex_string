#include "whitespace_regex_node.hpp"

#include <array>

namespace {
  constexpr std::array<char, 5> whitespaces{'\r', '\n', '\t', '\f', ' '};
}

using rand_regex::whitespace_regex_node_;

#include <iostream>
std::size_t whitespace_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: whitespace_regex_node_ " << current_index << '\n';
#endif

  random_value_ = random_gen.get_random(0, whitespaces.size() - 1);
  os << whitespaces[random_value_];

  return 1;
}

std::size_t whitespace_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: whitespace_regex_node_ " << current_index << '\n';
#endif

  os << whitespaces[random_value_];

  return 1;
}
