#include "literal_regex_node.hpp"

using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

#include <iostream>
std::size_t literal_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& /*random_gen*/)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: literal_regex_node_ " << current_index << '\n';
#endif

  os << literal_;

  return 1;
}


std::size_t literal_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: literal_regex_node_ " << current_index << '\n';
#endif

  os << literal_;

  return 1;
}
