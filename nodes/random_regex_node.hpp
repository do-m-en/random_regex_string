#ifndef RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANDOM_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class random_regex_node_ : public regex_node_ // . TODO check if this could be derived from range_random_regex_node_
{
public:
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "random_regex_node_";}
#endif

private:
  char generated_value_;
};

};

#endif // RANDOM_REGEX_NODE_HPP_INCLUDED
