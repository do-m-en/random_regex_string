#ifndef OPTIONAL_REGEX_NODE_HPP_INCLUDED
#define OPTIONAL_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class optional_regex_node_ : public regex_node_ // ?
{
public:
  optional_regex_node_(regex_node_* node);
  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups) override;
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const override;

private:
  std::unique_ptr<regex_node_> node_;
  int random_value_;
};

};

#endif // OPTIONAL_REGEX_NODE_HPP_INCLUDED
