#include "range_random_regex_node.hpp"

#include <random>

using rand_regex::range_random_regex_node_;

range_random_regex_node_::range_random_regex_node_(char from, char to)
  : from_(from)
  , to_(to)
{
  //
}

void range_random_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<char> distribution(from_, to_);

  os << distribution(generator);
}
