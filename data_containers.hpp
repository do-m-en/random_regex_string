#ifndef DATA_CONTAINERS_HPP_INCLUDED
#define DATA_CONTAINERS_HPP_INCLUDED

#include <variant>

#include <vector>
#include <string>

namespace rand_regex {

struct captured_group_reference_node_g;
struct captured_group_reference_node_d // capturing (stuff) with \number
{
  using generator = captured_group_reference_node_g;

  captured_group_reference_node_d(std::size_t referred_node)
    : referred_node_(referred_node)
  {
    //
  }

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "captured_group_reference_node_";}
#endif

  std::size_t referred_node_;
};

struct group_regex_node_g;
struct group_regex_node_d // (stuff)
{
  using generator = group_regex_node_g;

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "group_regex_node_";}
#endif

  void append() {++end_;}

  std::size_t end_ = 0; // end of or index
};

struct literal_regex_node_g;
struct literal_regex_node_d // literal character
{
  using generator = literal_regex_node_g;

  literal_regex_node_d(char literal) : literal_{literal} {}
  char getLiteral() const {return literal_;}

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const
  {
    std::string x;
    x += literal_;
    return "literal_regex_node_ - " + x;
  }
#endif

  char literal_;
};

struct optional_regex_node_g;
struct optional_regex_node_d // ?
{
  using generator = optional_regex_node_g;

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "optional_regex_node_";}
#endif

  int random_value_;
};

struct or_regex_node_g;
struct or_regex_node_d // |
{
  using generator = or_regex_node_g;

  or_regex_node_d() {} // TODO check if we could get rid of this one and require one element at minimum
  or_regex_node_d(const std::vector<std::size_t>& lenghts)
  {
    for(auto len : lenghts)
    {
      ++elements_count_;
      end_ += len;
    }
  }

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "or_regex_node_";}
#endif

  void append(std::size_t len) {++elements_count_; end_ += len;}

  int random_value_;

  std::size_t elements_count_ = 0;
  std::size_t end_ = 1; // end of or index
};

struct random_regex_node_g;
struct random_regex_node_d // . TODO check if this could be derived from range_random_regex_node_
{
  using generator = random_regex_node_g;

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "random_regex_node_";}
#endif

  char generated_value_;
};

struct range_random_regex_node_g;
struct range_random_regex_node_d // x-y that is used in range_regex_node_
{
  using generator = range_random_regex_node_g;

  range_random_regex_node_d(char from, char to)
    : from_(from)
    , to_(to)
  {
    //
  }

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "range_random_regex_node_";}
#endif

  char get_from() const {return from_;}
  char get_to() const {return to_;}

  char from_;
  char to_;
  char generated_value_;
};

struct repeat_range_regex_node_g;
struct repeat_range_regex_node_d // {x,y}
{
  using generator = repeat_range_regex_node_g;

  repeat_range_regex_node_d(std::size_t min, std::size_t max = 10) // TODO 10 should be a parameter
    : min_(min)
    , max_(max)
  {
    //
  }

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "repeat_range_regex_node_";}
#endif

  std::size_t min_;
  std::size_t max_;
  int random_value_;
};

struct repeat_regex_node_g;
struct repeat_regex_node_d // {x}
{
  using generator = repeat_regex_node_g;

  repeat_regex_node_d(std::size_t repeat)
    : repeat_(repeat)
  {
    //
  }

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "repeat_regex_node_";}
#endif

  std::size_t repeat_;
};

struct whitespace_regex_node_g;
struct whitespace_regex_node_d // match any whitespace character [\r\n\t\f ]
{
  using generator = whitespace_regex_node_g;

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "whitespace_regex_node_";}
#endif

  int random_value_;
};

struct empty_node_g;
struct empty_node_d
{
  using generator = empty_node_g;

#ifdef RANDOM_REGEX_DEBUG
  std::string name() const {return "empty_node_";}
#endif
};

using regex_variant = std::variant<
    empty_node_d, // TODO THROW AWAY
    captured_group_reference_node_d,
    group_regex_node_d,
    optional_regex_node_d,
    or_regex_node_d,
    repeat_range_regex_node_d,
    repeat_regex_node_d,
    literal_regex_node_d,
    random_regex_node_d,
    range_random_regex_node_d,
    whitespace_regex_node_d
  >;

} // ns rand_regex

#endif // DATA_CONTAINERS_HPP_INCLUDED
