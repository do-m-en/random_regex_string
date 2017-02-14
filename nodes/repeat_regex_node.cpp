#include "repeat_regex_node.hpp"

using rand_regex::repeat_regex_node_;

void repeat_regex_node_::generate(std::ostream& os, random_generator_base& random_gen)
{
  for(int i=0; i<repeat_; ++i)
  {
    node_.generate(os, random_gen);
  }
}

void repeat_regex_node_::regenerate(std::ostream& os) const
{
  for(int i=0; i<repeat_; ++i)
  {
    node_.regenerate(os);
  }
}
