#include "or_regex_node.hpp"

//#ifdef RANDOM_REGEX_DEBUG
  #include <iostream>
//#endif

using rand_regex::or_regex_node_;

or_regex_node_::or_regex_node_(const std::vector<std::size_t>& nodes)
  : or_nodes_{nodes}
{
  //
}

or_regex_node_::or_regex_node_(std::initializer_list<std::size_t> node_indices)
  : or_nodes_{node_indices}
{
  //
}

void or_regex_node_::append(std::size_t item)
{
  or_nodes_.push_back(item);
}

void or_regex_node_::generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen)
{
  if(or_nodes_.size() == 1)
  {
    random_value_ = 0;
  }
  else
  {
    random_value_ = random_gen.get_random(0, or_nodes_.size() - 1);
  }

#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: or_regex_node_ rand: " << random_value_ << "; pos: " << or_nodes_[random_value_] << '\n';
#endif

  nodes[or_nodes_[random_value_]]->generate(nodes, os, random_gen);
}

void or_regex_node_::regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: or_regex_node_ rand: " << random_value_ << "; pos: " << or_nodes_[random_value_] << '\n';
#endif

  nodes[or_nodes_[random_value_]]->regenerate(nodes, os);
}
