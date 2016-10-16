#include <iostream>
#include <fstream>
#include <string>
#include <regex>

namespace {

// input promises:
// unicode size() > 2
// unicode format: U+XX...X where X is replaced by hex value and ... means multiple hex values
unsigned long get_hex_part(std::string unicode)
{
  unicode[0] = '0';
  unicode[1] = 'x';

  unsigned long value;
  std::istringstream iss(unicode);
  iss >> std::hex >> value;

  return value;
}

} // namespace

int main()
{
  std::ifstream infile("in.txt");
  std::ofstream outfile("out.txt");
  int result = 0;
  std::string line;
  std::regex expression(R"(U\+[0-9A-Fa-f]*)");

  if(std::getline(infile, line))
  {
    if(std::regex_match(line, expression))
    {
      outfile << line;
      std::string prev_line(line);
      unsigned long prev_hex_part = get_hex_part(line);
      std::size_t cntr = 0;

      while(std::getline(infile, line))
      {
        if(std::regex_match(line, expression))
        {
          ++cntr;
          unsigned long hex_part = get_hex_part(line);

          if(prev_hex_part + 1 != hex_part)
          {
            if(cntr > 1)
            {
              outfile << " - " << prev_line;
            }

            outfile << ", " << line;
            cntr = 0;
          }

          prev_line = line;
          prev_hex_part = hex_part;
        }
        else
        {
          std::cerr << "Invalid input file format!" << std::endl;
          result = 1;
          break;
        }
      }

      if(cntr != 0)
      {
        outfile << " - " << prev_line;
      }
    }
    else
    {
      std::cerr << "Invalid input file format!" << std::endl;
      result = 1;
    }
  }

  return result;
}
