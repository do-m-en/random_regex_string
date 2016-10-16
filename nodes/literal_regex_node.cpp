#include "literal_regex_node.hpp"

using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

void literal_regex_node_::generate(std::ostream& os) const
{
  os << literal_;
}
