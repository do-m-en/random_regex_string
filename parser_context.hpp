struct unused_type{};


// -- context
template<typename T>
struct defer_type // same as boost::mpl::identity
{
//  typedef T type;
};

template<typename ID, typename T, typename Next_context>
struct context // compile time list
{
  context(const T& val, const Next_context& next_ctx)
    : val_{val}, next_ctx_{next_ctx} {}

  const T& get(defer_type<ID>) const
  {
    return val_;
  }

  template<typename Identity>
  auto get(Identity id) const
  {
    return next_ctx_.get(id);
  }

  const T& val_;
  const Next_context& next_ctx_;
};

struct empty_context // terminator (last element)
{
  struct undefined{};

  template<typename ID>
  undefined get(ID) const
  {
    return undefined{};
  }
};

