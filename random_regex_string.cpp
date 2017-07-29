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

#include "parser_context.hpp"

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


///- rules
template<typename ID, typename RHS>
struct rule_definition
{
  rule_definition(RHS rhs) : rhs_{rhs} {}

  template<typename Context>
  bool operator()(regex_param& param, const Context& ctx, auto& node) const
  {
    context<ID, RHS, Context> this_ctx{rhs_, ctx};
    return rhs_(param, this_ctx, node);
  }

  RHS rhs_;
};

template<typename ID>
struct rule
{
  template<typename Parser>
  auto operator=(const Parser& definition) const
  {
    return rule_definition<ID, Parser>{definition};
  }

  bool operator()(regex_param& param, const auto& ctx, auto& node) const
  {
    return ctx.get(defer_type<ID>{})(param, ctx, node);
  }
};

// class regex_template
using rand_regex::regex_template;

template<typename Left, typename Right>
inline auto operator>>(const Left& left, const Right& right) // sequence parser
{
  return [=](regex_param& param, const auto& context, auto& node) {
              if(auto result = left(param, context, node); result)
              {
                return right(param, context, node);
              }

              return false;
            };
}

inline auto operator "" _lp(char literal) // literal parser
{
  return [=](regex_param& param, const auto& context, auto& node){
              if(param.regex.size() > param.consumed &&
                param.regex[param.consumed] == literal)
              {
                ++param.consumed;

                return true;
              }

              return false;
            };
}

inline auto operator "" _nlp(char literal) // negated literal parser - lookahead
{
  return [=](regex_param& param, const auto& context, auto& node){
              if(param.regex.size() > param.consumed &&
                param.regex[param.consumed] != literal)
              {
                return true;
              }

              return false;
            };
}

template<typename Left, typename Right>
inline auto operator|(const Left& left, const Right& right) // alternative_parser
{
  return [=](regex_param& param, const auto& context, auto& node) {
              if(auto result = left(param, context, node); result)
                return true;

              return right(param, context, node);
            };
}

template<typename Right>
inline auto operator-(const Right& right) // optional_parser
{
  return [=](regex_param& param, const auto& context, auto& node) {
               right(param, context, node);

               return true;
            };
}

// ---- generator
inline auto operator "" _lg(char literal) // literal generator
{
  return [=](regex_param& param, const auto& context, auto& node){
              node = new literal_regex_node_{literal};
              return true;
            };
}
// ---- generator end


const rule<class regex_rule> regex_rule;


namespace
{
  /* terminator */
  auto terminate = [](regex_param& param, const auto& context, auto& node) -> bool {
          throw std::runtime_error("Regex error at " + std::to_string(param.consumed));
        };
  /* terminator end */

  /* generators */
  auto lit_gen = [](regex_param& param, const auto& context, auto& node){
          node = new literal_regex_node_(param.regex[param.consumed++]);
          return true;
        };
  /* generators end */

  auto form_feed = 'f'_lp >> '\f'_lg;
  auto new_line = 'n'_lp >> '\n'_lg;
  auto carriage_return = 'r'_lp >> '\r'_lg;
  auto horizontal_tab = 't'_lp >> '\t'_lg;
  auto vertical_tab = 'v'_lp >> '\v'_lg;
  auto any_digit = 'd'_lp >> [](regex_param& param, const auto& context, auto& node){
        node = new range_random_regex_node_{'0', '9'};
        return true;
      };
  auto null_character = '0'_lp >> '\0'_lg;
  auto any_non_digit = 'D'_lp >> rand_regex::any_non_digit_gen;
  auto any_whitespace = 's'_lp >> [](regex_param& param, const auto& context, auto& node){
        node = new whitespace_regex_node_{};
        return true;
      };
  auto any_non_whitespace = 'S'_lp >> rand_regex::any_non_whitespace_gen;
  auto any_alphanum_or_underscore = 'w'_lp >> rand_regex::any_alphanum_or_underscore_gen;
  auto any_not_alphanum_or_underscore = 'W'_lp >> rand_regex::any_not_alphanum_or_underscore_gen;
  auto hexadecimal_ascii_character_representation = 'x'_lp >> [](regex_param& param, const auto& context, auto& node){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // either regex ends too soon or the next two characters are not digits

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // requested char is out of ascii bounds

          param.consumed += 2;

          node = new literal_regex_node_(static_cast<char>(ascii_hex));
          return true;
        };

  auto backreference_parser = ('1'_lp | '2'_lp | '3'_lp | '4'_lp | '5'_lp | '6'_lp | '7'_lp | '8'_lp | '9'_lp) >>
        [](regex_param& param, const auto& context, auto& node){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(std::string(param.regex.substr(param.consumed-1, end)));

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == false)
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          node = new captured_group_reference_node_(digit - 1);
          return true;
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
        '\\'_lp
        >>
        (
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
          | escaped_regex_end_parser

          | terminate);

  auto escaped_or_literal =
        parser_escaped
      | lit_gen;

  auto parse_end_range_escaped =
        '\\'_lp
      >>
      (
          form_feed
        | new_line
        | carriage_return
        | horizontal_tab
        | vertical_tab
        | hexadecimal_ascii_character_representation

        // escaped special chars
        | escaped_escape_parser
        | escaped_optional_parser
        | escaped_repeat_zero_parser
        | escaped_repeat_one_parser
        | escaped_repeat_parser
        | escaped_group_parser
        | escaped_regex_start_parser
        | escaped_regex_end_parser

        | terminate);

  auto end_range_escaped_or_literal =
        parse_end_range_escaped
      | lit_gen;

  auto range_parser =
       ']'_nlp
      >> [](regex_param& param, const auto& context, auto& node)
          {
            if(!escaped_or_literal(param, context, node))
              throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // node was not created

            if(param.consumed+2 < param.regex.size() && param.regex[param.consumed] == '-' && param.regex[param.consumed+1] != ']')
            {
              literal_regex_node_* literal_node = static_cast<literal_regex_node_*>(node);

              ++param.consumed;

              if(param.consumed+1 >= param.regex.size())
                throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // group not closed but we're already at the end of regex

              regex_node_* tmp = nullptr;
              if(!end_range_escaped_or_literal(param, context, tmp))
                throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // node was not created

              node = new range_random_regex_node_(literal_node->getLiteral(), static_cast<literal_regex_node_*>(tmp)->getLiteral()); // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
            }

            return true;
          };

  auto range_or_negated_range =
        '['_lp >>
        (((('^'_lp >> [](regex_param& param, const auto& context, auto& node){
          std::vector<regex_node_*> tmp;
          regex_node_* tmp_node;
          for(bool ok = range_parser(param, context, tmp_node); ok; ok = range_parser(param, context, tmp_node))
          {
            tmp.push_back(tmp_node);
          }

          if(tmp.empty())
          {
            node = new range_random_regex_node_(ascii_min, ascii_max); // any character
            return true;
          }

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

          node = new or_regex_node_(std::move(invert));
          return true;
        })
      | ([](regex_param& param, const auto& context, auto& node){
          std::vector<regex_node_*> items;
          regex_node_* tmp_node;
          for(bool ok = range_parser(param, context, tmp_node); ok; ok = range_parser(param, context, tmp_node))
          {
            items.push_back(tmp_node);
          }

          if(!items.empty())
          {
            node = new or_regex_node_(std::move(items));
            return true;
          }

          return false;
        })
        | terminate // [] always fails so it's invalid for a generator
        ) >> ']'_lp) | terminate);

  auto empty_group_end = ')'_lp >> [](regex_param& param, const auto& context, auto& node){ // handle empty group
                            node = new regex_node_(); // empty
                            return true;
                          };

  // <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
  auto parser_base =
          ('('_lp >>
            ( empty_group_end
              |
              ( // non capturing group
                '?'_lp >> (':'_lp | terminate) >>
                (
                    empty_group_end
                  | (regex_rule >> ')'_lp)
                  | terminate)
                )
              |
              [](regex_param& param, const auto& context, auto& node) // capture group
              {
                param.captured_groups.push_back(false);
                int capture_index = param.captured_groups.size() - 1;

                regex_node_* tmp_node = nullptr;
                bool ok = regex_rule(param, context, tmp_node);
                if(!ok || param.regex.size() == param.consumed)
                  throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // missing closing bracket
                param.captured_groups[capture_index] = true;

                node = new inner_group_node_{tmp_node, capture_index};
                return true;
              } >> ')'_lp))
        | range_or_negated_range // [^range] | [range]
        | parser_escaped
        | ('.'_lp >> rand_regex::dot_gen)
        | (('$'_lp | '^'_lp) >> terminate) // unsupported regex items - '$' end of string and '^' start of string regex items are not supported for now
        | lit_gen; // the rest of the items are treated as literals

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
        '{'_lp
          >> [](regex_param& param, const auto& context, auto& node) {
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

                return true;
              } >> '}'_lp;

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
auto parser_factor = [](regex_param& param, const auto& context, auto& node)
{
  if(param.regex.size() == param.consumed)
  {
    node = new regex_node_{}; // empty or
    return true;
  }

  bool ok = parser_base(param, context, node);

  auto repeat_range_zero = '*'_lp >> [](regex_param& param, const auto& context, auto& node){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);
          new repeat_range_regex_node_(node, 0); // FIXME!!!!!! assign to node...

          return true;
        };
  auto repeat_range_one = '+'_lp >> [](regex_param& param, const auto& context, auto& node){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);
          new repeat_range_regex_node_(node, 1); // FIXME!!!!!! assign to node...

          return true;
        };
  auto optional_item = '?'_lp >> [](regex_param& param, const auto& context, auto& node){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);
          new optional_regex_node_(node); // FIXME!!!!!! assign to node...

          return true;
        };

  repeat_range_zero(param, context, node);
  repeat_range_one(param, context, node);
  optional_item(param, context, node);

  return ok;
} >> -limited_repetition_parser;

// <term> ::= { <factor> }
auto term =
    parser_factor >> -('|'_nlp >> ')'_nlp >> // not ) or |
      (
        [](regex_param& param, const auto& context, auto& node) {
          bool ok = true;

          node = new group_regex_node_{node}; // TODO consider renaming it to sequence
          do
          {
            regex_node_* next = nullptr;
            ok = parser_factor(param, context, next);
            static_cast<group_regex_node_*>(node)->push_back(next);
          } while(ok && param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
            && param.regex[param.consumed] != '|');

          return ok;
        } | terminate));

// <regex> ::= <term> '|' <regex> | <term>
auto parser_regex = regex_rule =
    -('|'_lp) >> term >> -('|'_lp >>
      ( [](regex_param& param, const auto& context, auto& node) -> bool
        {
          regex_node_* other = nullptr;
          bool ok = regex_rule(param, context, other);

          if(ok)
            node = new or_regex_node_{node, other};

          return ok;
        }
        | terminate));
} // ns

regex_template::regex_template(std::string_view regex)
{
  if(regex.size())
  {
    regex_param param{regex};
    param.consumed = 0;
    regex_node_* node;

    if(!parser_regex(param, unused_type{}, node) || param.regex.size() > param.consumed)
      throw std::runtime_error("Regex error at " + std::to_string(param.consumed)); // invalid regex

    root_node_.reset(node);
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
