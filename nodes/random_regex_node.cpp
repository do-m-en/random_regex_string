#include "random_regex_node.hpp"

#include <random>

using rand_regex::random_regex_node_;

void random_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<char> distribution(' ', '~');

  os << distribution(generator);
}
