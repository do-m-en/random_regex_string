#ifndef UNICODE_E_CONTINUATION_TYPE_HPP_INCLUDED
#define UNICODE_E_CONTINUATION_TYPE_HPP_INCLUDED

namespace unicode {

enum class e_continuation_type
{
  begin, // start of continuation
  end, // end of continuation
  point, // consecutive single character (dent on left and right)
  dent, // missing this character in continuation
  skip_one_begin, // use only i%2 == 0 begin
  skip_one_end // use only i%2 == 0 end
};

}

#endif // UNICODE_E_CONTINUATION_TYPE_HPP_INCLUDED
