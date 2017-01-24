#include "captured_group_reference_node.hpp"

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::captured_group_reference_node_;

captured_group_reference_node_::captured_group_reference_node_(regex_node_* referred_node)
  : referred_node_(referred_node)
{
  //
}

void captured_group_reference_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: captured_group_reference_node_\n";
#endif

  referred_node_->regenerate(nodes, os);
}

void captured_group_reference_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: captured_group_reference_node_\n";
#endif

  referred_node_->regenerate(nodes, os);
}
