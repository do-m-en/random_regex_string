#ifndef INNER_GROUP_NODE_HPP_INCLUDED
#define INNER_GROUP_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#include <memory>
#include <map>

namespace rand_regex {

class inner_group_node_ : public regex_node_ // (stuff)
{
public:
  inner_group_node_(regex_node_* node, int capture_group_no);

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const;

private:
  std::unique_ptr<regex_node_> node_;
  int capture_group_no_;
};

};

#endif // INNER_GROUP_NODE_HPP_INCLUDED
