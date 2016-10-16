#include "whitespace_regex_node.hpp"

#include <random>

using rand_regex::whitespace_regex_node_;

whitespace_regex_node_::whitespace_regex_node_()
{
  whitespaces_.push_back('\r');
  whitespaces_.push_back('\n');
  whitespaces_.push_back('\t');
  whitespaces_.push_back('\f');
  whitespaces_.push_back(' ');
}

void whitespace_regex_node_::generate(std::ostream& os) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0, whitespaces_.size() - 1);

  os << whitespaces_[distribution(generator)];
}
