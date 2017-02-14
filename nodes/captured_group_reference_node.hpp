#ifndef CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
#define CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class inner_group_node_;

class captured_group_reference_node_ // capturing (stuff) with \number
{
public:
  captured_group_reference_node_() = default;
  captured_group_reference_node_(int referred_group) : referred_group_{referred_group} {}

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  int referred_group_;
};

};

#endif // CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
