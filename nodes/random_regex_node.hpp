#ifndef RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANDOM_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

namespace rand_regex {

class random_regex_node_ // . TODO check if this could be derived from range_random_regex_node_
{
public:
  void generate(std::ostream& os, random_generator_base& random_gen);
  void regenerate(std::ostream& os) const;

private:
  char generated_value_;
};

};

#endif // RANDOM_REGEX_NODE_HPP_INCLUDED
