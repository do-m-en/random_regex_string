#ifndef GROUP_REGEX_NODE_HPP_INCLUDED
#define GROUP_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class group_regex_node_ : public regex_node_ // (stuff)
{
public:
  group_regex_node_(std::size_t start, std::size_t count);
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

  void append() {++count_;}

  void increment_index() override {++start_;}
  void decrement_index() override {--start_;}

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "group_regex_node_";}
#endif

private:
  std::size_t start_;
  std::size_t count_;
};

};

#endif // GROUP_REGEX_NODE_HPP_INCLUDED
