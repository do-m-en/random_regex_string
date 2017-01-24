#ifndef LITERAL_REGEX_NODE_HPP_INCLUDED
#define LITERAL_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class literal_regex_node_ : public regex_node_ // literal character
{
public:
  literal_regex_node_(char literal);
  char getLiteral() const {return literal_;}
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const
  {
    std::string x;
    x += literal_;
    return "literal_regex_node_ - " + x;
  }
#endif

private:
  char literal_;
  int random_value_;
};

};

#endif // LITERAL_REGEX_NODE_HPP_INCLUDED
