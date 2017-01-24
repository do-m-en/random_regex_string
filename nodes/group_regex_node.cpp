#include "group_regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::group_regex_node_;

group_regex_node_::group_regex_node_(std::size_t start, std::size_t count)
  : start_{start}
  , count_{count}
{
  //
}

void group_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: group_regex_node_\n";
#endif

  for(std::size_t i=0; i<count_; ++i)
    nodes[start_ + i]->generate(nodes, os, random_gen);
}

void group_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: group_regex_node_\n";
#endif

  for(std::size_t i=0; i<count_; ++i)
    nodes[start_ + i]->regenerate(nodes, os);
}
