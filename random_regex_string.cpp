#include "random_regex_string.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <functional>

#include "captured_group_reference_node.hpp"
#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"
#include "range_or_node.hpp"
#include "inner_group_node.hpp"

using rand_regex::captured_group_reference_node_;
using rand_regex::group_regex_node_;
using rand_regex::inner_group_node_;
using rand_regex::range_or_node_;
using rand_regex::literal_regex_node_;
using rand_regex::optional_regex_node_;
using rand_regex::or_regex_node_;
using rand_regex::range_random_regex_node_;
using rand_regex::random_regex_node_;
using rand_regex::regex_node_;
using rand_regex::repeat_range_regex_node_;
using rand_regex::repeat_regex_node_;
using rand_regex::whitespace_regex_node_;

constexpr char ascii_min = 0;
constexpr char ascii_max = 127;

#include <iostream> // FIXME delete

#include <type_traits>

namespace test_parser {

struct unused_type{};

template<typename Derived>
struct parser
{
  const Derived& derived() const
  {
    return *static_cast<const Derived*>(this);
  }
};

struct any_int_parser : parser<any_int_parser>
{
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    Iterator start = first;

    int digit = 0;
    while(first != last && std::isdigit(*first))
    {
      digit = (digit * 10) + (*first - '0');
      ++first;
    }

    if(start == first)
      return false;

    if constexpr(!std::is_same_v<Attribute, unused_type>)
    {
      // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
      if constexpr(std::is_same_v<Attribute, int>)
        attr = digit;
      else
        attr.push_back(digit);
    }

    return true;
  }
};

inline auto int_()
{
  return any_int_parser{};
}

template<typename Element>
struct any_char_parser : parser<any_char_parser<Element>>
{
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(first != last)
    {
      if constexpr(!std::is_same_v<Attribute, unused_type>)
      {
        // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
        if constexpr(std::is_same_v<Attribute, Element>)
          attr = *first;
        else
          attr.push_back(Element{*first});
      }
      ++first;
      return true;
    }

    return false;
  }
};

template<typename Element>
inline auto char_()
{
  return any_char_parser<Element>{};
}

template<typename Char>
struct char_parser : parser<char_parser<Char>>
{
  char_parser(Char ch) : ch_{ch} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(first != last && *first == ch_)
    {
      ++first;
      return true;
    }

    return false;
  }

  Char ch_;
};

template<typename Char>
inline auto char_(Char ch)
{
  return char_parser<Char>{ch};
}
/*
template<typename Char>
struct literal_parser : parser<literal_parser<Char>>
{
  literal_parser(Char ch) : ch_{ch} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(first != last && *first == ch_)
    {
      if constexpr(!std::is_same_v<Attribute, unused_type>)
      {
        // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
        if constexpr(std::is_same_v<Attribute, Char>)
          attr = ch_;
        else
          attr.push_back(ch_);
      }
      ++first;
      return true;
    }

    return false;
  }

  Char ch_;
};

template<typename Char>
inline auto lit_(Char ch)
{
  return literal_parser<Char>{ch};
}*/

template<typename Element>
struct hex_char_parser : parser<hex_char_parser<Element>>
{
  hex_char_parser(int ascii_min, int ascii_max, int no_count)
    : ascii_min_{ascii_min}, ascii_max_{ascii_max}, no_count_{no_count} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(first != last)
    {
      std::string chars;
      for(int i=0; i<no_count_; ++i)
      {
         if(first == last)
           return false;

         chars += *first;
         ++first;
      }

      int ascii_hex = std::stoi(chars, nullptr, 16);

      if(ascii_hex > ascii_max_)
        return false;

      if constexpr(!std::is_same_v<Attribute, unused_type>)
      {
        // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
        if constexpr(std::is_same_v<Attribute, Element>)
          attr = Element{static_cast<char>(ascii_hex)};
        else
          attr.push_back(Element{static_cast<char>(ascii_hex)});
      }

      return true;
    }

    return false;
  }

  int ascii_min_;
  int ascii_max_;
  int no_count_;
};

template<typename Element>
inline auto hex_char_(int ascii_min, int ascii_max, int no_count)
{
  return hex_char_parser<Element>(ascii_min, ascii_max, no_count);
}

template<typename Right>
struct not_parser : parser<not_parser<Right>>
{
  not_parser(Right right) : right_{right} {}
  
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(auto [mark, unused] = std::tuple{first, unused_type{}}; right_.parse(first, last, ctx, stash, unused)) // lookahead so always no attr capture
    {
      first = mark;

      return false;
    }
    else
    {
      first = mark;

      return true;
    }
  }

  Right right_;
};

template<typename Right>
inline auto operator!(const parser<Right>& right)
{
  return not_parser<Right>{right.derived()};
}

template<typename Right>
struct lookahead_parser : parser<lookahead_parser<Right>>
{
  lookahead_parser(Right right) : right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(auto [mark, unused] = std::tuple{first, unused_type{}}; right_.parse(first, last, ctx, stash, unused)) // lookahead so always no attr capture
    {
      first = mark;

      return true;
    }
    else
    {
      first = mark;

      return false;
    }
  }

  Right right_;
};

template<typename Right>
inline auto look_(const parser<Right>& right)
{
  return lookahead_parser<Right>{right.derived()};
}

template<typename Right>
struct optional_parser : parser<optional_parser<Right>>
{
  optional_parser(Right right) : right_{right} {}
  
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    right_.parse(first, last, ctx, stash, attr);

    return true;
  }

  Right right_;
};

template<typename Right>
inline optional_parser<Right> operator-(const parser<Right>& right)
{
  return optional_parser<Right>{right.derived()};
}

template<typename Element, typename Right>
struct plus_parser : parser<plus_parser<Element, Right>> // parse one or more
{
  plus_parser(Right right) : right_{right} {}
  
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if constexpr(std::is_same_v<Attribute, unused_type>)
    {
      if(right_.parse(first, last, ctx, stash, attr))
      {
        while(right_.parse(first, last, ctx, stash, attr));

        return true;
      }
    }
    // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
    else if constexpr(std::is_same_v<Attribute, Element>)
    {
      if(right_.parse(first, last, ctx, stash, attr))
      {
        while(right_.parse(first, last, ctx, stash, attr));

        return true;
      }
    }
    else if(Element el; right_.parse(first, last, ctx, stash, el))
    {
      while(right_.parse(first, last, ctx, stash, el));

      attr.push_back(std::move(el));

      return true;
    }

    return false;
  }

  Right right_;
};

template<typename Element, typename Right>
inline auto plus(const parser<Right>& right)
{
  return plus_parser<Element, Right>{right.derived()};
}

template<typename Left, typename Right>
struct sequence_parser : parser<sequence_parser<Left, Right>>
{
  sequence_parser(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    return left_.parse(first, last, ctx, stash, attr) && right_.parse(first, last, ctx, stash, attr);
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline sequence_parser<Left, Right> operator>>(
  const parser<Left>& left, const parser<Right>& right)
{
  return sequence_parser<Left, Right>{left.derived(), right.derived()};
}

template<typename Left, typename Right>
struct alternative_parser : parser<alternative_parser<Left, Right>>
{
  alternative_parser(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if(left_.parse(first, last, ctx, stash, attr))
      return true;

    return right_.parse(first, last, ctx, stash, attr);
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline alternative_parser<Left, Right> operator|(
  const parser<Left>& left, const parser<Right>& right)
{
  return alternative_parser<Left, Right>{left.derived(), right.derived()};
}

// -- context
template<typename T>
struct defer_type // same as boost::mpl::identity
{
//  typedef T type;
};

template<typename ID, typename T, typename Next_context>
struct context
{
  context(const T& val, const Next_context& next_ctx)
    : val_{val}, next_ctx_{next_ctx} {}

  const T& get(defer_type<ID>) const
  {
    return val_;
  }

  template<typename Identity>
//  decltype(std::declval<Next_context>().get(Identity()))
  auto get(Identity id) const
  {
    return next_ctx_.get(id);
  }

  const T& val_;
  const Next_context& next_ctx_;
};

struct empty_context
{
  struct undefined{};

  template<typename ID>
  undefined get(ID) const
  {
    return undefined{};
  }
};

// -- rule definition & rule

template<typename ID, typename RHS>
struct rule_definition : parser<rule_definition<ID, RHS>>
{
  rule_definition(RHS rhs) : rhs_{rhs} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    context<ID, RHS, Context> this_ctx{rhs_, ctx};
    return rhs_.parse(first, last, this_ctx, stash, attr);
  }

  RHS rhs_;
};

template<typename ID>
struct rule : parser<rule<ID>>
{
  template<typename Derived>
//  rule_definition<ID, Derived>
  auto operator=(const parser<Derived>& definition) const
  {
    return rule_definition<ID, Derived>{definition.derived()};
  }

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    return ctx.get(defer_type<ID>{}).parse(first, last, ctx, stash, attr);
  }
};

// -- type modifier

template<typename Element, typename Right>
struct type_modifier : parser<type_modifier<Element, Right>>
{
  type_modifier(Right right) : right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if constexpr(std::is_same_v<Attribute, unused_type>)
      return right_.parse(first, last, ctx, stash, attr);

    Element el;
    bool res = right_.parse(first, last, ctx, stash, el);

    if(res)
      attr.push_back(std::move(el));

    return res;
  }

  Right right_;
};

template<typename Element, typename Right>
inline auto type_mod(const parser<Right>& right)
{
  return type_modifier<Element, Right>{right.derived()};
}

template<typename Type1, typename Type2, typename Left, typename Right>
struct compare_modifier : parser<compare_modifier<Type1, Type2, Left, Right>>
{
  compare_modifier(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    Type1 left_item;
    Type2 right_item;
    if(!left_.parse(first, last, ctx, stash, left_item) || !right_.parse(first, last, ctx, stash, right_item))
      return false;

    if(right_item == -1)
      right_item = left_item + 10; // 10 == max auto + items
    else if(right_item == -2)
      right_item = left_item;
    else if(left_item > right_item)
      return false;

    if constexpr(!std::is_same_v<Attribute, unused_type>)
      attr = std::tuple<int,int>(left_item, right_item);

    return true;
  }

  Left left_;
  Right right_;
};

template<typename Type1, typename Type2, typename Left, typename Right>
inline auto compare(const parser<Left>& left, const parser<Right>& right)
{
  return compare_modifier<Type1, Type2, Left, Right>{left.derived(), right.derived()};
}

template<typename Element_left, typename Element_both, typename Type1, typename Type2, typename Left, typename Right>
struct ternary_modifier : parser<ternary_modifier<Element_left, Element_both, Type1, Type2, Left, Right>>
{
  ternary_modifier(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    Type1 left_item;
    if(!left_.parse(first, last, ctx, stash, left_item))
      return false;

    Type2 right_item;
    if(right_.parse(first, last, ctx, stash, right_item))
      attr.push_back(std::move(Element_both{std::move(left_item), std::move(right_item)}));
    else
      attr.push_back(std::move(Element_left{std::move(left_item)}));

    return  true;
  }

  Left left_;
  Right right_;
};

template<typename Element_left, typename Element_both, typename Type1, typename Type2, typename Left, typename Right>
inline auto ternary(const parser<Left>& left, const parser<Right>& right)
{
  return ternary_modifier<Element_left, Element_both, Type1, Type2, Left, Right>{left.derived(), right.derived()};
}


template<typename Element_left, typename Element_both, typename Type, typename Left, typename Right>
inline auto ternary(const parser<Left>& left, const parser<Right>& right)
{
  return ternary_modifier<Element_left, Element_both, Type, Type, Left, Right>{left.derived(), right.derived()};
}

template<typename Element>
struct attribute_generator : parser<attribute_generator<Element>>
{
  attribute_generator(Element&& value) : value_{std::forward<Element>(value)} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if constexpr(!std::is_same_v<Attribute, unused_type>)
    {
      // poor mans check if element should be assigned or pushed back (could check for push_back but this is enough)
      if constexpr(std::is_same_v<Attribute, Element>)
        attr = value_;
      else
        attr.push_back(value_);
    }

    return true;
  }

  Element value_;
};

template<typename Element>
inline auto gen(Element&& value)
{
  return attribute_generator<Element>{std::forward<Element>(value)};
}

template<typename Right>
struct group_add : parser<group_add<Right>>
{
  group_add(Right right) : right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if constexpr(!std::is_same_v<Attribute, unused_type>)
    {
      std::size_t index = stash.groups.size();
      stash.groups.push_back(false);

      bool res = right_.parse(first, last, ctx, stash, attr);

      if(res)
        stash.groups[index] = true;

      return res;
    }
    else
      return right_.parse(first, last, ctx, stash, attr);
  }

  Right right_;
};

template<typename Right>
inline auto grp_add(Right right)
{
  return group_add<Right>{right};
}

template<typename Right>
struct capture_group_setter : parser<capture_group_setter<Right>>
{
  capture_group_setter(Right right) : right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    if constexpr(!std::is_same_v<Attribute, unused_type>)
    {
      attr.set_capture_group(stash.groups.size());
    }

    return right_.parse(first, last, ctx, stash, attr);
  }

  Right right_;
};

template<typename Right>
inline auto set_capture_group(Right right)
{
  return capture_group_setter<Right>{right};
}

struct capture_group_parser : parser<capture_group_parser>
{
  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    int digit = 0;
    while(first != last && std::isdigit(*first))
    {
      digit = (digit * 10) + (*first - '0');
      ++first;
    }

    if constexpr(!std::is_same_v<Attribute, unused_type>)
    {
      // TODO check if group is still marked with false (we are inside of the group)
      // TODO check if group exceeds expected value - //.set_capture_group(stash.groups.size() - 1);
      attr = captured_group_reference_node_{digit};
    }

    return true;
  }
};

template<typename Right>
struct invert_range_parser_ : parser<invert_range_parser_<Right>>
{
  invert_range_parser_(Right right) : right_{right} {}

  template<typename Iterator, typename Context, typename Stash, typename Attribute>
  bool parse(Iterator& first, Iterator last, const Context& ctx, Stash& stash, Attribute& attr) const
  {
    range_or_node_ nodes;
    if(!right_.parse(first, last, ctx, stash, nodes))
      return false;

    attr = nodes;
/*    std::vector<std::pair<char, char>> ranges;

    // convert
    for(auto& element : tmp)
    {
      if(auto lit = std::get_if<literal_regex_node_d>(&element))
        ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
      else
      {
        auto range = std::get<range_random_regex_node_d>(element);
        ranges.emplace_back(range.get_from(), range.get_to());
      }
    }

    // handle single range case
    if(tmp.size() == 1)
    {
      // TODO merge
      if(ranges[0].first != ascii_min && ranges[0].second != ascii_max)
      {
        invert.push_back(range_random_regex_node_d{ascii_min, ranges[0].first - 1});
        invert.push_back(range_random_regex_node_d{ranges[0].second + 1, ascii_max});
      }
      else if(ranges[0].first == ascii_min && ranges[0].second == ascii_max)
        // in rare case where no characters are allowed throw an exception as regex is faulty...
        throw 1; // TODO exception
      else if(ranges[0].first == ascii_min)
        invert.push_back(range_random_regex_node_d{ranges[0].second + 1, ascii_max});
      else
        invert.push_back(range_random_regex_node_d{ascii_min, ranges[0].first - 1});
    }*/
/*    else
    {
      std::sort(ranges.begin(), ranges.end(), [](auto& i, auto& j){return (i.first<j.first);});

      // merge and invert ranges
      auto min = ascii_min;
      auto cur_it = ranges.begin();
      for(auto it = ranges.begin() + 1; it != ranges.end(); ++it)
      {
        auto& cur = *cur_it;
        auto& next = *it;

        if(cur.second >= next.second) // because of the sort we know that first is either same or smaller so a sub range
        {
          if(it+1 == ranges.end()) // TODO merge with lower
          {
            if(cur.first != ascii_min && cur.second != ascii_max)
            {
              invert.push_back(range_random_regex_node_d{min, cur.first - 1});
              invert.push_back(range_random_regex_node_d{cur.second + 1, ascii_max});
            }
            else if(cur.first == ascii_min && cur.second == ascii_max)
              // in rare case where no characters are allowed throw an exception as regex is faulty...
              throw 1; // TODO exception
            else if(cur.first == ascii_min)
              invert.push_back(range_random_regex_node_d{cur.second + 1, ascii_max});
            else
              invert.push_back(range_random_regex_node_d{min, cur.first - 1});

            break;
          }

          continue;
        }
        else if(cur.second+1 >= next.first) // adjacent or overlapping ranges
        {
          if(it+1 == ranges.end()) // TODO merge with upper
          {
            if(cur.first != ascii_min && next.second != ascii_max)
            {
              invert.push_back(range_random_regex_node_d{min, cur.first - 1});
              invert.push_back(range_random_regex_node_d{next.second + 1, ascii_max});
            }
            else if(cur.first == ascii_min && next.second == ascii_max)
              // in rare case where no characters are allowed throw an exception as regex is faulty...
              throw 1; // TODO exception
            else if(cur.first == ascii_min)
              invert.push_back(range_random_regex_node_d{next.second + 1, ascii_max});
            else
              invert.push_back(range_random_regex_node_d{min, cur.first - 1});

            break;
          }

          cur.second = next.second;
        }
        else // ranges aren't connected
        {
          if(cur.first != ascii_min && cur.second != ascii_max)
          {
            invert.push_back(range_random_regex_node_d{min, static_cast<char>(cur.first - 1)});
            invert.push_back(range_random_regex_node_d{static_cast<char>(cur.second + 1), static_cast<char>(next.first - 1)});
          }
          else if(cur.first == ascii_min && cur.second == ascii_max)
            // in rare case where no characters are allowed throw an exception as regex is faulty...
            throw 1; // TODO exception
          // else skip the element as not allowed numbers are from beginning to end of this range
          // TODO !min but ==max with merge, merge, merge...???

          min = cur.second + 1;
          cur_it = it;
        }
      }
    }*/

    return true;
  }

  Right right_;
};

template<typename Right>
inline auto invert_(Right right)
{
  return invert_range_parser_<Right>{right};
}

} // ns

namespace ascii_regex_grammar
{
  namespace grammar
  {
    using namespace test_parser;

    struct max_repeats
    {
      template<typename T>
      max_repeats(T) {} // ignore the assignment
      operator int() {return 10;}
    };

    const rule<class regex_rule> regex_rule;
    constexpr char ascii_min = 0;
    constexpr char ascii_max = 127;

    const auto any_digit_g = char_('d') >> gen(std::move(range_random_regex_node_{'0', '9'}));

    const auto any_non_digit_g =
           gen(std::move(range_random_regex_node_{ascii_min, '0' - 1}))
        >> gen(std::move(range_random_regex_node_{'9' + 1, ascii_max}));

    const auto any_whitespace_char_g =
           gen(std::move(literal_regex_node_{'\r'}))
        >> gen(std::move(literal_regex_node_{'\n'}))
        >> gen(std::move(literal_regex_node_{'\t'}))
        >> gen(std::move(literal_regex_node_{'\f'}))
        >> gen(std::move(literal_regex_node_{' '}));

    const auto any_non_whitespace_char_g =
           gen(std::move(range_random_regex_node_{ascii_min, '\t' - 1}))
        >> gen(std::move(range_random_regex_node_{'\r' + 1, ' ' - 1}))
        >> gen(std::move(range_random_regex_node_{' ' + 1, ascii_max}));

    const auto any_word_character_g =
           gen(std::move(range_random_regex_node_{'a', 'z'}))
        >> gen(std::move(range_random_regex_node_{'0', '9'}))
        >> gen(std::move(literal_regex_node_{'_'}));

    const auto any_non_word_character_g =
           gen(std::move(range_random_regex_node_{ascii_min, '0' - 1}))
        >> gen(std::move(range_random_regex_node_{'9' + 1, 'A' - 1}))
        >> gen(std::move(range_random_regex_node_{'Z' + 1, '_' - 1}))
        >> gen(std::move(range_random_regex_node_{'_' + 1, 'a' - 1}))
        >> gen(std::move(range_random_regex_node_{'z' + 1, ascii_max}));

    const auto non_rangable_g =
          any_digit_g
        | (char_('D') >> any_non_digit_g)
        | (char_('s') >> any_whitespace_char_g)
        | (char_('S') >> any_non_whitespace_char_g)
        | (char_('w') >> any_word_character_g)
        | (char_('W') >> any_non_word_character_g);

    const auto escaped_chars_g =
          char_('r') >> gen('\r')
        | char_('n') >> gen('\n')
        | char_('t') >> gen('\t')
        | char_('f') >> gen('\f');

    const auto range_g =
          (!char_('\\') >> ternary<literal_regex_node_, range_random_regex_node_, char>(
            char_<char>(),
            (!(char_('-') >> char_(']')) >> char_('-') >> char_<char>())))
        | (char_('\\')
            >> (non_rangable_g
               | (ternary<literal_regex_node_, range_random_regex_node_, char>(
                    char_('^') >> gen('^'),
                    !(char_('-') >> (char_(']') | (char_('\\') >> (char_('d') | char_('D') | char_('s') | char_('S') | char_('w') | char_('W')))))
                       >> char_('-') >> ((char_('\\') >> escaped_chars_g ) | char_<char>())) // TODO all escaped_chars_g are smaller than ^ symbol so they should be treated as error - also somehow chack that lhs is smaller than the rhs
                 )
               | (ternary<literal_regex_node_, range_random_regex_node_, char>(
                    char_('r') >> gen('\r'),
                    !(char_('-') >> (char_(']') | (char_('\\') >> (char_('d') | char_('D') | char_('s') | char_('S') | char_('w') | char_('W')))))
                       >> char_('-') >> ((char_('\\') >> escaped_chars_g ) | char_<char>()))
                 )
               | (ternary<literal_regex_node_, range_random_regex_node_, char>(
                    char_('n') >> gen('\n'),
                    !(char_('-') >> (char_(']') | (char_('\\') >> (char_('d') | char_('D') | char_('s') | char_('S') | char_('w') | char_('W')))))
                       >> char_('-') >> ((char_('\\') >> escaped_chars_g ) | char_<char>()))
                 )
               | (ternary<literal_regex_node_, range_random_regex_node_, char>(
                    char_('t') >> gen('\t'),
                    !(char_('-') >> (char_(']') | (char_('\\') >> (char_('d') | char_('D') | char_('s') | char_('S') | char_('w') | char_('W')))))
                       >> char_('-') >> ((char_('\\') >> escaped_chars_g ) | char_<char>()))
                 )
               | (ternary<literal_regex_node_, range_random_regex_node_, char>(
                    char_('f') >> gen('\f'),
                    !(char_('-') >> (char_(']') | (char_('\\') >> (char_('d') | char_('D') | char_('s') | char_('S') | char_('w') | char_('W')))))
                       >> char_('-') >> ((char_('\\') >> escaped_chars_g ) | char_<char>()))
                 )
            ));

    const auto range_group_g =
          (char_('^') >> invert_(plus<range_or_node_>(!char_(']') >> range_g)))
        | plus<range_or_node_>(!char_(']') >> range_g);

    const auto repeat_g = // create factor variant repeat element
          compare<int, int>(int_(), ((char_(',') >> (int_() | gen(-1))) | gen(-2))); // TODO rename... and perhaps make composable..

    const auto repeat_base_g =
          (char_('*') >> gen(std::move(std::tuple<int, int>{0, 10})))
        | (char_('+') >> gen(std::move(std::tuple<int, int>{1, 10})))
        | (char_('?') >> gen(std::move(std::tuple<int, int>{0, 1})) >> -char_('?')) // for some reason two question marks are allowed but without making a difference in case of generation
        | (char_('{') >> repeat_g >> char_('}') >> -char_('?')); // ? doesn't change the result

    const auto whitespace_g = char_(' ') | char_('\f') | char_('\n') | char_('\r') | char_('\t') | char_('\v');

    const auto base_g =
          (char_('(')
            >> (!(char_('?') >> char_(':')) >> grp_add(
              ternary<inner_group_node_, repeat_range_regex_node_, inner_group_node_, std::tuple<int, int>>(
                set_capture_group(-regex_rule >> -char_('$') >> char_(')')), repeat_base_g)))
            | (char_('?') >> char_(':') >>
               ternary<inner_group_node_, repeat_range_regex_node_, inner_group_node_, std::tuple<int, int>>(
                 -regex_rule >> -char_('$') >> char_(')'), repeat_base_g)
             ))
        | (char_('[') >>
            ternary<range_or_node_, repeat_range_regex_node_, range_or_node_, std::tuple<int, int>>(
               range_group_g >> char_(']'), repeat_base_g))
        | (char_('\\') >>
            (
                ternary<range_random_regex_node_, repeat_range_regex_node_, range_random_regex_node_, std::tuple<int, int>>(
                  any_digit_g, repeat_base_g)
              | ternary<inner_group_node_, repeat_range_regex_node_, inner_group_node_, std::tuple<int, int>>(
                    (char_('D') >> type_mod<group_regex_node_>(
                        type_mod<range_or_node_>(
                             any_non_digit_g)))
                  | (char_('s') >> type_mod<group_regex_node_>( // whitespaces
                        type_mod<range_or_node_>(
                             any_whitespace_char_g )))
                  | // '\t' // tab: 9; '\n' // newline: 10; '\v' // vertical tab: 11; '\f' // formfeed: 12; '\r' // carriage return: 13; ' ' // space: 32 */
                    (char_('S') >> type_mod<group_regex_node_>(
                       type_mod<range_or_node_>(
                             any_non_whitespace_char_g )))
                  | // any alphanumeric characters or _
                    (char_('w') >> type_mod<group_regex_node_>(
                       type_mod<range_or_node_>(
                             any_word_character_g )))
                  | (char_('W') >> type_mod<group_regex_node_>(
                       type_mod<range_or_node_>(
                           any_non_word_character_g )))
                  , repeat_base_g)
              | (look_(char_('1') | char_('2') | char_('3') | char_('4') | char_('5') | char_('6') | char_('7') | char_('8') | char_('9'))
                  >> ternary<captured_group_reference_node_, repeat_range_regex_node_, captured_group_reference_node_, std::tuple<int, int>>(
                      capture_group_parser{}, repeat_base_g)
                    )
              | ternary<literal_regex_node_, repeat_range_regex_node_, literal_regex_node_, std::tuple<int, int>>(
                    (char_('f') >> gen(std::move(literal_regex_node_{'\f'})))
                  | (char_('n') >> gen(std::move(literal_regex_node_{'\n'})))
                  | (char_('r') >> gen(std::move(literal_regex_node_{'\r'})))
                  | (char_('t') >> gen(std::move(literal_regex_node_{'\t'})))
                  | (char_('v') >> gen(std::move(literal_regex_node_{'\v'})))
                  | (char_('0') >> gen(std::move(literal_regex_node_{'\0'})))
                  | (char_('x') >> hex_char_<literal_regex_node_>(ascii_min, ascii_max, 2)) // \x00 to \x7F
                  | (-(char_('b') | char_('B')) // skip B or b as it doesn't change anything (\b functionality... // TODO \b means word boundary (^\w|\w$|\W\w|\w\W) so "- \bc is valid while "-\b c" isn't..")
                      >> char_<literal_regex_node_>()), repeat_base_g) // TODO add capture group number version
            ))
        | (!( whitespace_g | char_('.') // we'll handle these cases later as not honoring them should cause grammar termination - \b functionality...
             | char_('$') | char_(')') | char_('*') | char_('+') | char_('?') | (char_('{') >> int_())) >> // prevent parsing of characters that are not allowed in this context
            ternary<literal_regex_node_, repeat_range_regex_node_, literal_regex_node_, std::tuple<int, int>>(
                look_(char_<literal_regex_node_>() >> (!(char_('\\') >> char_('b')) | (char_('\\') >> char_('b') >> (whitespace_g | char_('.')))))
                >> char_<literal_regex_node_>(), repeat_base_g)) // allot of handling for this \b functionality...
        | (look_(whitespace_g >> -(repeat_base_g) >> (!(char_('\\') >> char_('b')) | ( char_('\\') >> char_('b') >> char_<literal_regex_node_>()))) >> // \b functionality...
            ternary<literal_regex_node_, repeat_range_regex_node_, literal_regex_node_, std::tuple<int, int>>(
                char_<literal_regex_node_>(), repeat_base_g))
        | (look_(char_('.') // any character except LF (ascii 10 - \n) or CR (ascii 13 - \r) as it doesn't generate line terminators
            >> (!(-(repeat_base_g) >> char_('\\') >> char_('b')) | (-(repeat_base_g) >> char_('\\') >> char_('b') >> char_<literal_regex_node_>()))) // \b functionality...
            >> ternary<inner_group_node_, repeat_range_regex_node_, inner_group_node_, std::tuple<int, int>>(
                char_('.') >> type_mod<group_regex_node_>(
                  type_mod<range_or_node_>(
                      gen(std::move(range_random_regex_node_{ascii_min, 9})) // \n - 1
                   >> gen(std::move(range_random_regex_node_{11, 12})) // \n + 1, \r - 1
                   >> gen(std::move(range_random_regex_node_{14, ascii_max})))) // \r + 1
                , repeat_base_g)); // TODO for \b correctly handle generation of spaces before or after (and correctly handle \B to be without spaces before and after)

    const auto term_g =
          plus<group_regex_node_>(!char_('|') >> base_g);

    const auto regex_g = regex_rule =
          -char_('|') >> term_g >> -(char_('|') >> regex_rule); // TODO generate empty node...

    const auto start_g = -char_('^') >> -regex_g >> -char_('$'); // TODO split regex_g into base level and deaper ones (so that base level can restart ^ and $ tags)...
  }

  using grammar::start_g;
};

#include <vector>
#include <variant>

using rand_regex::regex_template;

regex_template::regex_template(std::string_view regex)
{
  auto begin = regex.begin();

  struct Stash
  {
    std::vector<bool> groups;
  };

  Stash stash;
  ascii_regex_grammar::start_g.parse(begin, regex.end(), test_parser::unused_type{}, stash, root_node_);

  std::cout << (begin == regex.end()) << '\n';

  if(begin != regex.end())
  {
    std::cout << "FATAL ERROR!\n";
    exit(1);
  }
}

void regex_template::generate(std::ostream& os)
{
  random_generator<> rand; // TODO should be provided from the outside
  std::vector<std::tuple<int, inner_group_node_*>> groups;

  root_node_.generate(os, rand, groups);
}
