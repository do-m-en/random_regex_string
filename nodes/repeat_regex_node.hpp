#ifndef REPEAT_REGEX_NODE_HPP_INCLUDED
#define REPEAT_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class repeat_regex_node_ : public regex_node_ // {x}
{
public:
  repeat_regex_node_(regex_node_* node, std::size_t repeat);
  ~repeat_regex_node_();
  void generate(std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::ostream& os) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t repeat_;
};

};

#endif // REPEAT_REGEX_NODE_HPP_INCLUDED
