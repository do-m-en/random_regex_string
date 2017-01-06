#ifndef OR_REGEX_NODE_HPP_INCLUDED
#define OR_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class or_regex_node_ : public regex_node_ // |
{
public:
  or_regex_node_();
  or_regex_node_(const std::vector<std::size_t>& lenghts);
//  void append(regex_node_* item);
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "or_regex_node_";}

  void append(std::size_t len) {jumps_.push_back(end_); end_ += len;}

private:
  std::vector<std::size_t> jumps_;
  int random_value_;

  std::size_t end_ = 1; // end of or index
};

};

#endif // OR_REGEX_NODE_HPP_INCLUDED
