#ifndef LITERAL_REGEX_NODE_HPP_INCLUDED
#define LITERAL_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class inner_group_node_;

class literal_regex_node_ // literal character
{
public:
  literal_regex_node_() = default;
  literal_regex_node_(char literal);

  char getLiteral() const {return literal_;}
  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  char literal_;
  int random_value_;
};

};

#endif // LITERAL_REGEX_NODE_HPP_INCLUDED
