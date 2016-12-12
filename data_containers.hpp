#ifndef DATA_CONTAINERS_HPP_INCLUDED
#define DATA_CONTAINERS_HPP_INCLUDED

#include <variant>

namespace rand_regex {

struct captured_group_reference_node_s
{
  std::size_t node_id_;
};

struct group_regex_node_s
{
  std::size_t node_id_;
  std::size_t count_;

  std::size_t jump_;
};

struct optional_regex_node_s
{
  std::size_t node_id_;
  int random_value_;

  std::size_t jump_;
};

struct or_regex_node_s
{
  std::size_t node_id_;
  std::size_t count_;
  int random_value_;

  std::size_t jump_;
};

struct repeat_range_regex_node_s
{
  std::size_t node_id_;
  std::size_t min_;
  std::size_t max_;
  int random_value_;

  std::size_t jump_;
};

struct repeat_regex_node_s
{
  std::size_t node_id_;
  std::size_t repeat_;

  std::size_t jump_;
};

//-----------------------------------

struct literal_regex_node_s
{
  char literal_;
  int random_value_;
};

struct random_regex_node_s
{
  char generated_value_;
};

struct range_random_regex_node_s
{
  char from_;
  char to_;
  char generated_value_;
};

struct whitespace_regex_node_s
{
  std::vector<char> whitespaces_; // TODO provide whitespaces by reference or a callback (amount, getter)...
  int random_value_;
};

//----------------------------------- TODO THROW AWAY

struct empty_node_s{};

//-----------------------------------

using regex_variant = std::variant<
    empty_node_s, // TODO THROW AWAY
    captured_group_reference_node_s,
    group_regex_node_s,
    optional_regex_node_s,
    or_regex_node_s,
    repeat_range_regex_node_s,
    repeat_regex_node_s,
    literal_regex_node_s,
    random_regex_node_s,
    range_random_regex_node_s,
    whitespace_regex_node_s
  >;

} // ns rand_regex

#endif // DATA_CONTAINERS_HPP_INCLUDED
