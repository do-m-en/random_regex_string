#include "repeat_range_regex_node.hpp"

#include <random>

using rand_regex::repeat_range_regex_node_;

repeat_range_regex_node_::repeat_range_regex_node_(std::size_t min, std::size_t max)
  : min_(min)
  , max_(max)
{
  //
}

#include <iostream>
std::size_t repeat_range_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: repeat_range_regex_node_ " << current_index << '\n';
#endif

  random_value_ = random_gen.get_random(min_, max_);
  std::size_t len = 0;

  for(int i = 0; i < random_value_; ++i)
  {
    len = nodes[current_index + 1]->generate(nodes, current_index + 1, os, random_gen);
  }

  return len + 1;
}

std::size_t repeat_range_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: repeat_range_regex_node_ " << current_index << '\n';
#endif

  std::size_t len = 0;

  for(int i = 0; i < random_value_; ++i)
  {
    len = nodes[current_index + 1]->regenerate(nodes, current_index + 1, os);
  }

  return len + 1;
}
