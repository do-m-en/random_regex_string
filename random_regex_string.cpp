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
#include <string>

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
#include "inner_group_node.hpp"

#include "ascii_nodes.hpp"

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
using rand_regex::inner_group_node_;

constexpr char ascii_min = 0;
constexpr char ascii_max = 127;

struct regex_param
{
  regex_param(std::string_view regex_) : regex{regex_} {}

  std::string_view regex;
  std::size_t consumed = 0;
  std::vector<bool> captured_groups;
};

// class regex_template
using rand_regex::regex_template;

template<typename Left, typename Right>
struct sequence_parser
{
  sequence_parser(Left left, Right right) : left_{left}, right_{right} {}

  regex_node_* operator()(regex_param& param) const
  {
    if(auto result = left_(param); result)
    {
      return right_(param);
    }

    return nullptr;
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline sequence_parser<Left, Right> operator>>(
  const Left& left, const Right& right)
{
  return sequence_parser<Left, Right>{left, right};
}

inline auto operator "" _lp(char literal)
{
  return [=](regex_param& param){
              if(param.regex.size() > param.consumed)
              {
                if(param.regex[param.consumed] == literal)
                {
                  ++param.consumed;

                  return true;
                }
              }

              return false;
            };
}

inline auto operator "" _nlp(char literal)
{
  return [=](regex_param& param){
              if(param.regex.size() > param.consumed)
              {
                if(param.regex[param.consumed] != literal)
                {
                  return true;
                }
              }

              return false;
            };
}

template<typename Left, typename Right>
struct alternative_parser
{
  alternative_parser(Left left, Right right) : left_{left}, right_{right} {}

  regex_node_* operator()(regex_param& param) const
  {
    if(auto node = left_(param); node)
      return node;

    return right_(param);
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline alternative_parser<Left, Right> operator|(
  const Left& left, const Right& right)
{
  return alternative_parser<Left, Right>{left, right};
}


template<typename Left, typename Right>
struct alternative_parser_bool
{
  alternative_parser_bool(Left left, Right right) : left_{left}, right_{right} {}

  bool operator()(regex_param& param) const
  {
    if(bool ok = left_(param); ok)
      return true;

    return right_(param);
    // FIXME this alternative somehow causes a test to fail: return (left_(param) | right_(param));
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline alternative_parser_bool<Left, Right> operator>(
  const Left& left, const Right& right)
{
  return alternative_parser_bool<Left, Right>{left, right};
}

// ---- generator
inline auto operator "" _lg(char literal)
{
  return [=](regex_param& param){
              return new literal_regex_node_{literal};
            };
}
// ---- generator end

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

  auto repeat_range_zero = '*'_lp >> [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);
          return new repeat_range_regex_node_(node, 0);
        };
  auto repeat_range_one = '+'_lp >> [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);
          return new repeat_range_regex_node_(node, 1);
        };
  auto optional_item = '?'_lp >> [node](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);
          return new optional_regex_node_(node);
        };

  repeat_range_zero(param);
  repeat_range_one(param);
  optional_item(param);

  auto digit_parser = [](regex_param& param){
        std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed); // TODO find in range 0-9 would be nice...

        if(end == std::string_view::npos || end == param.consumed)
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // number was expected

        int digit = std::stoi(std::string(param.regex.substr(param.consumed, end)));

        param.consumed = end;

        return digit;
      };

  // {x[,y]}
  auto limited_repetition_parser =
       ('{'_lp
          >> [&](regex_param& param) {
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

                return node;
              })
    | [&](regex_param& param){return node;};

  return limited_repetition_parser(param);
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(regex_param& param)
{
  auto form_feed = 'f'_lp >> '\f'_lg;
  auto new_line = 'n'_lp >> '\n'_lg;
  auto carriage_return = 'r'_lp >> '\r'_lg;
  auto horizontal_tab = 't'_lp >> '\t'_lg;
  auto vertical_tab = 'v'_lp >> '\v'_lg;
  auto any_digit = 'd'_lp >> [](regex_param& param){return new range_random_regex_node_{'0', '9'};};
  auto null_character = '0'_lp >> '\0'_lg;
  auto any_non_digit = 'D'_lp >> [](regex_param& param){return rand_regex::any_non_digit_node();};
  auto any_whitespace = 's'_lp >> [](regex_param& param){return new whitespace_regex_node_{};};
  auto any_non_whitespace = 'S'_lp >> [](regex_param& param){return rand_regex::any_non_whitespace_node();};
  auto any_alphanum_or_underscore = 'w'_lp >> [](regex_param& param){return rand_regex::any_alphanum_or_underscore_node();};
  auto any_not_alphanum_or_underscore = 'W'_lp >> [](regex_param& param){return rand_regex::any_not_alphanum_or_underscore_node();};
  auto hexadecimal_ascii_character_representation = 'x'_lp >> [](regex_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // either regex ends too soon or the next two characters are not digits

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // requested char is out of ascii bounds

          param.consumed += 2;

          return new literal_regex_node_(static_cast<char>(ascii_hex));
        };

  auto backreference_parser = ('1'_lp > '2'_lp > '3'_lp > '4'_lp > '5'_lp > '6'_lp > '7'_lp > '8'_lp > '9'_lp) >>
        [](regex_param& param){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(std::string(param.regex.substr(param.consumed-1, end)));

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == false)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          return new captured_group_reference_node_(digit - 1);
        };

  auto escaped_escape_parser = '\\'_lp >> '\\'_lg;
  auto escaped_optional_parser = '?'_lp >> '?'_lg;
  auto escaped_repeat_zero_parser = '*'_lp >> '*'_lg;
  auto escaped_repeat_one_parser = '+'_lp >> '+'_lg;
  auto escaped_repeat_parser = '{'_lp >> '{'_lg;
  auto escaped_group_parser = '['_lp >> '['_lg;
  auto escaped_regex_start_parser = '^'_lp >> '^'_lg;
  auto escaped_regex_end_parser = '$'_lp >> '$'_lg;

  auto parser_escaped = 
        form_feed
      | new_line
      | carriage_return
      | horizontal_tab
      | vertical_tab
      | any_digit
      | any_non_digit
      | any_whitespace
      | any_non_whitespace
      | any_alphanum_or_underscore
      | any_not_alphanum_or_underscore
      | hexadecimal_ascii_character_representation
      | null_character
      | backreference_parser

      // escaped special chars
      | escaped_escape_parser
      | escaped_optional_parser
      | escaped_repeat_zero_parser
      | escaped_repeat_one_parser
      | escaped_repeat_parser
      | escaped_group_parser
      | escaped_regex_start_parser
      | escaped_regex_end_parser;

  auto escaped_or_literal =
        ('\\'_lp >> [parser_escaped](regex_param& param){return parser_escaped(param);})
      | [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);};

  auto parse_end_range_escaped =
        form_feed
      | new_line
      | carriage_return
      | horizontal_tab
      | vertical_tab
      | hexadecimal_ascii_character_representation
      | [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);}; // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)

  auto end_range_escaped_or_literal =
        ('\\'_lp >> [parse_end_range_escaped](regex_param& param){return parse_end_range_escaped(param);})
      | [](regex_param& param){return new literal_regex_node_(param.regex[param.consumed++]);};

  auto range_parser =
       ']'_nlp
      >> [escaped_or_literal, end_range_escaped_or_literal](regex_param& param)
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
          };

  auto range_or_negated_range =
        ('^'_lp >> [range_parser](regex_param& param){
          std::vector<regex_node_*> tmp;
          for(auto item = range_parser(param); item != nullptr; item = range_parser(param))
          {
            tmp.push_back(item);
          }
          // TODO check if range parser returns an empty vector

          if(param.regex[param.consumed] != ']') // TODO delete nodes before this point
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // terminator not found*/
          ++param.consumed;

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

          return new or_regex_node_(std::move(invert));
        })
      | [range_parser](regex_param& param){
          std::vector<regex_node_*> items;
          for(auto tmp = range_parser(param); tmp != nullptr; tmp = range_parser(param))
          {
            items.push_back(tmp);
          }
          // TODO check if range parser returns an empty vector

          if(param.regex[param.consumed] != ']') // TODO delete nodes before this point
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // terminator not found*/
          ++param.consumed;

          return new or_regex_node_(std::move(items));
        };

  auto parser_base_type =
          ('('_lp >> [](regex_param& param)
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
                param.captured_groups.push_back(false);
                capture_index = param.captured_groups.size() - 1;
              }

              auto node = parser_regex(param);
              if(param.regex.size() == param.consumed)
                throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // missing closing bracket
              ++param.consumed;

              if(capture_index != -1)
                param.captured_groups[capture_index] = true;

              return static_cast<regex_node_*>(new inner_group_node_{node, capture_index});
            }) // TODO, ')'},
        | ('['_lp >> [range_or_negated_range](regex_param& param){return range_or_negated_range(param);})
        | ('\\'_lp >> [parser_escaped](regex_param& param){
            auto node = parser_escaped(param);
            if(node == nullptr)
              throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // FIXME add more returned nullptr handling...

            return node;
          }) // FIXME nullptr return handling...
        | ('.'_lp >> [](regex_param& param){return rand_regex::dot_node();})
        | [](regex_param& param){
            if(param.regex[param.consumed] == '$' || param.regex[param.consumed] == '^')
              throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // unsupported regex items

            return static_cast<regex_node_*>(new literal_regex_node_(param.regex[param.consumed++]));
          };

  return parser_base_type(param);
}

regex_template::regex_template(std::string_view regex)
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
  {
    std::vector<std::tuple<int, regex_node_*>> groups;
    root_node_->generate(os, rand, groups);
  }
}
