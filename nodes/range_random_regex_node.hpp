#ifndef RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class range_random_regex_node_ : public regex_node_ // x-y that is used in range_regex_node_
{
public:
  range_random_regex_node_(char from, char to);
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

  char get_from() const {return from_;}
  char get_to() const {return to_;}

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "range_random_regex_node_";}
#endif

private:
  char from_;
  char to_;
  char generated_value_;
};

};

#endif // RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
