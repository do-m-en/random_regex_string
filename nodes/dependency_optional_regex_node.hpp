#ifndef DEPENDENCY_OPTIONAL_REGEX_NODE_HPP_INCLUDED
#define DEPENDENCY_OPTIONAL_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class dependency_optional_regex_node_ : public regex_node_ // generate if previous char doesn't fulfill the expectation
{
public:
  dependency_optional_regex_node_(bool not_matching, regex_node_* node);
  void generate(std::ostream& os, random_generator_base& random_gen, char& last_gen) override;
  void regenerate(std::ostream& os, char& last_gen) const override;

private:
  std::unique_ptr<regex_node_> node_;
  bool not_matching_;
  bool generate_ = false;
};

};

#endif // DEPENDENCY_OPTIONAL_REGEX_NODE_HPP_INCLUDED
