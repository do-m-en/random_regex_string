#include "range_or_node.hpp"

using rand_regex::range_or_node_;

void range_or_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups)
{
  if(nodes_.size() == 1)
    random_value_ = 0;
  else
    random_value_ = random_gen.get_random(0, nodes_.size() - 1);

  std::visit(
    [&os, &random_gen, &groups](auto&& node)
    {
      return node.generate(os, random_gen, groups);
    },
    nodes_[random_value_]
  );
}

void range_or_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const
{
  std::visit(
    [&os, &groups](auto&& node)
    {
      return node.regenerate(os, groups);
    },
    nodes_[random_value_]
  );
}
