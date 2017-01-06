#include "random_regex_node.hpp"

#include <random>

using rand_regex::random_regex_node_;

#include <iostream>

// TODO merge with range_random_regex_node (templated edition)
std::size_t random_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: random_regex_node_ " << current_index << '\n';
#endif

  generated_value_ = (char)random_gen.get_random((int)' ', (int)'~'); // TODO clean up the casts...
  os << generated_value_;

  return 1;
}

std::size_t random_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: random_regex_node_ " << current_index << '\n';
#endif

  os << generated_value_;

  return 1;
}
