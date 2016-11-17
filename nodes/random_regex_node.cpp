#include "random_regex_node.hpp"

#include <random>

using rand_regex::random_regex_node_;

// TODO merge with range_random_regex_node (templated edition)
void random_regex_node_::generate(std::ostream& os, random_generator_base& random_gen)
{
  generated_value_ = (char)random_gen.get_random((int)' ', (int)'~'); // TODO clean up the casts...
  os << generated_value_;
}

void random_regex_node_::regenerate(std::ostream& os) const
{
  os << generated_value_;
}
