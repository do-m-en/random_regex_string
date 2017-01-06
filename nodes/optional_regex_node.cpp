#include "optional_regex_node.hpp"

using rand_regex::optional_regex_node_;

#include <iostream>
std::size_t optional_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: optional_regex_node_ " << current_index << '\n';
#endif

  if((random_value_ = random_gen.get_random(0, 1)))
  {
    return nodes[current_index + 1]->generate(nodes, current_index + 1, os, random_gen) + 1;
  }

  return 1; // TODO get node size
}

std::size_t optional_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: optional_regex_node_ " << current_index << '\n';
#endif

  if(random_value_)
  {
    return nodes[current_index + 1]->regenerate(nodes, current_index + 1, os) + 1;
  }

  return 1; // TODO get node size
}
