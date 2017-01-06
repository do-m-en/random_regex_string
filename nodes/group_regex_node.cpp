#include "group_regex_node.hpp"

using rand_regex::group_regex_node_;

#include <iostream>
std::size_t group_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: group_regex_node_ " << current_index << '\n';
#endif

  std::size_t i = current_index + 1;
  for(int x = 0; x < end_; ++x)
  {
    i += nodes[i]->generate(nodes, i, os, random_gen);
  }

  return i;
}

std::size_t group_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: group_regex_node_ " << current_index << '\n';
#endif

  std::size_t i = current_index + 1;
  for(int x = 0; x < end_; ++x)
    i += nodes[i]->regenerate(nodes, i, os);

  return i;
}
