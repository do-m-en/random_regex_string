#ifndef OR_REGEX_NODE_HPP_INCLUDED
#define OR_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class or_regex_node_ : public regex_node_ // |
{
public:
  or_regex_node_(std::initializer_list<regex_node_*> list);
  void append(regex_node_* item);
  void generate(std::ostream& os) const override;

private:
  std::vector<regex_node_*> or_nodes_; // TODO should be a unique ptr...
};

};

#endif // OR_REGEX_NODE_HPP_INCLUDED
