#include "group_regex_node.hpp"

using rand_regex::group_regex_node_;

void group_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups)
{
  for(auto& node : nodes_)
  {
    std::visit(
      [&os, &random_gen, &groups](auto&& node)
      {
        return node.generate(os, random_gen, groups);
      },
      node
    );
  }
}

void group_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const
{
  for(const auto& node : nodes_)
  {
    std::visit(
      [&os, &groups](auto&& node)
      {
        return node.regenerate(os, groups);
      },
      node
    );
  }
}
