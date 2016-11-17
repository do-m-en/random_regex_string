#include "group_regex_node.hpp"

using rand_regex::group_regex_node_;

group_regex_node_::group_regex_node_(std::vector<regex_node_*>&& grouped_nodes)
  : grouped_nodes_(grouped_nodes)
{
  //
}

void group_regex_node_::generate(std::ostream& os, random_generator_base& random_gen)
{
  for(const auto node : grouped_nodes_)
    node->generate(os, random_gen);
}

void group_regex_node_::regenerate(std::ostream& os) const
{
  for(const auto node : grouped_nodes_)
    node->regenerate(os);
}
