#include "literal_regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

void literal_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& /*random_gen*/)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: literal_regex_node_\n";
#endif

  os << literal_;
}


void literal_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: literal_regex_node_\n";
#endif

  os << literal_;
}
