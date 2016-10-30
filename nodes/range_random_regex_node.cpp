#include "range_random_regex_node.hpp"

#include <random>

using rand_regex::range_random_regex_node_;

range_random_regex_node_::range_random_regex_node_(char from, char to)
  : from_(from)
  , to_(to)
{
  //
}

void range_random_regex_node_::generate(std::ostream& os, random_generator_base& random_gen) const
{
  os << (char)random_gen.get_random((int)from_, (int)to_); // TODO clean up the casts...
}
