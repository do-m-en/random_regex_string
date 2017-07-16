#include "optional_regex_node.hpp"

using rand_regex::optional_regex_node_;

optional_regex_node_::optional_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void optional_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups)
{
  if((random_value_ = random_gen.get_random(0, 1)))
  {
    node_->generate(os, random_gen, groups);
  }
}

void optional_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const
{
  if(random_value_)
  {
    node_->regenerate(os, groups);
  }
}
