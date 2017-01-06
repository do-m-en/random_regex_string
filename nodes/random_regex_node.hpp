#ifndef RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANDOM_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

namespace rand_regex {

class random_regex_node_ : public regex_node_ // . TODO check if this could be derived from range_random_regex_node_
{
public:
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "random_regex_node_";}

private:
  char generated_value_;
};

};

#endif // RANDOM_REGEX_NODE_HPP_INCLUDED
