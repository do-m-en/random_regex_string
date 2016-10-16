#include "repeat_min_regex_node.hpp"

#include <random>

using rand_regex::repeat_min_regex_node_;

repeat_min_regex_node_::repeat_min_regex_node_(regex_node_* node, std::size_t min)
  : node_(node)
  , min_(min)
{
  //
}

void repeat_min_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(min_, 10); // TODO 10 should be a parameter

  for(int i = distribution(generator); i > 0; --i)
  {
    node_->generate(os);
  }
}
