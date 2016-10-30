#ifndef RANDOM_REGEX_STRING_HPP_INCLUDED
#define RANDOM_REGEX_STRING_HPP_INCLUDED

#include <experimental/string_view>
#include <vector>
#include <memory>
#include <initializer_list>

#include "regex_node.hpp"

namespace rand_regex {

/*
class repeat_posessive_regex_node_ : public regex_node_ // *+ TODO Should I delete this one and make it an error? (no need as the generator will always step back)
{
public:
  repeat_posessive_regex_node_(regex_node_* node, regex_node_* next_node);
  void generate(std::ostream& os) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::unique_ptr<regex_node_> next_node_; // optional as it could be the end of regex
};

class inclusive_regex_node_ : public regex_node_ // \p{...}
{
public:
  inclusive_regex_node_();
  void generate(std::ostream& os) const override;
};

class exclusive_regex_node_ : public regex_node_ // \P{...}
{
public:
  exclusive_regex_node_();
  void generate(std::ostream& os) const override;
};*/

class regex_template
{
public:
  regex_template(const std::string& regex);
//  std::string generate(std::function<std::string(/*TODO*/)> unicode_to_encoding) const;

  void generate(std::ostream& os) const; /// TODO add nothrow edition

private:
  std::unique_ptr<regex_node_> root_node_;
};


std::string generate(const regex_template& gen);

}

#endif // RANDOM_REGEX_STRING_HPP_INCLUDED
