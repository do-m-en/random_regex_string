#ifndef REPEAT_REGEX_NODE_HPP_INCLUDED
#define REPEAT_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

#include "group_regex_node.hpp"

namespace rand_regex {

class repeat_regex_node_ : public regex_node_ // {x}
{
public:
  repeat_regex_node_(group_regex_node_&& node, int repeat) : node_{std::move(node)}, repeat_{repeat} {}
  void generate(std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::ostream& os) const override;

private:
  group_regex_node_ node_;
  int repeat_;
};

};

#endif // REPEAT_REGEX_NODE_HPP_INCLUDED
