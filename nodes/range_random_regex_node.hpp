#ifndef RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class range_random_regex_node_ : public regex_node_ // x-y that is used in range_regex_node_
{
public:
  range_random_regex_node_(char from, char to);
  void generate(std::ostream& os, random_generator_base& random_gen) const override;

  char get_from() const {return from_;}
  char get_to() const {return to_;}

private:
  char from_;
  char to_;
};

};

#endif // RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
