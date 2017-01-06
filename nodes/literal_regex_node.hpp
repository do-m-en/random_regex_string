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
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {
      std::string x;
      x += literal_;
      return "literal_regex_node_ - " + x;
    }
  std::size_t get_size(const std::vector<regex_node_*>& nodes, std::size_t current_index) const {return 1;}

private:
  char literal_;
  int random_value_;
};

};

#endif // LITERAL_REGEX_NODE_HPP_INCLUDED
