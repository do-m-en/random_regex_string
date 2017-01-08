#include "literal_regex_node.hpp"

using rand_regex::literal_regex_node_g;

#include <iostream>
std::size_t literal_regex_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& /*random_gen*/)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: literal_regex_node_ " << current_index << '\n';
#endif

  os << std::get<literal_regex_node_d>(nodes[current_index]).literal_;

  return 1;
}

std::size_t literal_regex_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: literal_regex_node_ " << current_index << '\n';
#endif

  os << std::get<literal_regex_node_d>(nodes[current_index]).literal_;

  return 1;
}
