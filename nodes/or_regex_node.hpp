#ifndef OR_REGEX_NODE_HPP_INCLUDED
#define OR_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class or_regex_node_ : public regex_node_ // |
{
public:
  or_regex_node_() = default;
  or_regex_node_(const std::vector<std::size_t>& nodes);
  or_regex_node_(std::initializer_list<std::size_t> node_indices);
  void append(std::size_t item);
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

  void increment_index() override
  {
    for(auto& index : or_nodes_)
      ++index;
  }

  void decrement_index() override
  {
    for(auto& index : or_nodes_)
      --index;
  }

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "or_regex_node_";}
#endif

private:
  std::vector<std::size_t> or_nodes_;
  int random_value_;
};

};

#endif // OR_REGEX_NODE_HPP_INCLUDED
