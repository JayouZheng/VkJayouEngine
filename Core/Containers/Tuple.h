//
// Tuple.h
//

#pragma once

// helpers
template <typename T>
struct id { using type = T; };

template <typename T>
using type_of = typename T::type;

template <size_t... N>
struct sizes : id <sizes <N...> > { };

// choose N-th element in list <T...>
template <size_t N, typename... T>
struct Choose;

template <size_t N, typename H, typename... T>
struct Choose <N, H, T...> : Choose <N - 1, T...> { };

template <typename H, typename... T>
struct Choose <0, H, T...> : id <H> { };

template <size_t N, typename... T>
using choose = type_of <Choose <N, T...> >;

// given L>=0, generate sequence <0, ..., L-1>
template <size_t L, size_t I = 0, typename S = sizes <> >
struct Range;

template <size_t L, size_t I, size_t... N>
struct Range <L, I, sizes <N...> > : Range <L, I + 1, sizes <N..., I> > { };

template <size_t L, size_t... N>
struct Range <L, L, sizes <N...> > : sizes <N...> { };

template <size_t L>
using range = type_of <Range <L> >;

// single tuple element
template <size_t N, typename T>
class TupleElem
{
	T elem;
public:
	T& get() { return elem; }
	const T& get() const { return elem; }
};

// tuple implementation
template <typename N, typename... T>
class TupleImpl;

template <size_t... N, typename... T>
class TupleImpl <sizes <N...>, T...> : TupleElem <N, T>...
{
	template <size_t M> using pick = choose <M, T...>;
	template <size_t M> using elem = TupleElem <M, pick <M> >;

public:
	template <size_t M>
	pick <M>& get() { return elem <M>::get(); }

	template <size_t M>
	const pick <M>& get() const { return elem <M>::get(); }
};

template <typename... T>
struct Tuple : TupleImpl <range <sizeof...(T)>, T...>
{
	static constexpr std::size_t size() { return sizeof...(T); }
};

#pragma region Understand Tuple

#if 0
inline void tuple_test()
{
	using namespace std;

	id<int>::type c;
	sizes<1, 2, 3>::type b;
	id<sizes<5, 6, 7>>::type d;
	type_of<Range <10, 0, sizes<>> > a;

	type_of<Choose<2, int, float, char, double>> x;
	Choose<1, float, char, double> t;
	Choose<0, char, double> y;
	Choose<0, double> h;

	Tuple<int, float, const char*, int, int> test;

	test.get<0>() = 1;
	test.get<1>() = 2.2365f;
	test.get<2>() = "Hello World!";

	cout << test.size() << endl;
	cout << test.get<0>() << endl;
	cout << test.get<1>() << endl;
	cout << test.get<2>() << endl;
}
#endif

#pragma endregion
