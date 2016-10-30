#include "optional_regex_node.hpp"

using rand_regex::optional_regex_node_;

optional_regex_node_::optional_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void optional_regex_node_::generate(std::ostream& os, random_generator_base& random_gen) const
{
  if(random_gen.get_random(0, 1))
  {
    node_->generate(os, random_gen);
  }
}
