#ifndef WHITESPACE_REGEX_NODE_HPP_INCLUDED
#define WHITESPACE_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

namespace rand_regex {

class whitespace_regex_node_ // match any whitespace character [\r\n\t\f ]
{
public:
  void generate(std::ostream& os, random_generator_base& random_gen);
  void regenerate(std::ostream& os) const;

private:
  int random_value_;
};

};

#endif // WHITESPACE_REGEX_NODE_HPP_INCLUDED
