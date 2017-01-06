#ifndef WHITESPACE_REGEX_NODE_HPP_INCLUDED
#define WHITESPACE_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

namespace rand_regex {

class whitespace_regex_node_ : public regex_node_ // match any whitespace character [\r\n\t\f ]
{
public:
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "whitespace_regex_node_";}
  std::size_t get_size(const std::vector<regex_node_*>& nodes, std::size_t current_index) const override {return 1;}

private:
  int random_value_;
};

};

#endif // WHITESPACE_REGEX_NODE_HPP_INCLUDED
