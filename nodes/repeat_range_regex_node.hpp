#ifndef REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
#define REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class repeat_range_regex_node_ : public regex_node_ // {x,y}
{
public:
  repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max);
  void generate(std::ostream& os) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t min_;
  std::size_t max_;
};

};

#endif // REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
