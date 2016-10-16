#include "repeat_range_regex_node.hpp"

#include <random>

using rand_regex::repeat_range_regex_node_;

repeat_range_regex_node_::repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max)
  : node_(node)
  , min_(min)
  , max_(max)
{
  //
}

void repeat_range_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(min_, max_);

  for(int i = distribution(generator); i > 0; --i)
  {
    node_->generate(os);
  }
}
