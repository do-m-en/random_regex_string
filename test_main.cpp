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

  std::pair<test_result, std::string> execute_regex(std::string_view regex)
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

  std::pair<std::string_view, std::string> simplify_regex(std::string_view regex, test_result reproducing_result)
  {
    std::string_view working_regex{regex};
    std::string simplified_result;

    // subdivide untill it no longer matches
    /*while(1)  // TODO one of the while loops or all of them can cause the function to never exit...
    {
      if(working_regex.size() == 1) // prevent cycling
        break;

      std::string_view current_regex{working_regex.substr(0, working_regex.size()/2)};
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
    }*/

    // remove from back
    while(1)
    {
      if(working_regex.size() == 1) // prevent cycling
        break;

      std::string_view current_regex{working_regex.substr(0, working_regex.size()-1)};
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

      std::string_view current_regex{working_regex.substr(1)};
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

  std::string convert_chars_to_visible(std::string_view input)
  {
    std::string converted;
    const char hex[] = "0123456789ABCDEF";

    for(auto item : input)
    {
      if(std::isprint(item))
        converted += item;
      else
      {
        unsigned char c = static_cast<unsigned char>(item);
        converted += "\\x";
        converted += hex[c >> 4];
        converted += hex[c & 0xf];
        converted += "";
      }
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
      std::uniform_int_distribution<char> char_dist(6, 127);
      std::string regex;

      for(int length = length_dist(mt); length >= 0; --length)
      {
        char gen = char_dist(mt);
        // ignore chars that we don't support but are valid regex entries
        if(gen == '^' || gen == '$' || gen == 'b' || gen == 'B' || gen == '\\')
          regex += gen + 1;
        else
          regex += gen;
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

    std::regex rx(regex); // check the validity of a regex first
    std::cout << " std::regex valid";
    regex_template generator(regex);

    int max = 10000;
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

  void test_negative_regex_ECMAScript(const std::string& regex)
  {
    using rand_regex::regex_template;

    std::cout << "negative regex='" << regex << "'";

    try
    {
      std::regex rx(regex); // check the validity of a regex first
      std::cout << "rx is ok!\n";
    }
    catch(...)
    {
      std::cout << "unexpected negative test failure: regex is not valid\n";
      throw std::runtime_error("unexpected negative test failure: regex is not valid");
    }

    try
    {
      regex_template generator(regex);
      throw std::runtime_error("unexpected negative test success!");
    }
    catch(...)
    {
      std::cout << "Expected fail.\n";
    }
  }


  void test_invalid_regex_ECMAScript(const std::string& regex)
  {
    using rand_regex::regex_template;

    std::cout << "invalid regex='" << regex << "'";

    try
    {
      std::regex rx(regex); // check the validity of a regex first
      if(std::regex_match("abc", rx)) // here only because we want to prevent rx from being removed by the optimizer...
      {
        std::cout << "foooo\n";
      }
      throw std::runtime_error("unexpected invalid test failure: regex is valid");
    }
    catch(...)
    {
      //
    }

    bool success = false;
    try
    {
      regex_template generator(regex);
      success = true;
    }
    catch(const std::runtime_error& e)
    {
      std::cout << "Expected fail. " << e.what() << "\n";
    }
    catch(...)
    {
      std::cout << "Expected fail.\n";
    }

    if(success)
      throw std::runtime_error("unexpected invalid test success!");
  }
  void test_unsupported_regex_ECMAScript(std::string_view regex)
  {
    using rand_regex::regex_template;

    std::cout << "unsupported regex='" << regex << "'";

    bool success = false;
    try
    {
      regex_template generator(regex);
      success = true;
    }
    catch(const std::runtime_error& e)
    {
      std::cout << "Expected fail. " << e.what() << "\n";
    }
    catch(...)
    {
      std::cout << "Expected fail.\n";
    }

    if(success)
      throw std::runtime_error("unexpected negative test success!");
  }
}

int main()
{
  try
  {
    test_regex_ECMAScript("a");
    test_regex_ECMAScript("ab");
    test_regex_ECMAScript("a|f");
    test_regex_ECMAScript("a|f|g");
    test_regex_ECMAScript("ab|f|g");
    test_regex_ECMAScript("a|fe|g");
    test_regex_ECMAScript("a|f|gh");
    test_regex_ECMAScript("ab|fe|gh");

    test_regex_ECMAScript("()");
    test_regex_ECMAScript("(a)");

    test_regex_ECMAScript("[a]");
    test_regex_ECMAScript("[a-b]");
    test_regex_ECMAScript("[a-b1]");
    test_regex_ECMAScript("[a-b1-9]");
    test_regex_ECMAScript("[0-9A-Z]{5}");
    test_regex_ECMAScript("[abd-x---0-9--]*");

    test_regex_ECMAScript("a*");

    test_regex_ECMAScript("a{3,6}");
    test_regex_ECMAScript("a{3}");
    test_regex_ECMAScript("a{3,}");
    test_regex_ECMAScript("(ab)|(cde)|(fg(hi|(jk)))a");
    test_regex_ECMAScript("(ab)*|(cde)|(fg(hi|(jk)))a");
    test_regex_ECMAScript("ab?c?");
    test_regex_ECMAScript("a??b");
    test_regex_ECMAScript("ab\\?c\\\\\\?");
    test_regex_ECMAScript("(ab){2}(cd){3,5}");
    test_regex_ECMAScript("(ab){2}(cd){3,5}e{2,}");
    test_regex_ECMAScript("[a-c][abd][a-bcd-e0-8][---][--][-a-]");
    test_regex_ECMAScript("[0-9]{3}-\\s[0-9]{2}-[0-9]{4}");
    test_regex_ECMAScript("a\\nb");
    test_regex_ECMAScript("\\D");
    test_regex_ECMAScript("\\^*");
    test_regex_ECMAScript("[\\^]*");
    test_regex_ECMAScript("\\x65");

    test_regex_ECMAScript("\\f\\{");
    test_regex_ECMAScript("\\d+_\\D+");
    test_regex_ECMAScript("\\s+_\\S+");
    test_regex_ECMAScript("\\w+_\\W+");
    test_regex_ECMAScript("[a-]]");

    test_regex_ECMAScript("gbc.*");
    test_regex_ECMAScript("\\^*");
    test_regex_ECMAScript("[\\^]*");
    test_regex_ECMAScript("[\\n-\\^]*");
    test_regex_ECMAScript("[^a]*");
    test_regex_ECMAScript("[^a-c]*");
    test_regex_ECMAScript("[^a-z0-9x]*");
    test_regex_ECMAScript("[^b-cd-eg-mh-ij-ls-vr-t]*");

    test_regex_ECMAScript("\\^");
    test_regex_ECMAScript("\\^ sdfasdfa");
    test_regex_ECMAScript("\\$");
    test_regex_ECMAScript("(\\$)");
    test_regex_ECMAScript("\\^\\$");
    test_regex_ECMAScript("\\$|a");
    test_regex_ECMAScript("(\\$)|a");
    test_regex_ECMAScript("(\\$|ab)|a");
    test_regex_ECMAScript("((a\\$)|ab)|a");
    test_regex_ECMAScript("\\^abc\\$|\\^b\\$");
    test_regex_ECMAScript("(\\^a|(\\^b))bc\\$|\\^b\\$");

    test_regex_ECMAScript("\\x61");
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

/* TODO as far as I understand \0 is not allowed to be followed by a decimal digit so why does this work???
    test_regex_ECMAScript("a\\01");
    test_regex_ECMAScript("a\\00");
*/
    test_regex_ECMAScript("()");
    test_regex_ECMAScript("(?:)");
    test_regex_ECMAScript("(?:abc)");
    test_regex_ECMAScript("(?:abc|def)");

    //test_regex_ECMAScript("d[conv[22]]syMs\=[conv[31]][conv[26]][conv[0]][conv[30]]z[conv[27]]KR=n/mdpZzd96j[conv[4]]LVgJ"ykXp[conv[17]][conv[3]][conv[21]][conv[5]]gKl[conv[8]]7[conv[1]][conv[20]][conv[11]]chcJ@");

    // negative tests
    test_negative_regex_ECMAScript("a\\ba");
    test_negative_regex_ECMAScript("gbc.**");
    test_negative_regex_ECMAScript("gbc++(cd)");
    test_negative_regex_ECMAScript("gbc.*+abc");
    test_negative_regex_ECMAScript("(abc)*+abc");
    test_negative_regex_ECMAScript("a|f|gbc.*+abc");
    test_negative_regex_ECMAScript("a???b");
    test_negative_regex_ECMAScript("((a$)|ab)c|a");
    test_negative_regex_ECMAScript("a\\bc");
    test_negative_regex_ECMAScript("a\\B c");
    test_negative_regex_ECMAScript("a?*");
    test_negative_regex_ECMAScript("$1");
    test_negative_regex_ECMAScript("1^");

    test_negative_regex_ECMAScript("\\u1234");
    test_invalid_regex_ECMAScript("\\u1234");

    // negative in C++ regex but working in online regex tester as literal text
    test_unsupported_regex_ECMAScript("a*{,3}");
    test_unsupported_regex_ECMAScript("a*{a,3}");
    test_unsupported_regex_ECMAScript("a*{ 1,3}");

    // word boundary regex is not supported as we'd have to support too many cases
    test_unsupported_regex_ECMAScript("a\\Bb");
    test_unsupported_regex_ECMAScript("a \\bb");
    // start and end of text marker are not supported as we want to controll the generation of the entire
    // string anyway so anyone wanting to generate don't-care front and back can use something like
    // .{10,1000}
    test_unsupported_regex_ECMAScript("^");
    test_unsupported_regex_ECMAScript("^ sdfasdfa");
    test_unsupported_regex_ECMAScript("$");
    test_unsupported_regex_ECMAScript("($)");
    test_unsupported_regex_ECMAScript("^$");
    test_unsupported_regex_ECMAScript("$|a");
    test_unsupported_regex_ECMAScript("($)|a");
    test_unsupported_regex_ECMAScript("($|ab)|a");
    test_unsupported_regex_ECMAScript("((a$)|ab)|a");
    test_unsupported_regex_ECMAScript("^abc$|^b$");
    test_unsupported_regex_ECMAScript("(^a|(^b))bc$|^b$");
    test_unsupported_regex_ECMAScript("\\p");
    test_unsupported_regex_ECMAScript("(\\a)");

    // TODO add negative cases... TODO also check if they realy are invalid
    // []
    // [z-a]
    // (
    // a\B+ here + is a special character so it doesn't count
    // (+a) <- TODO this is not yet covered in parser...

  //  test_regex_ECMAScript("upper\\p{Lu}case");

    // TODO http://stackoverflow.com/questions/2637675/how-to-negate-the-whole-regex

//    test_regex_ECMAScript_fuzzer();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
