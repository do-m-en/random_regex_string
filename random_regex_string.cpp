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
  regex_param(std::string_view regex_, std::vector<regex_variant>& nodes_) : regex{regex_}, nodes{nodes_} {}

  std::string_view regex;
  std::vector<regex_variant>& nodes;
  std::size_t consumed = 0;
  std::vector<int> captured_groups;

  std::size_t last_index = 0;
};

using regex_consumer_function = std::function<bool (regex_param&)>;

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
  bool operator()(regex_param& param) const
  {
    bool found = false;

    if(param.regex.size() > param.consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(param.regex[param.consumed] == start_token)
        {
          ++param.consumed;
          auto res = consumer_(param);

          if(!res) {
            --param.consumed;
            throw 1; // TODO throw if start token was found but consumption did not take place
          }

          found = true;

          break;
        }
      }
    }

    return found;
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
  bool operator()(regex_param& param) const
  {
    while(param.regex.size() > param.consumed && param.regex[param.consumed] != terminator_)
    {
      bool found = false;
      for(auto& p : parsers_)
      {
        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto found = p(param);
        if(found)
        {
          break;
        }
      }

      if(!found)
      {
        if(!else_parser_ || !else_parser_(param))
        {
          //throw 1; // TODO throw no match found exception

          throw 1;
        }
      }
    }

    if(param.regex.size() == param.consumed && param.regex[param.consumed-1] != terminator_)
      throw 1; // handle case when terminator is not reached (TODO delete...)

    return true;
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

  bool operator()(regex_param& param) const
  {
    if(param.regex.size() == param.consumed)
      throw 1; // error should contain one more character

    for(auto& p : parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      if(p(param))
        return true;
    }

    if(else_parser_)
      return else_parser_(param);

    throw 1; // TODO throw no match found exception
  }
private:
  std::vector<parser> parsers_;
  regex_consumer_function else_parser_;
};

bool parser_term(regex_param& param);
bool parser_factor(regex_param& param);
bool parser_base(regex_param& param);
#include <iostream>
// <regex> ::= <term> '|' <regex> | <term>
bool parser_regex(regex_param& param)
{
  param.nodes.emplace_back(or_regex_node_d{}); // TODO remove empty or case
  auto pos = param.nodes.size() - 1;
  auto prev_len = 0;
  ++param.last_index;

  while(param.consumed < param.regex.size())
  {
    if(param.regex[param.consumed] == '|')
      ++param.last_index;

    if(param.regex[param.consumed] != '|')
    {
      if(!parser_term(param))
      {
        // ERROR
        throw 1;
      }
    }
    else
    {
      param.nodes.emplace_back(empty_node_d{});
    }

    prev_len = param.nodes.size() - 1 - pos - prev_len;

    std::get<or_regex_node_d>(param.nodes[pos]).append(prev_len);

    if(param.consumed == param.regex.size() || param.regex[param.consumed] != '|')
      break;

    ++param.consumed; // consume |
  }

  return true;
}

// <term> ::= { <factor> }
bool parser_term(regex_param& param)
{
  param.nodes.emplace_back(group_regex_node_d{}); // TODO remove group node if single node
  auto pos = param.nodes.size() - 1;
  ++param.last_index;

  while(param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
    && param.regex[param.consumed] != '|')
  {
    parser_factor(param);
    std::get<group_regex_node_d>(param.nodes[pos]).append();
  }

  return true;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
bool parser_factor(regex_param& param)
{
  if(param.regex.size() == param.consumed) // TODO I could probably delete this now
  {
    ++param.last_index;
    param.nodes.emplace_back(empty_node_d{}); // empty or

    return true;
  }

  auto pos = param.nodes.size(); // one past the previous node as we expect new nodes...
  parser_base(param);

  auto repeat_range_zero = parser{'*', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);

          ++param.last_index;
//FIXME          param.nodes.emplace_back(repeat_range_regex_node_d{0});
          return true;
        }};
  auto repeat_range_one = parser{'+', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);

          ++param.last_index;
//FIXME          param.nodes.emplace_back(repeat_range_regex_node_d{1});

          return true;
        }};
  auto optional_item = parser{'?', [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // star must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);

          ++param.last_index;
//FIXME          param.nodes.emplace_back(optional_regex_node_d{});

          return true;
        }};

// FIXME not in use!
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
          param.nodes.insert(param.nodes.begin() + pos, repeat_range_regex_node_d{num});
        }
        else
        {
          ++param.last_index;
          param.nodes.insert(param.nodes.begin() + pos, repeat_range_regex_node_d{num, digit_parser(param)});
        }
      }
      else
        /// TODO error handling
        throw 1;
    }
    else if(param.regex.size() > param.consumed && param.regex[param.consumed] == '}')
    {
      ++param.last_index;
      param.nodes.insert(param.nodes.begin() + pos, repeat_regex_node_d{num});
    }
    else
      /// TODO error handling
      throw 1;

    ++param.consumed; // consume }
  }

  return true;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
bool parser_base(regex_param& param)
{
  auto form_feed = parser{'f', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\f'});

      return true;
    }};
  auto new_line = parser{'n', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\n'});

      return true;
    }};
  auto carriage_return = parser{'r', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\r'});

      return true;
    }};
  auto horizontal_tab = parser{'t', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\t'});

      return true;
    }};
  auto vertical_tab = parser{'v', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\v'});

      return true;
    }};
  auto any_digit = parser{'d', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(range_random_regex_node_d{'0', '9'});

      return true;
    }};
  auto null_character = parser{'0', [](regex_param& param){
      ++param.last_index;
      param.nodes.emplace_back(literal_regex_node_d{'\0'});

      return true;
    }};
  auto any_non_digit = parser{'D', [](regex_param& param){
          param.last_index += 3;
          param.nodes.emplace_back(or_regex_node_d{{1, 1}});
          param.nodes.emplace_back(range_random_regex_node_d{ascii_min, '0' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'9' + 1, ascii_max});

          return true;
        }};
  auto any_whitespace = parser{'s', [](regex_param& param){
          ++param.last_index;
          param.nodes.emplace_back(whitespace_regex_node_d{});

          return true;
        }};
  auto any_non_whitespace = parser{'S', [](regex_param& param){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          param.last_index += 4;
          param.nodes.emplace_back(or_regex_node_d{{1, 1, 1}});
          param.nodes.emplace_back(range_random_regex_node_d{ascii_min, '\t' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'\r' + 1, ' ' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{' ' + 1, ascii_max});

          return true;
        }};
  auto any_alphanum_or_underscore = parser{'w', [](regex_param& param) // any alphanumeric characters or _
        {
          param.last_index += 4;
          param.nodes.emplace_back(or_regex_node_d{{1, 1, 1}});
          param.nodes.emplace_back(range_random_regex_node_d{'a', 'z'});
          param.nodes.emplace_back(range_random_regex_node_d{'0', '9'});
          param.nodes.emplace_back(literal_regex_node_d{'_'});

          return true;
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](regex_param& param){
          param.last_index += 6;
          param.nodes.emplace_back(or_regex_node_d{{1, 1, 1, 1, 1}});
          param.nodes.emplace_back(range_random_regex_node_d{ascii_min, '0' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'9' + 1, 'A' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'Z' + 1, '_' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'_' + 1, 'a' - 1});
          param.nodes.emplace_back(range_random_regex_node_d{'z' + 1, ascii_max});

          return true;
        }};
  auto hexadecimal_ascii_character_representation = parser{'x', [](regex_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw 1;

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw 1;

          param.consumed += 2;

          ++param.last_index;
          param.nodes.emplace_back(literal_regex_node_d{static_cast<char>(ascii_hex)});

          return true;
        }};
  auto word_boundary = parser{'b', [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \b doesn't have a value after it - not valid
            throw 1;

          if(param.consumed - 2 == 0) // case when \b doesn't have a value before it - not valid
            throw 1;

          if(!::isspace(param.regex[param.consumed - 3]) && !::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          param.nodes.emplace_back(empty_node_d()); // TODO remove dummy node as it is useles

          return true;
        }};
  auto not_word_boundary = parser{'B', [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \B doesn't have a value after it - could cause a partial match
            throw 1;

          if(param.consumed - 2 == 0) // case when \B doesn't have a value before it - could cause a partial match
            throw 1;

          if(::isspace(param.regex[param.consumed - 3]) || ::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          param.nodes.emplace_back(empty_node_d()); // TODO remove dummy node as it is useles

          return true;
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
          param.nodes.emplace_back(captured_group_reference_node_d{param.captured_groups[digit - 1]});

          return true;
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
          param.nodes.emplace_back(literal_regex_node_d{param.regex[param.consumed++]}); // escaped_literal_char

          return true;
        }
    };

  or_parser escaped_or_literal {
      {parser{'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}}},
      [](regex_param& param){
          ++param.last_index;
          param.nodes.emplace_back(literal_regex_node_d{param.regex[param.consumed++]});

          return true;
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
          param.nodes.emplace_back(literal_regex_node_d{param.regex[param.consumed++]}); // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)

          return true;
        }
    };

  or_parser end_range_escaped_or_literal {
      {parser{'\\', [parse_end_range_escaped](regex_param& param){return parse_end_range_escaped(param);}}},
      [](regex_param& param){
          ++param.last_index;
          param.nodes.emplace_back(literal_regex_node_d{param.regex[param.consumed++]});

          return true;
        }
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](regex_param& param)
      {
        auto pos = param.nodes.size();
        escaped_or_literal(param); // TODO check if it returns bool == false so we don't need pos
        if(pos == param.nodes.size())
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

          pos = param.nodes.size();
          end_range_escaped_or_literal(param); // TODO check if it returns bool == false so we don't need pos
          if(pos == param.nodes.size())
          {
            /// TODO exception handling
            throw 1;
          }

          ++param.last_index;
          char literal_1 = std::get<literal_regex_node_d>(param.nodes.back()).getLiteral();
          param.nodes.pop_back(); // TODO just keep the old nodes...
          char literal_2 = std::get<literal_regex_node_d>(param.nodes.back()).getLiteral();
          param.nodes.back() = range_random_regex_node_d{literal_2, literal_1}; // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_ or preserve the two nodes and access them by id/pointer
        }

        return true;
      }
    };

  or_parser range_or_negated_range {
      {parser{'^', [range_parser](regex_param& param){
          auto pos = param.nodes.size();
          param.nodes.emplace_back(empty_node_d{}); // placeholder
          range_parser(param);

          param.last_index -= param.nodes.size() - pos;
          // TODO check if range parser returns an empty vector

          std::vector<std::pair<char, char>> ranges;

          // convert
          for(std::size_t i = pos + 1; i<param.nodes.size(); ++i)
          {
            if(auto lit = std::get_if<literal_regex_node_d>(&param.nodes[i]))
              ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
            else
            {
              auto range = std::get<range_random_regex_node_d>(param.nodes[i]);
              ranges.emplace_back(range.get_from(), range.get_to());
            }
          }

          param.nodes.erase(param.nodes.begin() + pos, param.nodes.end());

          // handle single range case
          if(ranges.size() == 1)
          {
            // TODO merge
            if(ranges[0].first != ascii_min && ranges[0].second != ascii_max)
            {
              param.nodes.emplace_back(range_random_regex_node_d{ascii_min, ranges[0].first - 1});
              param.nodes.emplace_back(range_random_regex_node_d{ranges[0].second + 1, ascii_max});
            }
            else if(ranges[0].first == ascii_min && ranges[0].second == ascii_max)
              // in rare case where no characters are allowed throw an exception as regex is faulty...
              throw 1; // TODO exception
            else if(ranges[0].first == ascii_min)
              param.nodes.emplace_back(range_random_regex_node_d{ranges[0].second + 1, ascii_max});
            else
              param.nodes.emplace_back(range_random_regex_node_d{ascii_min, ranges[0].first - 1});
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
                    param.nodes.emplace_back(range_random_regex_node_d{min, cur.first - 1});
                    param.nodes.emplace_back(range_random_regex_node_d{cur.second + 1, ascii_max});
                  }
                  else if(cur.first == ascii_min && cur.second == ascii_max)
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  else if(cur.first == ascii_min)
                    param.nodes.emplace_back(range_random_regex_node_d{cur.second + 1, ascii_max});
                  else
                    param.nodes.emplace_back(range_random_regex_node_d{min, cur.first - 1});

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
                    param.nodes.emplace_back(range_random_regex_node_d{min, cur.first - 1});
                    param.nodes.emplace_back(range_random_regex_node_d{next.second + 1, ascii_max});
                  }
                  else if(cur.first == ascii_min && next.second == ascii_max)
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  else if(cur.first == ascii_min)
                    param.nodes.emplace_back(range_random_regex_node_d{next.second + 1, ascii_max});
                  else
                    param.nodes.emplace_back(range_random_regex_node_d{min, cur.first - 1});

                  break;
                }

                cur.second = next.second;
              }
              else // ranges aren't connected
              {
                if(cur.first != ascii_min && cur.second != ascii_max)
                {
                  param.nodes.emplace_back(range_random_regex_node_d{min, static_cast<char>(cur.first - 1)});
                  param.nodes.emplace_back(range_random_regex_node_d{static_cast<char>(cur.second + 1), static_cast<char>(next.first - 1)});
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
          for(std::size_t i = pos + 1; i<param.nodes.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.consumed; // consume ]
          param.nodes[pos] = or_regex_node_d{lenghts};
          param.last_index += lenghts.size(); // TODO simplify...
          return true;
        }}},
      [range_parser](regex_param& param){
          auto pos = param.nodes.size();
          param.nodes.emplace_back(empty_node_d{}); // placeholder
          range_parser(param);
          std::vector<std::size_t> lenghts;
          for(std::size_t i = pos + 1; i<param.nodes.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.last_index;
          param.nodes[pos] = or_regex_node_d{lenghts}; // TODO check if range parser returns an empty vector
          
          ++param.consumed; // consume ]
          return true;
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
                param.nodes.emplace_back(empty_node_d{}); // empty

                return true;
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
                  param.nodes.emplace_back(empty_node_d{}); // empty

                  return true;
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

            parser_regex(param);
            if(param.regex.size() == param.consumed)
              throw 1; // missing closing bracket
            ++param.consumed;

            if(index != -1)
              param.captured_groups[index] = capture_index;

            return true;
          }}, // TODO, ')'},
        parser{'[', [range_or_negated_range](regex_param& param){return range_or_negated_range(param);}},
        parser{'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}},
        parser{'.', [](regex_param& param){
            ++param.last_index;
            param.nodes.emplace_back(random_regex_node_d{});

            return true;
          }}
      },
      [](regex_param& param){
          if(param.regex[param.consumed] == '$' && param.regex.size() == param.consumed + 1) // handle end of text symbol (TODO add random text generation if $ is missing at the end - check if OK)
          {
            ++param.consumed;
            ++param.last_index;
            param.nodes.emplace_back(empty_node_d{});

            return true;
          }

          ++param.last_index;
          param.nodes.emplace_back(literal_regex_node_d{param.regex[param.consumed++]});

          return true;
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
      regex_param param{regex, nodes_};
      param.consumed = consumed;
      parser_regex(param);

      std::size_t i = 0;

#ifdef RANDOM_REGEX_DEBUG
      for(auto& node : nodes_)
        std::cout << std::visit([](auto&& node){return node.name();}, node) << "   " << i++ << '\n';

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

#ifdef RANDOM_REGEX_DEBUG
  std::cout << "-----\n";
#endif

  if(len < nodes_.size()) // TODO this if could be changed for dummy regex node
  {
// FIXME error: ‘this’ was not captured for this lambda function (probably something to do with this http://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function)
//    len += std::visit([&nodes_, &len, &os, &rand](auto&& node){return std::remove_reference_t<decltype(node)>::generator::generate(nodes_, len, os, rand);}, nodes_[len]);
    len += std::visit([&](auto&& node){return std::remove_reference_t<decltype(node)>::generator::generate(nodes_, len, os, rand);}, nodes_[len]);
  }
}
