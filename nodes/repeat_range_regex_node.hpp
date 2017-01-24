#ifndef REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
#define REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class repeat_range_regex_node_ : public regex_node_ // {x,y}
{
public:
  repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max = 10); // TODO 10 should be a parameter
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "repeat_range_regex_node_";}
#endif

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t min_;
  std::size_t max_;
  int random_value_;
};

};

#endif // REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
