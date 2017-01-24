#include "optional_regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::optional_regex_node_;

optional_regex_node_::optional_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void optional_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: optional_regex_node_\n";
#endif

  if((random_value_ = random_gen.get_random(0, 1)))
  {
    node_->generate(nodes, os, random_gen);
  }
}

void optional_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: optional_regex_node_\n";
#endif

  if(random_value_)
  {
    node_->regenerate(nodes, os);
  }
}
