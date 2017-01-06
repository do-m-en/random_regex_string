#include "captured_group_reference_node.hpp"

#include <iostream>

using rand_regex::captured_group_reference_node_;

captured_group_reference_node_::captured_group_reference_node_(std::size_t referred_node)
  : referred_node_(referred_node)
{
  //
}

std::size_t captured_group_reference_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: captured_group_reference_node_ " << current_index << '\n';
#endif

  return nodes[referred_node_]->regenerate(nodes, referred_node_, os) + 1;
}

std::size_t captured_group_reference_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: captured_group_reference_node_ " << current_index << '\n';
#endif

  return nodes[referred_node_]->regenerate(nodes, referred_node_, os) + 1;
}
