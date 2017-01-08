#include "random_regex_string.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <functional>
#include <iterator>
#include <numeric>
#include <type_traits>

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

using rand_regex::captured_group_reference_node_d;
using rand_regex::group_regex_node_d;
using rand_regex::literal_regex_node_d;
using rand_regex::optional_regex_node_d;
using rand_regex::or_regex_node_d;
using rand_regex::range_random_regex_node_d;
using rand_regex::random_regex_node_d;
using rand_regex::empty_node_d;
using rand_regex::repeat_range_regex_node_d;
using rand_regex::repeat_regex_node_d;
using rand_regex::whitespace_regex_node_d;

using rand_regex::regex_variant;

constexpr char ascii_min = 0;
constexpr char ascii_max = 127;


// Scott Schurr's str_const (constexpr string)
// see: https://github.com/boostcon/cppnow_presentations_2012/blob/master/wed/schurr_cpp11_tools_for_class_authors.pdf?raw=true
/*class str_const
{
public:
  template<std::size_t N>
  constexpr str_const(const char(&a)[N])
    : p_(a)
    , sz_(N-1)
  {
    //
  }

  constexpr char operator[](std::size_t n)
  {
    return n < sz_ ? p_[n] : throw std::out_of_range("");
  }

  constexpr std::size_t size() { return sz_; }
private:
  const char* const p_;
  const std::size_t sz_;
};*/


struct regex_param
{
  regex_param(std::string_view regex_) : regex{regex_} {}

  std::string_view regex;
  std::size_t consumed = 0;
  std::vector<int> captured_groups;

  std::size_t last_index = 0;
};

using regex_consumer_function = std::function<std::vector<regex_variant> (regex_param&)>;

// class regex_template
using rand_regex::regex_template;

struct parser
{
public:
  parser(char start_token, regex_consumer_function consumer)
    : start_tokens_{start_token}
    , consumer_{consumer}
  {
    //
  }

  parser(std::initializer_list<char> start_tokens, regex_consumer_function consumer)
    : start_tokens_{start_tokens}
    , consumer_{consumer}
  {
    //
  }

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
//  template<bool truncate=true>
  std::vector<regex_variant> operator()(regex_param& param) const
  {
    std::vector<regex_variant> nodes;

    if(param.regex.size() > param.consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(param.regex[param.consumed] == start_token)
        {
          ++param.consumed;
          nodes = consumer_(param);

          if(nodes.empty()) {
            --param.consumed;
            throw 1; // TODO throw if start token was found but consumption did not take place
          }

          break;
        }
      }
    }

    return nodes;
  }

private:
  std::vector<char> start_tokens_;
  regex_consumer_function consumer_;
};

struct and_parser
{
public:
  and_parser(std::initializer_list<parser> and_parsers, char terminator,
        regex_consumer_function else_parser = nullptr)
    : parsers_{and_parsers}, terminator_{terminator}, else_parser_{else_parser} {}

  // TODO this should be optional return value
  std::vector<regex_variant> operator()(regex_param& param) const
  {
    std::vector<regex_variant> nodes;

    while(param.regex.size() > param.consumed && param.regex[param.consumed] != terminator_)
    {
    
      bool found = false;
      for(auto& p : parsers_)
      {
        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto tmp = p(param);
        if(!nodes.empty())
        {
          nodes.insert(nodes.end(), tmp.begin(), tmp.end()); // TODO move...
          found = true;
          break;
        }
      }

      if(!found)
      {
        if(else_parser_)
        {
          auto tmp = else_parser_(param);
          nodes.insert(nodes.end(), tmp.begin(), tmp.end()); // TODO check if empty is returned... TODO move...
        }
        else
        {
          //throw 1; // TODO throw no match found exception

          throw 1;
        }
      }
    }

    if(param.regex.size() == param.consumed && param.regex[param.consumed-1] != terminator_)
      throw 1; // handle case when terminator is not reached (TODO delete...)

    return nodes;
  }

private:
  std::vector<parser> parsers_;
  regex_consumer_function else_parser_;
  char terminator_;
};

struct or_parser
{
public:
  or_parser(std::initializer_list<parser> or_parsers,
      regex_consumer_function else_parser = nullptr
    ) : parsers_{or_parsers}, else_parser_{else_parser} {}

  std::vector<regex_variant> operator()(regex_param& param) const
  {
    if(param.regex.size() == param.consumed)
      throw 1; // error should contain one more character

    std::vector<regex_variant> nodes;

    for(auto& p : parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto nodes = p(param);
      if(!nodes.empty())
        return nodes;
    }

    if(else_parser_)
      return else_parser_(param);

    throw 1; // TODO throw no match found exception
  }
private:
  std::vector<parser> parsers_;
  regex_consumer_function else_parser_;
};

std::vector<regex_variant> parser_term(regex_param& param);
std::vector<regex_variant> parser_factor(regex_param& param);
std::vector<regex_variant> parser_base(regex_param& param);
#include <iostream>
// <regex> ::= <term> '|' <regex> | <term>
std::vector<regex_variant> parser_regex(regex_param& param)
{
  std::vector<regex_variant> nodes{or_regex_node_d{}}; // TODO remove or node if single node
  ++param.last_index;

  while(param.consumed < param.regex.size())
  {
    if(param.regex[param.consumed] == '|')
      ++param.last_index;

    auto others = (param.regex[param.consumed] != '|' ? parser_term(param) : std::vector<regex_variant>{empty_node_d{}}); // term or empty node

    if(others.empty())
    {
      // ERROR
      nodes.clear();
      break;
    }

    std::move(others.begin(), others.end(), std::back_inserter(nodes));
    std::get<or_regex_node_d>(nodes[0]).append(others.size());

    if(param.consumed == param.regex.size() || param.regex[param.consumed] != '|')
      break;

    ++param.consumed; // consume |
  }

  return nodes;
}

// <term> ::= { <factor> }
std::vector<regex_variant> parser_term(regex_param& param)
{
//  auto nodes = parser_factor(param);

  std::vector<regex_variant> nodes{group_regex_node_d{}}; // TODO remove group node if single node
  ++param.last_index;

  while(param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
    && param.regex[param.consumed] != '|')
  {
    auto next = parser_factor(param);
    std::size_t tmp = nodes.size() - 1;
    std::move(next.begin(), next.end(), std::back_inserter(nodes));
    std::get<group_regex_node_d>(nodes[0]).append();
  }

  return nodes;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
std::vector<regex_variant> parser_factor(regex_param& param)
{
  if(param.regex.size() == param.consumed) // TODO I could probably delete this now
  {
    ++param.last_index;
    return std::vector<regex_variant>{empty_node_d{}}; // empty or
  }

  auto nodes = parser_base(param);

  auto repeat_range_zero = parser{'*', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);

          ++param.last_index;
          return std::vector<regex_variant>{repeat_range_regex_node_d{0}};
        }};
  auto repeat_range_one = parser{'+', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);

          ++param.last_index;
          return std::vector<regex_variant>{repeat_range_regex_node_d{1}};
        }};
  auto optional_item = parser{'?', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // star must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);

          ++param.last_index;
          return std::vector<regex_variant>{optional_regex_node_d{}};
        }};

  repeat_range_zero(param);
  repeat_range_one(param);
  optional_item(param);

  auto digit_parser = [](regex_param& param){
        std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed); // TODO find in range 0-9 would be nice...

        if(end == std::string_view::npos || end == param.consumed)
          throw 1; // TODO exception

        int digit = std::stoi(std::string(param.regex.substr(param.consumed, end)));

        if(digit < 0)
          throw 1; // TODO exception

        param.consumed = end;

        return digit;
      };

  if(param.consumed < param.regex.size() && param.regex[param.consumed] == '{')
  {
    ++param.consumed; // consume {
    std::size_t num = 0;
    if(param.consumed < param.regex.size())
    {
      if(param.regex[param.consumed] != ',')
        num = digit_parser(param);
      else
        /// TODO handle exception
        throw 1; // syntax x{,y} is not supported
    }
    else
      throw 1; // TODO exception - unexpected end of regex

    if(param.consumed < param.regex.size() && param.regex[param.consumed] == ',')
    {
      if(param.regex.size() > ++param.consumed)
      {
        if(param.regex[param.consumed] == '}')
        {
          ++param.last_index;
          nodes.insert(nodes.begin(), repeat_range_regex_node_d{num});
        }
        else
        {
          ++param.last_index;
          nodes.insert(nodes.begin(), repeat_range_regex_node_d{num, digit_parser(param)});
        }
      }
      else
        /// TODO error handling
        throw 1;
    }
    else if(param.regex.size() > param.consumed && param.regex[param.consumed] == '}')
    {
      ++param.last_index;
      nodes.insert(nodes.begin(), repeat_regex_node_d{num});
    }
    else
      /// TODO error handling
      throw 1;

    ++param.consumed; // consume }
  }

  return nodes;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
std::vector<regex_variant> parser_base(regex_param& param)
{
  auto form_feed = parser{'f', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{literal_regex_node_d{'\f'}};
    }};
  auto new_line = parser{'n', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{literal_regex_node_d{'\n'}};
    }};
  auto carriage_return = parser{'r', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{literal_regex_node_d{'\r'}};
    }};
  auto horizontal_tab = parser{'t', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{literal_regex_node_d{'\t'}};
    }};
  auto vertical_tab = parser{'v', [](regex_param& param){
      return std::vector<regex_variant>{literal_regex_node_d{'\v'}};
    }};
  auto any_digit = parser{'d', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{range_random_regex_node_d{'0', '9'}};
    }};
  auto null_character = parser{'0', [](regex_param& param){
      ++param.last_index;
      return std::vector<regex_variant>{literal_regex_node_d{'\0'}};
    }};
  auto any_non_digit = parser{'D', [](regex_param& param){
          param.last_index += 3;
          return std::vector<regex_variant>{or_regex_node_d{{1, 1}},
              range_random_regex_node_d{ascii_min, '0' - 1},
              range_random_regex_node_d{'9' + 1, ascii_max}};
        }};
  auto any_whitespace = parser{'s', [](regex_param& param){
          ++param.last_index;
          return std::vector<regex_variant>{whitespace_regex_node_d{}};
        }};
  auto any_non_whitespace = parser{'S', [](regex_param& param){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          param.last_index += 4;
          return std::vector<regex_variant>{or_regex_node_d{{1, 1, 1}},
              range_random_regex_node_d{ascii_min, '\t' - 1},
              range_random_regex_node_d{'\r' + 1, ' ' - 1},
              range_random_regex_node_d{' ' + 1, ascii_max}};
        }};
  auto any_alphanum_or_underscore = parser{'w', [](regex_param& param) // any alphanumeric characters or _
        {
          param.last_index += 4;
          return std::vector<regex_variant>{or_regex_node_d{{1, 1, 1}},
              range_random_regex_node_d{'a', 'z'},
              range_random_regex_node_d{'0', '9'},
              literal_regex_node_d{'_'}};
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](regex_param& param){
          param.last_index += 6;
          return std::vector<regex_variant>{or_regex_node_d{{1, 1, 1, 1, 1}},
              range_random_regex_node_d{ascii_min, '0' - 1},
              range_random_regex_node_d{'9' + 1, 'A' - 1},
              range_random_regex_node_d{'Z' + 1, '_' - 1},
              range_random_regex_node_d{'_' + 1, 'a' - 1},
              range_random_regex_node_d{'z' + 1, ascii_max}};
        }};
  auto hexadecimal_ascii_character_representation = parser{'x', [](regex_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw 1;

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw 1;

          param.consumed += 2;

          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d(static_cast<char>(ascii_hex))};
        }};
  auto word_boundary = parser{'b', [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \b doesn't have a value after it - not valid
            throw 1;

          if(param.consumed - 2 == 0) // case when \b doesn't have a value before it - not valid
            throw 1;

          if(!::isspace(param.regex[param.consumed - 3]) && !::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          return std::vector<regex_variant>{empty_node_d()}; // TODO remove dummy node as it is useles
        }};
  auto not_word_boundary = parser{'B', [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \B doesn't have a value after it - could cause a partial match
            throw 1;

          if(param.consumed - 2 == 0) // case when \B doesn't have a value before it - could cause a partial match
            throw 1;

          if(::isspace(param.regex[param.consumed - 3]) || ::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          return std::vector<regex_variant>{empty_node_d()}; // TODO remove dummy node as it is useles
        }};
  auto backreference_parser = parser{{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        [](regex_param& param){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(std::string(param.regex.substr(param.consumed-1, end)));

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == -1)
            throw 1; // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          ++param.last_index;
          return std::vector<regex_variant>{captured_group_reference_node_d{param.captured_groups[digit - 1]}};
        }};


  or_parser parser_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      any_digit,
      any_non_digit,
      any_whitespace,
      any_non_whitespace,
      any_alphanum_or_underscore,
      any_not_alphanum_or_underscore,
      hexadecimal_ascii_character_representation,
      word_boundary,
      not_word_boundary,
      null_character,
      backreference_parser},
      [](regex_param& param){
          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d{param.regex[param.consumed++]}}; // escaped_literal_char
        }
    };

  or_parser escaped_or_literal {
      {parser{'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}}},
      [](regex_param& param){
          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d{param.regex[param.consumed++]}};
        }
    };

  or_parser parse_end_range_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      hexadecimal_ascii_character_representation},
      [](regex_param& param){
          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d{param.regex[param.consumed++]}}; // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)
        }
    };

  or_parser end_range_escaped_or_literal {
      {parser{'\\', [parse_end_range_escaped](regex_param& param){return parse_end_range_escaped(param);}}},
      [](regex_param& param){
          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d{param.regex[param.consumed++]}};
        }
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](regex_param& param)
      {
        auto tmp_nodes = escaped_or_literal(param);
        if(tmp_nodes.empty())
        {
          /// TODO exception handling
          throw 1;
        }

        if(param.consumed+2 < param.regex.size() && param.regex[param.consumed] == '-' && param.regex[param.consumed+1] != ']')
        {
          ++param.consumed;

          if(param.consumed+1 >= param.regex.size())
          {
            /// TODO exception handling
            throw 1;
          }

          auto tmp = end_range_escaped_or_literal(param);
          if(tmp.empty())
          {
            /// TODO exception handling
            throw 1;
          }

          ++param.last_index;
          tmp_nodes = std::vector<regex_variant>{range_random_regex_node_d{
                                                  std::get<literal_regex_node_d>(tmp_nodes.back()).getLiteral(),
                                                  std::get<literal_regex_node_d>(tmp.back()).getLiteral()}}; // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
        }

        return tmp_nodes;
      }
    };

  or_parser range_or_negated_range {
      {parser{'^', [range_parser](regex_param& param){
          auto tmp = range_parser(param);

          param.last_index -= tmp.size();
          // TODO check if range parser returns an empty vector

          std::vector<regex_variant> invert;
          std::vector<std::pair<char, char>> ranges;

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
          }
          else
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
          }

          std::vector<std::size_t> lenghts;
          for(std::size_t i = 0; i<invert.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.consumed; // consume ]
          invert.insert(invert.begin(), or_regex_node_d{lenghts});
          param.last_index += invert.size();
          return invert;
        }}},
      [range_parser](regex_param& param){
          auto tmp_vec = range_parser(param);
          std::vector<std::size_t> lenghts;
          for(std::size_t i = 0; i<tmp_vec.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.last_index;
          tmp_vec.insert(tmp_vec.begin(), or_regex_node_d{lenghts}); // TODO check if range parser returns an empty vector
          
          ++param.consumed; // consume ]
          return tmp_vec;
        }
    };

  or_parser parser_base_type{
      {
        parser{'(', [](regex_param& param)
          {
            if(param.regex.size() == param.consumed + 1) // handle () case
            {
              if(param.regex[param.consumed] == ')')
              {
                ++param.consumed;
                ++param.last_index;
                return std::vector<regex_variant>{empty_node_d{}}; // empty
              }
              else
                throw 1; // not enough characters...
            }

            std::size_t capture_index = param.last_index;
            int index = -1;
            if(param.regex[param.consumed] == '?')
            {
              // (?:...) is a non capturing group
              if(param.regex.size() > param.consumed + 2 && param.regex[param.consumed+1] == ':')
              {
                param.consumed += 2; // discard matches everything enclosed as that's exactly what will be generated anyway

                if(param.regex[param.consumed] == ')') // handle (?:) case
                {
                  ++param.consumed;
                  ++param.last_index;
                  return std::vector<regex_variant>{empty_node_d{}}; // empty
                }
              }
              else
                throw 1; // not enough characters
            }
            else
            {
              param.captured_groups.push_back(-1);
              index = param.captured_groups.size() - 1;
            }

            auto nodes = parser_regex(param);
            if(param.regex.size() == param.consumed)
              throw 1; // missing closing bracket
            ++param.consumed;

            if(index != -1)
              param.captured_groups[index] = capture_index;

            return nodes;
          }}, // TODO, ')'},
        parser{'[', [range_or_negated_range](regex_param& param){return range_or_negated_range(param);}},
        parser{'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}},
        parser{'.', [](regex_param& param){
            ++param.last_index;
            return std::vector<regex_variant>{random_regex_node_d{}};
          }}
      },
      [](regex_param& param){
          if(param.regex[param.consumed] == '$' && param.regex.size() == param.consumed + 1) // handle end of text symbol (TODO add random text generation if $ is missing at the end - check if OK)
          {
            ++param.consumed;
            ++param.last_index;
            return std::vector<regex_variant>{empty_node_d{}};
          }

          ++param.last_index;
          return std::vector<regex_variant>{literal_regex_node_d{param.regex[param.consumed++]}};
        }
    };

  return parser_base_type(param);
}
#include <iostream> // TODO delete
regex_template::regex_template(std::string_view regex)
{
  if(regex.size())
  {
    std::size_t consumed = (regex[0] == '^'); // set to 0 if regex doesn't start with start of text symbol (^) otherwise start with 1 (TODO add random text generation if ^ is missing at the beginning - check if OK)

    if(!consumed || regex.size() != 1)
    {
      regex_param param{regex};
      param.consumed = consumed;
      nodes_ = parser_regex(param);

      std::size_t i = 0;

#ifdef RANDOM_REGEX_DEBUG
      for(auto& node : nodes_)
        std::cout << node->name() << "   " << i++ << '\n';

      std::cout << '\n';
#endif

      /// TODO in some cases this is also true: regex.size() < consumed (check why???)
      if(param.regex.size() > param.consumed)
        // TODO throw invalid regex exception
        throw 1;
    }
  }
}

regex_template::~regex_template() = default;

void regex_template::generate(std::ostream& os)
{
  random_generator<> rand; // TODO should be provided from the outside

//---------------------------------
/*  regex_variant testv_{whitespace_regex_node_()};
std::cout << '\'';
std::visit([&](auto&& n){std::stringstream ss; n.generate(ss, rand); std::cout << ss.str();}, testv_);*/
/*  auto& tmp_n = std::get<whitespace_regex_node_>(testv_);
  std::stringstream ss;
  tmp_n.generate(ss, rand);*/
//    std::cout << "v.index = " << v.index() << '\n';
//std::cout << "'\n";
//---------------------------------
  std::size_t len = 0;

  if(len < nodes_.size()) // TODO this if could be changed for dummy regex node
  {
// FIXME error: ‘this’ was not captured for this lambda function (probably something to do with this http://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function)
//    len += std::visit([&nodes_, &len, &os, &rand](auto&& node){return std::remove_reference_t<decltype(node)>::generator::generate(nodes_, len, os, rand);}, nodes_[len]);
    len += std::visit([&](auto&& node){return std::remove_reference_t<decltype(node)>::generator::generate(nodes_, len, os, rand);}, nodes_[len]);
  }
}
