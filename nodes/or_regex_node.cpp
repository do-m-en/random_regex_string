#include "or_regex_node.hpp"

using rand_regex::or_regex_node_;

// TODO add bool squash or nodes template flag or helper function that does that in advance (maybe better)...
or_regex_node_::or_regex_node_(std::initializer_list<regex_node_*> list)
{
  // unwrap or nodes
  for(auto* item : list)
  {
    if(auto or_node = dynamic_cast<or_regex_node_*>(item))
    {
      or_nodes_.reserve(or_nodes_.size() + or_node->or_nodes_.size());
      std::move(std::begin(or_node->or_nodes_), std::end(or_node->or_nodes_), std::back_inserter(or_nodes_));

      delete or_node;
    }
    else
    {
      or_nodes_.push_back(item);
    }
  }
}

void or_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups)
{
  if(or_nodes_.size() == 1)
  {
    random_value_ = 0;
  }
  else
  {
    random_value_ = random_gen.get_random(0, or_nodes_.size() - 1);
  }

  or_nodes_[random_value_]->generate(os, random_gen, groups);
}

void or_regex_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const
{
  or_nodes_[random_value_]->regenerate(os, groups);
}
