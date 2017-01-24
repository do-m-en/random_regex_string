#include "range_random_regex_node.hpp"

#include <random>

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::range_random_regex_node_;

range_random_regex_node_::range_random_regex_node_(char from, char to)
  : from_(from)
  , to_(to)
{
  //
}

void range_random_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: range_random_regex_node_\n";
#endif

  generated_value_ = (char)random_gen.get_random((int)from_, (int)to_); // TODO clean up the casts...
  os << generated_value_;
}

void range_random_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: range_random_regex_node_\n";
#endif

  os << generated_value_;
}
