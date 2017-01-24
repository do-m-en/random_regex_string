#include "whitespace_regex_node.hpp"

#include <array>

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

namespace {
  constexpr std::array<char, 5> whitespaces{'\r', '\n', '\t', '\f', ' '};
}

using rand_regex::whitespace_regex_node_;

void whitespace_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: whitespace_regex_node_\n";
#endif

  random_value_ = random_gen.get_random(0, whitespaces.size() - 1);
  os << whitespaces[random_value_];
}

void whitespace_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: whitespace_regex_node_\n";
#endif

  os << whitespaces[random_value_];
}
