#include "repeat_regex_node.hpp"

using rand_regex::repeat_regex_node_;

repeat_regex_node_::repeat_regex_node_(regex_node_* node, std::size_t repeat)
  : node_(node)
  , repeat_(repeat)
{
  //
}

void repeat_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups)
{
  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->generate(os, random_gen, groups);
  }
}

void repeat_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const
{
  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->regenerate(os, groups);
  }
}
