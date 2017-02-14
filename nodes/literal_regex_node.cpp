#include "literal_regex_node.hpp"

using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

void literal_regex_node_::generate(std::ostream& os, random_generator_base& /*random_gen*/, std::vector<std::tuple<int, inner_group_node_*>>& /*groups*/)
{
  os << literal_;
}


void literal_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& /*groups*/) const
{
  os << literal_;
}
