#include "range_random_regex_node.hpp"

#include <random>

using rand_regex::range_random_regex_node_;

range_random_regex_node_::range_random_regex_node_(char from, char to)
  : from_(from)
  , to_(to)
{
  //
}

#include <iostream>
std::size_t range_random_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: range_random_regex_node_ " << current_index << '\n';
#endif

  generated_value_ = (char)random_gen.get_random((int)from_, (int)to_); // TODO clean up the casts...
  os << generated_value_;

  return 1;
}

std::size_t range_random_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: range_random_regex_node_ " << current_index << '\n';
#endif

  os << generated_value_;

  return 1;
}
