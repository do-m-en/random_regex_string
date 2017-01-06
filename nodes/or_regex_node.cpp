#include "or_regex_node.hpp"

using rand_regex::or_regex_node_;

or_regex_node_::or_regex_node_()
{
}

or_regex_node_::or_regex_node_(const std::vector<std::size_t>& lenghts)
{
  for(auto len : lenghts)
  {
    ++elements_count_;
    end_ += len;
  }
}

/*void or_regex_node_::append(regex_node_* item)
{
  if(auto or_node = dynamic_cast<or_regex_node_*>(item))
  {
    or_nodes_.reserve(or_nodes_.size() + or_node->or_nodes_.size());
    std::move(std::begin(or_node->or_nodes_), std::end(or_node->or_nodes_), std::back_inserter(or_nodes_));
  }
  else
  {
    or_nodes_.push_back(item);
  }
}*/

#include <iostream>
std::size_t or_regex_node_::generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: or_regex_node_ " << current_index << '\n';
#endif

  if(elements_count_ == 1)
  {
    random_value_ = 0;
  }
  else
  {
    random_value_ = random_gen.get_random(0, elements_count_ - 1);
  }

  std::size_t next_index = current_index + 1;
  for(std::size_t i = 0; i < random_value_; ++i)
  {
    next_index += nodes[next_index]->get_size(nodes, next_index);
  }

  nodes[next_index]->generate(nodes, next_index, os, random_gen);

  return end_;
}

std::size_t or_regex_node_::regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: or_regex_node_ " << current_index << '\n';
#endif

  std::size_t next_index = current_index + 1;
  for(std::size_t i = 0; i < random_value_; ++i)
  {
    next_index += nodes[next_index]->get_size(nodes, next_index);
  }

  nodes[next_index]->regenerate(nodes, next_index, os);

  return end_;
}
