#ifndef RANGE_REGEX_NODE_HPP_INCLUDED
#define RANGE_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class range_regex_node_ : public regex_node_ // []
{
public:
  range_regex_node_(std::vector<regex_node_*>&& nodes);
  void generate(std::ostream& os) const override;

private:
  std::vector<regex_node_*> range_nodes_;
};

};

#endif // RANGE_REGEX_NODE_HPP_INCLUDED
