#include "repeat_range_regex_node.hpp"

#include <random>

#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
#endif

using rand_regex::repeat_range_regex_node_;

repeat_range_regex_node_::repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max)
  : node_(node)
  , min_(min)
  , max_(max)
{
  //
}

void repeat_range_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: repeat_range_regex_node_\n";
#endif

  random_value_ = random_gen.get_random(min_, max_);

  for(int i = 0; i < random_value_; ++i)
  {
    node_->generate(nodes, os, random_gen);
  }
}

void repeat_range_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: repeat_range_regex_node_\n";
#endif

  for(int i = 0; i < random_value_; ++i)
  {
    node_->regenerate(nodes, os);
  }
}
