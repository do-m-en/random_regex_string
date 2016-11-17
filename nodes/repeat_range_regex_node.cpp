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

void repeat_range_regex_node_::generate(std::ostream& os, random_generator_base& random_gen)
{
  random_value_ = random_gen.get_random(min_, max_);

  for(int i = 0; i < random_value_; ++i)
  {
    node_->generate(os, random_gen);
  }
}

void repeat_range_regex_node_::regenerate(std::ostream& os) const
{
  for(int i = 0; i < random_value_; ++i)
  {
    node_->regenerate(os);
  }
}
