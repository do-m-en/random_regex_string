#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <random>
#include <limits>
#include <cctype>
#include "unicode/blocks.hpp" // TODO this should probbably be moved to random_regex_string as base? or not as ascii could also be supported...
#include "random_regex_string.hpp"

namespace {
  enum class test_result {
      OK,
      regex_error_and_failed_OK,
      regex_OK_generated_error,
      regex_OK_parsing_failed,
      regex_error_but_parsing_passed
    };

  std::pair<test_result, std::string> execute_regex(std::experimental::string_view regex)
  {
    try
    {
      rand_regex::regex_template generator(regex);
      std::set<std::string> distinct;

      try
      {      
        std::regex rx(regex.data(), regex.size()); // FIXME missing string_view constructor...

        for(int j=0; j<10; ++j)
        {
          std::stringstream ss;
          generator.generate(ss);

          std::string result(ss.str());

          if(!std::regex_match(result, rx))
            return std::make_pair(test_result::regex_OK_generated_error, result);

          distinct.insert(result);
        }
      }
      catch(const std::exception& e)
      {
        return std::make_pair(test_result::regex_error_but_parsing_passed, "");
      }
    }
    catch(...)
    {
      try
      {      
        std::regex rx(regex.data(), regex.size()); // FIXME missing string_view constructor...
      }
      catch(...)
      {
        return std::make_pair(test_result::regex_error_and_failed_OK, "");
      }

      return std::make_pair(test_result::regex_OK_parsing_failed, "");
    }

    return std::make_pair(test_result::OK, "");
  }

  std::pair<std::experimental::string_view, std::string> simplify_regex(std::experimental::string_view regex, test_result reproducing_result)
  {
    std::experimental::string_view working_regex{regex};
    std::string simplified_result;

    // subdivide untill it no longer matches
    while(1)  // TODO one of the while loops or all of them can cause the function to never exit...
    {
      if(working_regex.size() == 1) // prevent cycling
        break;

      std::experimental::string_view current_regex{working_regex.substr(0, working_regex.size()/2)};
      auto result = execute_regex(current_regex);
      if(reproducing_result == result.first)
      {
        working_regex = current_regex;
        simplified_result = result.second;
        continue;
      }

      current_regex = working_regex.substr(working_regex.size()/2);
      result = execute_regex(current_regex);
      if(reproducing_result == result.first)
      {
        working_regex = current_regex;
        simplified_result = result.second;
        continue;
      }

      // TODO cut out the middle...

      break; // could not make it smaller
    }

    // remove from back
    while(1)
    {
      if(working_regex.size() == 1) // prevent cycling
        break;

      std::experimental::string_view current_regex{working_regex.substr(0, working_regex.size()-1)};
      auto result = execute_regex(current_regex);
      if(reproducing_result == result.first)
      {
        working_regex = current_regex;
        simplified_result = result.second;
        continue;
      }

      break; // could not make it smaller
    }

    // remove from front
    while(1)
    {
      if(working_regex.size() == 1) // prevent cycling
        break;

      std::experimental::string_view current_regex{working_regex.substr(1)};
      auto result = execute_regex(current_regex);
      if(reproducing_result == result.first)
      {
        working_regex = current_regex;
        simplified_result = result.second;
        continue;
      }

      break; // could not make it smaller
    }

    // TODO try to remove it from the middle
    std::string final_regex{working_regex};

    for(std::size_t i = 0; i<working_regex.size(); ++i)
    {
      std::string current_regex{final_regex};
      current_regex[i] = 'a';
      auto result = execute_regex(current_regex);
      if(reproducing_result == result.first)
      {
        final_regex = current_regex;
        simplified_result = result.second;
        continue;
      }
    }

    return std::make_pair(final_regex, simplified_result);
  }

  std::string convert_chars_to_visible(std::experimental::string_view input)
  {
    std::string converted;

    for(auto item : input)
    {
      if(std::isprint(item))
        converted += item;
      else
        converted += "[conv[" + std::to_string((unsigned int)item) + "]]";
    }

    return converted;
  }

  void test_regex_ECMAScript_fuzzer()
  {
    int max = 1000;

    std::random_device rd;
    std::mt19937 mt(rd());

    for(int i=0; i<max; ++i)
    {
      std::uniform_int_distribution<int> length_dist(20, 200);
      std::uniform_int_distribution<char> char_dist(0, 127);
      std::string regex;

      for(int length = length_dist(mt); length >= 0; --length)
      {
        regex += char_dist(mt);
      }

      std::cout << "fuzzed regex='" << convert_chars_to_visible(regex) << "'" << std::endl;

      auto result = execute_regex(regex);
      switch(result.first)
      {
      case test_result::OK:
        std::cout << "  valid and passed - OK!\n";
        break;
      case test_result::regex_error_and_failed_OK:
        std::cout << "  not valid and failed - OK!\n";
        break;
      case test_result::regex_error_but_parsing_passed:
        {
          auto simplified = simplify_regex(regex, test_result::regex_error_but_parsing_passed);
          std::cout << "    ERROR! simplified_regex(" << regex.size() << "->" << simplified.first.size() << ")='" << convert_chars_to_visible(simplified.first) << "'\n";
          std::cout << "  not valid but passed!\n";
          throw 1; // TODO
        }
      case test_result::regex_OK_generated_error:
        {
          auto simplified = simplify_regex(regex, test_result::regex_OK_generated_error);
          std::cout << "    ERROR! simplified_regex(" << regex.size() << "->" << simplified.first.size() << ")='" << convert_chars_to_visible(simplified.first) << "'\n";
          std::cout << "    ERROR! simplified_reslut='" << convert_chars_to_visible(simplified.second) << "'\n";
          std::cout << "  regex OK but generator error!\n";
          throw std::runtime_error("regex test failed!");
        }
      case test_result::regex_OK_parsing_failed:
        {
          auto simplified = simplify_regex(regex, test_result::regex_OK_parsing_failed);
          std::cout << "    ERROR! simplified_regex(" << regex.size() << "->" << simplified.first.size() << ")='" << convert_chars_to_visible(simplified.first) << "'\n";
          std::cout << "  valid but failed!\n";
          throw 1; // TODO
        }
      }
    }
  }
}

namespace {
  void test_regex_ECMAScript(const std::string& regex)
  {
    using rand_regex::regex_template;

    std::cout << "regex='" << regex << "'";

    regex_template generator(regex);
    std::regex rx(regex);

    int max = 1000;
    std::set<std::string> distinct;

    for(int i=0; i<max; ++i)
    {
      std::stringstream ss;
      generator.generate(ss);

      std::string result(ss.str());

      if(!std::regex_match(result, rx))
      {
        std::cout << "    ERROR! result='" << convert_chars_to_visible(result) << "'\n";
        throw std::runtime_error("regex test failed!");
      }

      distinct.insert(result);
    }

    std::cout << "    distinct: " << distinct.size() << '/' << max << '\n';
  }
}

int main()
{
  try
  {
    test_regex_ECMAScript("a");
    test_regex_ECMAScript("a|f");
    test_regex_ECMAScript("a|f|g");
    test_regex_ECMAScript("gbc.*");
    test_regex_ECMAScript("gbc.*****************"); // oddly enough this id valid even though it means the same as *
    test_regex_ECMAScript("gbc+++++++++++(cd)"); // oddly enough this is valid even though it means the same as ++
    test_regex_ECMAScript("gbc.*+abc");
    test_regex_ECMAScript("(abc)*+abc"); // TODO test for *+ (is that even a legal statement?)
    test_regex_ECMAScript("a|f|gbc.*+abc");
    test_regex_ECMAScript("(ab)|(cde)|(fg(hi|(jk)))a");
    test_regex_ECMAScript("(ab)*|(cde)|(fg(hi|(jk)))a");
    test_regex_ECMAScript("ab?c?");
    test_regex_ECMAScript("a??b");
    test_regex_ECMAScript("a???b"); // oddly enough this is valid even though it means the same as ??
    test_regex_ECMAScript("ab\\?c\\\\\\?");
    test_regex_ECMAScript("(ab){2}(cd){3,5}");
    test_regex_ECMAScript("(ab){2}(cd){3,5}e{2,}");
    test_regex_ECMAScript("[0-9A-Z]{5}");
    test_regex_ECMAScript("[abd-x---0-9--]*");
    test_regex_ECMAScript("[a-c][abd][a-bcd-e0-8][---][--][-a-]");
    test_regex_ECMAScript("[0-9]{3}-\\s[0-9]{2}-[0-9]{4}");
    test_regex_ECMAScript("\\^*");
    test_regex_ECMAScript("[\\^]*");
    test_regex_ECMAScript("[\\n-\\^]*");
    test_regex_ECMAScript("\\f\\{");
    test_regex_ECMAScript("\\d+_\\D+");
    test_regex_ECMAScript("\\s+_\\S+");
    test_regex_ECMAScript("\\w+_\\W+");
    test_regex_ECMAScript("[a-]]");
    test_regex_ECMAScript("[^a]*");
    test_regex_ECMAScript("[^a-c]*");
    test_regex_ECMAScript("[^a-z0-9x]*");
    test_regex_ECMAScript("[^b-cd-eg-mh-ij-ls-vr-t]*");
    test_regex_ECMAScript("^");
    test_regex_ECMAScript("^ sdfasdfa");
    test_regex_ECMAScript("$");
    test_regex_ECMAScript("^$");
    test_regex_ECMAScript("\\x61");
    test_regex_ECMAScript("a\\Bb");
    test_regex_ECMAScript("a \\bb");
    test_regex_ECMAScript("\\k");
    test_regex_ECMAScript("\\`");
    test_regex_ECMAScript(".U");
    test_regex_ECMAScript("(abc)(def)(ghi)(jkl)(mno)(pqr)(stu)(abc1)(def1)(ghi1)(jkl1)(abc|mno1)\\12");
    test_regex_ECMAScript("(abc)(def)(ghi)(jkl)(mno)(pqr)(stu)(abc1)(def1)(ghi1(|abc|mno1))(jkl1)\\11"); // TODO empty or is not handled
    test_regex_ECMAScript("(abc)(def)(?:ghi)(jkl)\\3");
    test_regex_ECMAScript("(abc)(def)(?:ghi|xyz)(jkl)\\3");
    test_regex_ECMAScript("\\0");
    test_regex_ECMAScript("a\\0b");
    test_regex_ECMAScript("a|");
    test_regex_ECMAScript("|");
    test_regex_ECMAScript("a||b");
    test_regex_ECMAScript("|b");
//    test_regex_ECMAScript("a?*"); // TODO I'm at this one -> rebuild * + and ? parser so that they can be mixed in whatever order they choose
/* TODO as far as I understand \0 is not allowed to be followed by a decimal digit so why does this work???
    test_regex_ECMAScript("a\\01");
    test_regex_ECMAScript("a\\00");
*/
    test_regex_ECMAScript("()");
    test_regex_ECMAScript("(?:)");
    test_regex_ECMAScript("(?:abc)");
    test_regex_ECMAScript("(?:abc|def)");
//    test_regex_ECMAScript("a\\B"); TODO special exceptions for partial matches - I'll not support those?
    test_regex_ECMAScript_fuzzer();
    // TODO add negative cases... TODO also check if they realy are invalid
    // []
    // [z-a]
    // (
    // a\B b
    // a\B+ here + is a special character so it doesn't count
    // (+a) <- TODO this is not yet covered in parser...

  //  test_regex_ECMAScript("upper\\p{Lu}case");

    // TODO http://stackoverflow.com/questions/2637675/how-to-negate-the-whole-regex

    // odd regex patterns:
    // '|'
    // '$1
    // '1^'
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
