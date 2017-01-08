#include "range_random_regex_node.hpp"

using rand_regex::range_random_regex_node_g;

#include <iostream>
std::size_t range_random_regex_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: range_random_regex_node_ " << current_index << '\n';
#endif

  std::get<range_random_regex_node_d>(nodes[current_index]).generated_value_ = (char)random_gen.get_random((int)std::get<range_random_regex_node_d>(nodes[current_index]).from_, (int)std::get<range_random_regex_node_d>(nodes[current_index]).to_); // TODO clean up the casts...
  os << std::get<range_random_regex_node_d>(nodes[current_index]).generated_value_;

  return 1;
}

std::size_t range_random_regex_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: range_random_regex_node_ " << current_index << '\n';
#endif

  os << std::get<range_random_regex_node_d>(nodes[current_index]).generated_value_;

  return 1;
}
