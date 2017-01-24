#include "repeat_regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::repeat_regex_node_;

repeat_regex_node_::repeat_regex_node_(regex_node_* node, std::size_t repeat)
  : node_(node)
  , repeat_(repeat)
{
  //
}

void repeat_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: repeat_regex_node_\n";
#endif

  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->generate(nodes, os, random_gen);
  }
}

void repeat_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: repeat_regex_node_\n";
#endif

  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->regenerate(nodes, os);
  }
}
