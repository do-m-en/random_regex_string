#ifndef REPEAT_MIN_REGEX_NODE_HPP_INCLUDED
#define REPEAT_MIN_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class repeat_min_regex_node_ : public regex_node_ // {x,}
{
public:
  repeat_min_regex_node_(regex_node_* node, std::size_t min);
  void generate(std::ostream& os) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t min_;
};

};

#endif // REPEAT_MIN_REGEX_NODE_HPP_INCLUDED
