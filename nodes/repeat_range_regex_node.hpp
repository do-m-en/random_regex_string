#ifndef REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
#define REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class repeat_range_regex_node_ : public regex_node_ // {x,y}
{
public:
  repeat_range_regex_node_(std::size_t min, std::size_t max = 10); // TODO 10 should be a parameter
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "repeat_range_regex_node_";}
  std::size_t get_size(const std::vector<regex_node_*>& nodes, std::size_t current_index) const {return nodes[current_index + 1]->get_size(nodes, current_index + 1);}

private:
  std::size_t min_;
  std::size_t max_;
  int random_value_;
};

};

#endif // REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
