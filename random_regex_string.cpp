#include "random_regex_string.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <functional>
#include <stdexcept>

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

using rand_regex::captured_group_reference_node_;
using rand_regex::group_regex_node_;
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

struct regex_param
{
  regex_param(std::experimental::string_view regex_) : regex{regex_} {}

  std::experimental::string_view regex;
  std::size_t consumed = 0;
  std::vector<regex_node_*> captured_groups;
};

using regex_consumer_function = std::function<regex_node_* (regex_param&)>;

// class regex_template
using rand_regex::regex_template;

struct parser
{
public:
  parser(char start_token, regex_consumer_function consumer)
    : start_tokens_{{start_token}}
    , consumer_{consumer}
  {
    //
  }

  parser(std::vector<char> start_tokens, regex_consumer_function consumer)
    : start_tokens_{std::forward<std::vector<char>>(start_tokens)}
    , consumer_{consumer}
  {
    //
  }

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
  regex_node_* operator()(regex_param& param) const
  {
    regex_node_* node = nullptr;

    if(param.regex.size() > param.consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(param.regex[param.consumed] == start_token)
        {
          ++param.consumed;
          node = consumer_(param);

          if(!node)
          {
            --param.consumed;
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed));
          }

          break;
        }
      }
    }

    return node;
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
  std::vector<regex_node_*> operator()(regex_param& param) const
  {
    std::vector<regex_node_*> nodes;

    while(param.regex.size() > param.consumed && param.regex[param.consumed] != terminator_)
    {
      bool found = false;
      for(auto& p : parsers_)
      {
        if(auto node = p(param); node)
        {
          nodes.push_back(node);
          found = true;
          break;
        }
      }

      if(!found)
      {
        if(else_parser_)
          nodes.push_back(else_parser_(param)); // TODO check if null is returned...
        else
        {
          for(auto node : nodes)
            delete node;

          nodes.clear();

          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // no match found
        }
      }
    }

    if(param.regex.size() == param.consumed && param.regex[param.consumed-1] != terminator_)
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // terminator not found

    return nodes;
  }

private:
  std::vector<parser> parsers_;
  regex_consumer_function else_parser_; // TODO define signature...
  char terminator_;
};

struct or_parser
{
public:
  or_parser(std::initializer_list<parser> or_parsers,
      regex_consumer_function else_parser = nullptr
    ) : or_parsers_{or_parsers}, else_parser_{else_parser} {}

  regex_node_* operator()(regex_param& param) const
  {
    if(param.regex.size() == param.consumed)
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // should contain one more character

    for(auto& p : or_parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto node = p(param);
      if(node)
        return node;
    }

    if(else_parser_)
      return else_parser_(param);

    throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // no match found
  }
private:
  std::vector<parser> or_parsers_;
  regex_consumer_function else_parser_;
};

regex_node_* parser_term(regex_param& param);
regex_node_* parser_factor(regex_param& param);
regex_node_* parser_base(regex_param& param);

// <regex> ::= <term> '|' <regex> | <term>
regex_node_* parser_regex(regex_param& param)
{
  if(param.regex[param.consumed] == '|')
  {
    ++param.consumed;
  }

  regex_node_* node = parser_term(param);

  if(param.consumed < param.regex.size() && param.regex[param.consumed] == '|')
  {
    ++param.consumed; // consume |
    regex_node_* other = parser_regex(param);
    node = new or_regex_node_{node, other};
  }

  return node;
}

// <term> ::= { <factor> }
regex_node_* parser_term(regex_param& param)
{
  regex_node_* node = parser_factor(param);

  while(param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
    && param.regex[param.consumed] != '|')
  {
    regex_node_* next = parser_factor(param);
    node = new group_regex_node_(std::vector<regex_node_*>{node, next}); // TODO consider renaming it to sequence
  }

  return node;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
regex_node_* parser_factor(regex_param& param)
{
  if(param.regex.size() == param.consumed)
  {
    return new regex_node_{}; // empty or
  }

  regex_node_* node = parser_base(param);

  auto repeat_range_zero = parser{'*', [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);
          return new repeat_range_regex_node_(node, 0);
        }};
  auto repeat_range_one = parser{'+', [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);
          return new repeat_range_regex_node_(node, 1);
        }};
  auto optional_item = parser{'?', [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);
          return new optional_regex_node_(node);
        }};

  repeat_range_zero(param);
  repeat_range_one(param);
  optional_item(param);

  auto digit_parser = [](regex_param& param){
        std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed); // TODO find in range 0-9 would be nice...

        if(end == std::experimental::string_view::npos || end == param.consumed)
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // number was expected

        int digit = std::stoi(param.regex.substr(param.consumed, end).to_string());

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
        throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // syntax x{,y} is not supported in C++ (works as literal in online regex tester)
    }
    else
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // exception - unexpected end of regex

    if(param.consumed < param.regex.size() && param.regex[param.consumed] == ',')
    {
      if(param.regex.size() > ++param.consumed)
      {
        if(param.regex[param.consumed] == '}')
          node = new repeat_range_regex_node_(node, num);
        else
          node = new repeat_range_regex_node_(node, num, digit_parser(param));
      }
      else
        throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // segment not closed but end of regex reached
    }
    else if(param.regex.size() > param.consumed && param.regex[param.consumed] == '}')
      node = new repeat_regex_node_(node, num);
    else
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // unexpected character found

    ++param.consumed; // consume }
  }

  return node;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(regex_param& param)
{
  auto any_non_whitespace_node = [](){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '\t' - 1},
                                    new range_random_regex_node_{'\r' + 1, ' ' - 1},
                                    new range_random_regex_node_{' ' + 1, ascii_max}};
        };

  auto form_feed = parser{'f', [](regex_param& param){return new literal_regex_node_{'\f'};}};
  auto new_line = parser{'n', [](regex_param& param){return new literal_regex_node_{'\n'};}};
  auto carriage_return = parser{'r', [](regex_param& param){return new literal_regex_node_{'\r'};}};
  auto horizontal_tab = parser{'t', [](regex_param& param){return new literal_regex_node_{'\t'};}};
  auto vertical_tab = parser{'v', [](regex_param& param){return new literal_regex_node_{'\v'};}};
  auto any_digit = parser{'d', [](regex_param& param){return new range_random_regex_node_{'0', '9'};}};
  auto null_character = parser{'0', [](regex_param& param){return new literal_regex_node_{'\0'};}};
  auto any_non_digit = parser{'D', [](regex_param& param){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                                    new range_random_regex_node_{'9' + 1, ascii_max}};
        }};
  auto any_whitespace = parser{'s', [](regex_param& param){return new whitespace_regex_node_{};}};
  auto any_non_whitespace = parser{'S', [any_non_whitespace_node](regex_param& param){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          return any_non_whitespace_node();
        }};
  auto any_alphanum_or_underscore = parser{'w', [](regex_param& param) // any alphanumeric characters or _
        {
          return new or_regex_node_{new range_random_regex_node_{'A', 'Z'},
                                    new range_random_regex_node_{'a', 'z'},
                                    new range_random_regex_node_{'0', '9'},
                                    new literal_regex_node_{'_'}};
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](regex_param& param){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                                    new range_random_regex_node_{'9' + 1, 'A' - 1},
                                    new range_random_regex_node_{'Z' + 1, '_' - 1},
                                    new range_random_regex_node_{'_' + 1, 'a' - 1},
                                    new range_random_regex_node_{'z' + 1, ascii_max}};
        }};
  auto hexadecimal_ascii_character_representation = parser{'x', [](regex_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // either regex ends too soon or the next two characters are not digits

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // requested char is out of ascii bounds

          param.consumed += 2;

          return new literal_regex_node_(static_cast<char>(ascii_hex));
        }};
  /*auto word_boundary = parser{'b', [any_non_whitespace_node](regex_param& param){ // \b
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // word boundary related regex is not supported - too many cases to handle
          return new regex_node_{}; // will never get here - required only because otherwise compiler complains regarding return statement
        }};
  auto not_word_boundary = parser{'B', [any_non_whitespace_node](regex_param& param){ // \B -> not \b
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // word boundary related regex is not supported - too many cases to handle
          return new regex_node_{}; // will never get here - required only because otherwise compiler complains regarding return statement
        }};*/
  auto backreference_parser = parser{{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        [](regex_param& param){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(param.regex.substr(param.consumed-1, end).to_string());

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == nullptr)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          return new captured_group_reference_node_(param.captured_groups[digit - 1]);
        }};

  auto escaped_escape_parser = parser{'\\', [](regex_param& param){return new literal_regex_node_{'\\'};}};
  auto escaped_optional_parser = parser{'?', [](regex_param& param){return new literal_regex_node_{'?'};}};
  auto escaped_repeat_zero_parser = parser{'*', [](regex_param& param){return new literal_regex_node_{'*'};}};
  auto escaped_repeat_one_parser = parser{'+', [](regex_param& param){return new literal_regex_node_{'+'};}};
  auto escaped_repeat_parser = parser{'{', [](regex_param& param){return new literal_regex_node_{'{'};}};
  auto escaped_group_parser = parser{'[', [](regex_param& param){return new literal_regex_node_{'['};}};
  auto escaped_regex_start_parser = parser{'^', [](regex_param& param){return new literal_regex_node_{'^'};}};
  auto escaped_regex_end_parser = parser{'$', [](regex_param& param){return new literal_regex_node_{'$'};}};

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
      /*word_boundary,
      not_word_boundary,*/
      null_character,
      backreference_parser,

      // escaped special chars
      escaped_escape_parser,
      escaped_optional_parser,
      escaped_repeat_zero_parser,
      escaped_repeat_one_parser,
      escaped_repeat_parser,
      escaped_group_parser,
      escaped_regex_start_parser,
      escaped_regex_end_parser}/*,
      [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);} // escaped_literal_char
      characters that don't need to be escaped should not be supported in escape format...
      */
    };

  or_parser escaped_or_literal {
      {{'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}}},
      [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);}
    };

  or_parser parse_end_range_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      hexadecimal_ascii_character_representation},
      [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);} // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)
    };

  or_parser end_range_escaped_or_literal {
      {{'\\', [parse_end_range_escaped](regex_param& param){return parse_end_range_escaped(param);}}},
      [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);}
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](regex_param& param)
      {
        std::unique_ptr<regex_node_> tmp_node{escaped_or_literal(param)};
        if(!tmp_node.get())
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // node was not created

        if(param.consumed+2 < param.regex.size() && param.regex[param.consumed] == '-' && param.regex[param.consumed+1] != ']')
        {
          literal_regex_node_* literal_node = static_cast<literal_regex_node_*>(tmp_node.get());

          ++param.consumed;

          if(param.consumed+1 >= param.regex.size())
          {
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // group not closed but we're already at the end of regex
          }

          std::unique_ptr<regex_node_> tmp{end_range_escaped_or_literal(param)};
          if(!tmp.get())
          {
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // node was not created
          }

          tmp_node.reset(new range_random_regex_node_(literal_node->getLiteral(), static_cast<literal_regex_node_*>(tmp.get())->getLiteral())); // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
        }

        return tmp_node.release();
      }
    };

  or_parser range_or_negated_range {
      {{'^', [range_parser](regex_param& param){
          auto tmp = range_parser(param);
          // TODO check if range parser returns an empty vector

          std::vector<regex_node_*> invert;
          std::vector<std::pair<char, char>> ranges;

          // convert
          for(auto element : tmp)
          {
            if(auto lit = dynamic_cast<literal_regex_node_*>(element))
              ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
            else if(auto range = dynamic_cast<range_random_regex_node_*>(element))
              ranges.emplace_back(range->get_from(), range->get_to());

            delete element;
          }

          // handle single range case
          if(tmp.size() == 1)
          {
            // TODO merge
            if(ranges[0].first != ascii_min && ranges[0].second != ascii_max)
            {
              invert.push_back(new range_random_regex_node_(ascii_min, ranges[0].first - 1));
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, ascii_max));
            }
            else if(ranges[0].first == ascii_min && ranges[0].second == ascii_max)
              throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // in rare case where no characters are allowed throw an exception as regex is faulty...
            else if(ranges[0].first == ascii_min)
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, ascii_max));
            else
              invert.push_back(new range_random_regex_node_(ascii_min, ranges[0].first - 1));
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
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(cur.second + 1, ascii_max));
                  }
                  else if(cur.first == ascii_min && cur.second == ascii_max)
                    throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // in rare case where no characters are allowed throw an exception as regex is faulty...
                  else if(cur.first == ascii_min)
                    invert.push_back(new range_random_regex_node_(cur.second + 1, ascii_max));
                  else
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));

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
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(next.second + 1, ascii_max));
                  }
                  else if(cur.first == ascii_min && next.second == ascii_max)
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw std::runtime_error("Regex error at " + std::to_string(param.consumed));
                  else if(cur.first == ascii_min)
                    invert.push_back(new range_random_regex_node_(next.second + 1, ascii_max));
                  else
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));

                  break;
                }

                cur.second = next.second;
              }
              else // ranges aren't connected
              {
                if(cur.first != ascii_min && cur.second != ascii_max)
                {
                  invert.push_back(new range_random_regex_node_(min, static_cast<char>(cur.first - 1)));
                  invert.push_back(new range_random_regex_node_(static_cast<char>(cur.second + 1), static_cast<char>(next.first - 1)));
                }
                else if(cur.first == ascii_min && cur.second == ascii_max)
                  throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // in rare case where no characters are allowed throw an exception as regex is faulty...
                // else skip the element as not allowed numbers are from beginning to end of this range
                // TODO !min but ==max with merge, merge, merge...???

                min = cur.second + 1;
                cur_it = it;
              }
            }
          }

          ++param.consumed; // consume ]
          return new or_regex_node_(std::move(invert));
        }}},
      [range_parser](regex_param& param){
          auto tmp = new or_regex_node_(range_parser(param)); // TODO check if range parser returns an empty vector
          ++param.consumed; // consume ]
          return tmp;
        }
    };

  or_parser parser_base_type{{
        {'(', [](regex_param& param)
          {
            if(param.regex.size() == param.consumed + 1) // handle () case
            {
              if(param.regex[param.consumed] == ')')
              {
                ++param.consumed;
                return new regex_node_(); // empty
              }
              else
                throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // not enough characters...
            }

            int capture_index = -1;
            if(param.regex[param.consumed] == '?')
            {
              // (?:...) is a non capturing group
              if(param.regex.size() > param.consumed + 2 && param.regex[param.consumed+1] == ':')
              {
                param.consumed += 2; // discard matches everything enclosed as that's exactly what will be generated anyway

                if(param.regex[param.consumed] == ')') // handle (?:) case
                {
                  ++param.consumed;
                  return new regex_node_(); // empty
                }
              }
              else
                throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // not enough characters
            }
            else
            {
              param.captured_groups.push_back(nullptr);
              capture_index = param.captured_groups.size() - 1;
            }

            auto node = parser_regex(param);
            if(param.regex.size() == param.consumed)
              throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // missing closing bracket
            ++param.consumed;

            if(capture_index != -1)
              param.captured_groups[capture_index] = node;

            return node;
          }}, // TODO, ')'},
        {'[', [range_or_negated_range](regex_param& param){return range_or_negated_range(param);}},
        {'\\', [parser_escaped](regex_param& param){return parser_escaped(param);}},
        {'.', [](regex_param& param)
          {
            return static_cast<regex_node_*>(new or_regex_node_{new range_random_regex_node_{ascii_min, 9}, // \n - 1
                                                                new range_random_regex_node_{11, 12}, // \n + 1, \r - 1
                                                                new range_random_regex_node_{14, ascii_max}});
          }}
      },
      [](regex_param& param){
          if(param.regex[param.consumed] == '$' || param.regex[param.consumed] == '^')
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // unsupported regex items

          return static_cast<regex_node_*>(new literal_regex_node_(param.regex[param.consumed++]));
        }
    };

  return parser_base_type(param);
}

regex_template::regex_template(std::experimental::string_view regex)
{
  if(regex.size())
  {
    regex_param param{regex};
    param.consumed = 0;
    root_node_.reset(parser_regex(param));

    if(param.regex.size() > param.consumed)
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // invalid regex
  }
}

void regex_template::generate(std::ostream& os) const
{
  random_generator<> rand; // TODO should be provided from the outside

  if(root_node_.get()) // TODO this if could be changed for dummy regex node
    root_node_->generate(os, rand);
}
