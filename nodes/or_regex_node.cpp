#include "or_regex_node.hpp"

using rand_regex::or_regex_node_;

void or_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups)
{
  if(!nodes_.size())
    return;
  else if(nodes_.size() == 1)
    random_value_ = 0;
  else
    random_value_ = random_gen.get_random(0, nodes_.size() - 1);

  nodes_[random_value_].generate(os, random_gen, groups);
}

void or_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const
{
  if(nodes_.size())
    nodes_[random_value_].regenerate(os, groups);
}
