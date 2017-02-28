#include "repeat_regex_node.hpp"

using rand_regex::repeat_regex_node_;

repeat_regex_node_::repeat_regex_node_(regex_node_* node, std::size_t repeat)
  : node_(node)
  , repeat_(repeat)
{
  //
}

void repeat_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, char& last_gen)
{
  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->generate(os, random_gen, last_gen);
  }
}

void repeat_regex_node_::regenerate(std::ostream& os, char& last_gen) const
{
  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->regenerate(os, last_gen);
  }
}
