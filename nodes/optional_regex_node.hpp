#ifndef OPTIONAL_REGEX_NODE_HPP_INCLUDED
#define OPTIONAL_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class optional_regex_node_ : public regex_node_ // ?
{
public:
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "optional_regex_node_";}

private:
  int random_value_;
};

};

#endif // OPTIONAL_REGEX_NODE_HPP_INCLUDED
