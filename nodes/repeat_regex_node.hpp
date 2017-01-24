#ifndef REPEAT_REGEX_NODE_HPP_INCLUDED
#define REPEAT_REGEX_NODE_HPP_INCLUDED

#include <memory>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class repeat_regex_node_ : public regex_node_ // {x}
{
public:
  repeat_regex_node_(regex_node_* node, std::size_t repeat);
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "repeat_regex_node_";}
#endif

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t repeat_;
};

};

#endif // REPEAT_REGEX_NODE_HPP_INCLUDED
