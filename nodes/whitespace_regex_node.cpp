#include "whitespace_regex_node.hpp"

#include <array>

namespace {
  constexpr std::array<char, 5> whitespaces{'\r', '\n', '\t', '\f', ' '};
}

using rand_regex::whitespace_regex_node_;

void whitespace_regex_node_::generate(std::ostream& os, random_generator_base& random_gen)
{
  random_value_ = random_gen.get_random(0, whitespaces.size() - 1);
  os << whitespaces[random_value_];
}

void whitespace_regex_node_::regenerate(std::ostream& os) const
{
  os << whitespaces[random_value_];
}
