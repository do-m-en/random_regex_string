#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <random>
#include <limits>
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
          {
            return std::make_pair(test_result::regex_OK_generated_error, result);
          }

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

  void simplify_regex()
  {
  }

  void test_regex_ECMAScript_fuzzer()
  {
    int max = 1000;

    std::random_device rd;
    std::mt19937 mt(rd());

    for(int i=0; i<max; ++i)
    {
      std::uniform_int_distribution<int> length_dist(20, 200);
      std::uniform_int_distribution<char> char_dist(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
      std::string regex;

      for(int length = length_dist(mt); length >= 0; --length)
      {
        regex += char_dist(mt);
      }

      std::cout << "fuzzed regex='" << regex << "'" << std::endl;

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
        std::cout << "  not valid but passed!\n";
        throw 1; // TODO
      case test_result::regex_OK_generated_error:
        std::cout << "    ERROR! result='" << result.second << "'\n";
        throw std::runtime_error("regex test failed!");
      case test_result::regex_OK_parsing_failed:
        std::cout << "  valid but failed!\n";
        throw 1; // TODO
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
        std::cout << "    ERROR! result='" << result << "'\n";
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
    test_regex_ECMAScript_fuzzer();
    // TODO add negative cases... TODO also check if they realy are invalid
    // []
    // [z-a]

  //  test_regex_ECMAScript("upper\\p{Lu}case");

    // TODO http://stackoverflow.com/questions/2637675/how-to-negate-the-whole-regex
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
