#ifndef OR_REGEX_NODE_HPP_INCLUDED
#define OR_REGEX_NODE_HPP_INCLUDED

#include <vector>

#include "group_regex_node.hpp"
#include "regex_node.hpp"

namespace rand_regex {

class or_regex_node_ // |
{
public:
  void push_back(group_regex_node_&& item) {nodes_.push_back(std::move(item));}

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  std::vector<group_regex_node_> nodes_;
  int random_value_;
};

};

#endif // OR_REGEX_NODE_HPP_INCLUDED
