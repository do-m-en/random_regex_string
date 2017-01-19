#ifndef GROUP_REGEX_NODE_HPP_INCLUDED
#define GROUP_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class group_regex_node_ : public regex_node_ // (stuff)
{
public:
  group_regex_node_() = default;
  group_regex_node_(std::vector<regex_node_*>&& grouped_nodes);
  void generate(std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::ostream& os) const override;

  void append(regex_node_ *node) {grouped_nodes_.push_back(node);}

private:
  std::vector<regex_node_*> grouped_nodes_;
};

};

#endif // GROUP_REGEX_NODE_HPP_INCLUDED
