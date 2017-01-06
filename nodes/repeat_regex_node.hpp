#ifndef REPEAT_REGEX_NODE_HPP_INCLUDED
#define REPEAT_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class repeat_regex_node_ : public regex_node_ // {x}
{
public:
  repeat_regex_node_(std::size_t repeat);
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "repeat_regex_node_";}

private:
  std::size_t repeat_;
};

};

#endif // REPEAT_REGEX_NODE_HPP_INCLUDED
