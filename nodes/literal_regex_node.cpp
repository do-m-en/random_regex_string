#include "literal_regex_node.hpp"

using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

void literal_regex_node_::generate(std::ostream& os, random_generator_base& /*random_gen*/, char& last_gen)
{
  last_gen = literal_;
  os << literal_;
}


void literal_regex_node_::regenerate(std::ostream& os, char& last_gen) const
{
  last_gen = literal_;
  os << literal_;
}
