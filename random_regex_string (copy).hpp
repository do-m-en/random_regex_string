#ifndef RANDOM_REGEX_STRING_HPP_INCLUDED
#define RANDOM_REGEX_STRING_HPP_INCLUDED

#include <string>
#include <vector>
#include <memory>

namespace rand_regex {

class regex_node_
{
public:
  virtual void generate(std::stringstream& ss) const = 0;
};

class or_regex_node_ : public regex_node_ // |
{
public:
  or_regex_node_(regex_node_* first, regex_node_* second);
  void append(regex_node_* or_node);
  void generate(std::stringstream& ss) const override;

private:
  std::vector<regex_node_*> or_nodes_;
};

class range_random_regex_node_ : public regex_node_ // x-y that is used in range_regex_node_
{
public:
  range_random_regex_node_(char from, char to);
  void generate(std::stringstream& ss) const override;

private:
  char from_;
  char to_;
};

class range_regex_node_ : public regex_node_ // []
{
public:
  range_regex_node_(std::vector<regex_node_*>&& nodes);
  void generate(std::stringstream& ss) const override;

private:
  std::vector<regex_node_*> range_nodes_;
};

class random_regex_node_ : public regex_node_ // . TODO check if this could be derived from range_random_regex_node_
{
public:
  void generate(std::stringstream& ss) const override;
};

class repeat_zero_regex_node_ : public regex_node_ // *
{
public:
  repeat_zero_regex_node_(regex_node_* node);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
};

class optional_regex_node_ : public regex_node_ // ?
{
public:
  optional_regex_node_(regex_node_* node);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
};

class repeat_one_regex_node_ : public regex_node_ // +
{
public:
  repeat_one_regex_node_(regex_node_* node);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
};

class repeat_regex_node_ : public regex_node_ // {x}
{
public:
  repeat_regex_node_(regex_node_* node, std::size_t repeat);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t repeat_;
};

class repeat_min_regex_node_ : public regex_node_ // {x,}
{
public:
  repeat_min_regex_node_(regex_node_* node, std::size_t min);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t min_;
};

class repeat_range_regex_node_ : public regex_node_ // {x,y} and {,y} where x is set to 0
{
public:
  repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::size_t min_;
  std::size_t max_;
};

class repeat_posessive_regex_node_ : public regex_node_ // *+ TODO Should I delete this one and make it an error?
{
public:
  repeat_posessive_regex_node_(regex_node_* node, regex_node_* next_node);
  void generate(std::stringstream& ss) const override;

private:
  std::unique_ptr<regex_node_> node_;
  std::unique_ptr<regex_node_> next_node_; // optional as it could be the end of regex
};

class group_regex_node_ : public regex_node_ // (stuff)
{
public:
  group_regex_node_(std::vector<regex_node_*>&& grouped_nodes);
  void generate(std::stringstream& ss) const override;

private:
  std::vector<regex_node_*> grouped_nodes_;
};

class inclusive_regex_node_ : public regex_node_ // \p{...}
{
public:
  inclusive_regex_node_();
  void generate(std::stringstream& ss) const override;
};

class exclusive_regex_node_ : public regex_node_ // \P{...}
{
public:
  exclusive_regex_node_();
  void generate(std::stringstream& ss) const override;
};

class literal_regex_node_ : public regex_node_ // literal character
{
public:
  literal_regex_node_(char literal);
  char getLiteral() const {return literal_;}
  void generate(std::stringstream& ss) const override;

private:
  char literal_;
};

class regex_template
{
public:
  regex_template(const std::string& regex);
//  std::string generate(std::function<std::string(/*TODO*/)> unicode_to_encoding) const;
  std::string generate() const;

private:
  std::vector<regex_node_*> nodes_;
};

/**class regex_template_nothrow
{
public:
  regex_template_nothrow(const std::string& regex);
  std::string generate() const;

  std::size_t error_location;
  std::string error_reason;

private:
  std::vector<regex_node_*> nodes_;
};*/

}

#endif // RANDOM_REGEX_STRING_HPP_INCLUDED
