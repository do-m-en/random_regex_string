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

void repeat_range_regex_node_::generate(std::ostream& os, random_generator_base& random_gen) const
{
  for(int i = random_gen.get_random(min_, max_); i > 0; --i)
  {
    node_->generate(os, random_gen);
  }
}
