#include "repeat_zero_regex_node.hpp"

#include <random>

using rand_regex::repeat_zero_regex_node_;

repeat_zero_regex_node_::repeat_zero_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void repeat_zero_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0, 10); // TODO 10 should be a parameter

  for(int i = distribution(generator); i > 0; --i)
  {
    node_->generate(os);
  }
}
