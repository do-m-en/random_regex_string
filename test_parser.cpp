

namespace test_parser {
/*
template<typename Iterator, typename Context>
bool parse(Iterator& first, Iterator last, const Context& ctx) const;
*/

template<typename Derived>
struct parser
{
  const Derived& derived() const
  {
    return *static_cast<const Derived*>(this);
  }
};

template<typename Char>
struct char_parser : parser<char_parser<Char>>
{
  char_parser(Char ch) : ch_{ch} {}
  
  template<typename Iterator, typename Context>
  bool parse(Iterator& first, Iterator last, const Context& ctx) const
  {
    if(first != last && *first == ch_)
    {
      ++first;
      return true;
    }

    return false;
  }

  Char ch_;
};

template<typename Char>
inline auto char_(Char ch)
{
  return char_parser<Char>{ch};
}

template<typename Left, typename Right>
struct sequence_parser : parser<sequence_parser<Left, Right>>
{
  sequence_parser(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context>
  bool parse(Iterator& first, Iterator last, const Context& ctx) const
  {
    return left_.parse(first, last, ctx) && right_.parse(first, last, ctx);
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline sequence_parser<Left, Right> operator>>(
  const parser<Left>& left, const parser<Right>& right)
{
  return sequence_parser<Left, Right>{left.derived(), right.derived()};
}

template<typename Left, typename Right>
struct alternative_parser : parser<alternative_parser<Left, Right>>
{
  alternative_parser(Left left, Right right) : left_{left}, right_{right} {}

  template<typename Iterator, typename Context>
  bool parse(Iterator& first, Iterator last, const Context& ctx) const
  {
    if(left_.parse(first, last, ctx))
      return true;
    return right_.parse(first, last, ctx);
  }

  Left left_;
  Right right_;
};

template<typename Left, typename Right>
inline alternative_parser<Left, Right> operator|(
  const parser<Left>& left, const parser<Right>& right)
{
  return alternative_parser<Left, Right>{left.derived(), right.derived()};
}

// -- context
template<typename T>
struct defer_type // same as boost::mpl::identity
{
//  typedef T type;
};

template<typename ID, typename T, typename Next_context>
struct context
{
  context(const T& val, const Next_context& next_ctx)
    : val_{val}, next_ctx_{next_ctx} {}

  const T& get(defer_type<ID>) const
  {
    return val_;
  }

  template<typename Identity>
//  decltype(std::declval<Next_context>().get(Identity()))
  auto
  get(Identity id) const
  {
    return next_ctx_.get(id);
  }

  const T& val_;
  const Next_context& next_ctx_;
};

struct empty_context
{
  struct undefined{};

  template<typename ID>
  undefined get(ID) const
  {
    return undefined{};
  }
};

// -- rule definition & rule

template<typename ID, typename RHS>
struct rule_definition : parser<rule_definition<ID, RHS>>
{
  rule_definition(RHS rhs) : rhs_{rhs} {}

  template<typename Iterator, typename Context>
  bool parse(Iterator& first, Iterator last, const Context& ctx) const
  {
    context<ID, RHS, Context> this_ctx{rhs_, ctx};
    return rhs_.parse(first, last, this_ctx);
  }

  RHS rhs_;
};

template<typename ID>
struct rule : parser<rule<ID>>
{
  template<typename Derived>
  rule_definition<ID, Derived>
  // auto
  operator=(const parser<Derived>& definition) const
  {
    return rule_definition<ID, Derived>{definition.derived()};
  }

  template<typename Iterator, typename Context>
  bool parse(Iterator& first, Iterator last, Context ctx) const
  {
    return ctx.get(defer_type<ID>{}).parse(first, last, ctx);
  }
};

/*
namespace parser
{
  namespace grammar_definition
  {
    const rule<class x> x;
    const auto ax = char_('a') >> x;

    const auto g =
      x = char_('x') | ax;
  }

  using grammar_definition::g;
}
*/

} // ns

struct foo_id;

template<typename Context>
void bar(const Context& ctx)
{
  using namespace test_parser;
  std::cout << ctx.get(defer_type<foo_id>{}) << '\n';
}

int main()
{
  using namespace test_parser;
  auto abc_parser = char_('a') | (char_('b') >> char_('c') >> char_('d'));

  const char ctxt_c{'c'};
  const char *abc = "bcd";
  
  const char *p_abc = abc;

  std::cout << abc_parser.parse(p_abc, abc + 3, ctxt_c) << '\n';

  std::cout << "parsed all: " << (p_abc == abc + 3) << '\n';

  //-----
  int i = 123;
  empty_context empty_ctx;
  context<foo_id, int, empty_context> ctx{i, empty_ctx};
  bar(ctx);

  //-----
  rule<class x> x;
  auto const first = char_('a') >> (x | char_('c'));

  auto const start =
    x = char_('d') >> first;

  const char *test_string = "dadc";
  const char *p_test = test_string;
  std::cout << start.parse(p_test, test_string + 4, x) << '\n';
  std::cout << "parsed all: " << (p_test == test_string + 4) << '\n';

  return 0;
}
