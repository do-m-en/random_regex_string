#include "repeat_range_regex_node.hpp"

#include <random>

using rand_regex::repeat_range_regex_node_;

void repeat_range_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups)
{
  random_value_ = (min_ == max_ ? min_ : random_gen.get_random(min_, max_));

  for(int i = 0; i < random_value_; ++i)
  {
    std::visit(
      [&os, &random_gen, &groups](auto&& node)
      {
        return node.generate(os, random_gen, groups);
      },
      node_
    );
  }
}

void repeat_range_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const
{
  for(int i = 0; i < random_value_; ++i)
  {
    std::visit(
      [&os, &groups](auto&& node)
      {
        return node.regenerate(os, groups);
      },
      node_
    );
  }
}
