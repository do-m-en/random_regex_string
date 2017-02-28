#ifndef DEPENDENCY_REGEX_NODE_HPP_INCLUDED
#define DEPENDENCY_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

namespace rand_regex {

class dependency_regex_node_ : public regex_node_ // generate depending on previous char
{
public:
  dependency_regex_node_(regex_node_* node_match, regex_node_* node_no_match);
  void generate(std::ostream& os, random_generator_base& random_gen, char& last_gen) override;
  void regenerate(std::ostream& os, char& last_gen) const override;

private:
  std::unique_ptr<regex_node_> node_match_;
  std::unique_ptr<regex_node_> node_no_match_;
  bool match_ = false;
};

};

#endif // DEPENDENCY_REGEX_NODE_HPP_INCLUDED
