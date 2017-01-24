#ifndef WHITESPACE_REGEX_NODE_HPP_INCLUDED
#define WHITESPACE_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class whitespace_regex_node_ : public regex_node_ // match any whitespace character [\r\n\t\f ]
{
public:
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "whitespace_regex_node_";}
#endif

private:
  int random_value_;
};

};

#endif // WHITESPACE_REGEX_NODE_HPP_INCLUDED
