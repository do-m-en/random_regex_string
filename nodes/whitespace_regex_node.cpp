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

void whitespace_regex_node_::generate(std::ostream& os, random_generator_base& random_gen) const
{
  os << whitespaces_[random_gen.get_random(0, whitespaces_.size() - 1)];
}
