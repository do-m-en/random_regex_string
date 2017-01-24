#include "random_regex_node.hpp"

#include <random>

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::random_regex_node_;

// TODO merge with range_random_regex_node (templated edition)
void random_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: random_regex_node_\n";
#endif

  generated_value_ = (char)random_gen.get_random((int)' ', (int)'~'); // TODO clean up the casts...
  os << generated_value_;
}

void random_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: random_regex_node_\n";
#endif

  os << generated_value_;
}
