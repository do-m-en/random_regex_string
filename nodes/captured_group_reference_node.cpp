#include "captured_group_reference_node.hpp"

#include <iostream>

using rand_regex::captured_group_reference_node_;

captured_group_reference_node_::captured_group_reference_node_(regex_node_* referred_node)
  : referred_node_(referred_node)
{
  //
}

void captured_group_reference_node_::generate(std::ostream& os, random_generator_base& random_gen, char& last_gen)
{
  referred_node_->regenerate(os, last_gen);
}

void captured_group_reference_node_::regenerate(std::ostream& os, char& last_gen) const
{
  referred_node_->regenerate(os, last_gen);
}
