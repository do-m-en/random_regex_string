#ifndef LITERAL_REGEX_NODE_HPP_INCLUDED
#define LITERAL_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class literal_regex_node_ : public regex_node_ // literal character
{
public:
  literal_regex_node_(char literal);
  char getLiteral() const {return literal_;}
  void generate(std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::ostream& os) const override;
  std::string name() const override {return "literal_regex_node_";}

private:
  char literal_;
  int random_value_;
};

};

#endif // LITERAL_REGEX_NODE_HPP_INCLUDED
