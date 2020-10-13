#pragma once
#include <iostream>
#include <tuple>  
#include <time.h>

#pragma once
#define HAS_MEMBER(member)\
template<typename T, typename... Args>struct has_member_##member\
{\
private:\
		template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	template<typename U> static std::false_type Check(...);\
public:\
	enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

//template<class T, typename... Args>
//using has_before_t = decltype(std::declval<T>().before(std::declval<Args>()...));

template<typename Func, typename... Args>
struct Aspect 
{
	Aspect(Func&& f) : m_func(std::forward<Func>(f))
	{
	}

	template<typename T>
	typename std::enable_if<has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
		m_func(std::forward<Args>(args)...);//核心逻辑
		aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
	}

	template<typename T>
	typename std::enable_if<has_member_Before<T, Args...>::value && !has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
		m_func(std::forward<Args>(args)...);//核心逻辑
	}

	template<typename T>
	typename std::enable_if<!has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		m_func(std::forward<Args>(args)...);//核心逻辑
		aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
	}

	template<typename Head, typename... Tail>
	void Invoke(Args&&... args, Head&&headAspect, Tail&&... tailAspect)
	{
		headAspect.Before(std::forward<Args>(args)...);
		Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
		headAspect.After(std::forward<Args>(args)...);
	}

private:
	Func m_func; //被织入的函数
};
template<typename T> using identity_t = T;

//AOP的辅助函数，简化调用
template<typename... AP, typename... Args, typename Func>
void Invoke(Func&&f, Args&&... args)
{
	Aspect<Func, Args...> asp(std::forward<Func>(f));
	asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

struct TimeElapsedAspect
{
	void Before(int i)
	{
	}

	void After(int i)
	{
		std::cout << "time elapsed: " << std::endl;
	}

private:
	double m_lastTime;

};

struct LoggingAspect
{
	void Before(int i)
	{
		std::cout << "entering" << std::endl;
	}

	void After(int i)
	{
		std::cout << "leaving" << std::endl;
	}
};

struct testhasmem
{
	std::wstring before()
	{
		std::cout << "entering" << std::endl;
		return L"aaa";
	}

	void after(int i)
	{
		std::cout << "leaving" << std::endl;
	}
};

struct testhasmem2
{
	std::wstring before2()
	{
		std::cout << "entering" << std::endl;
		return L"aaa";
	}

	void after(int i)
	{
		std::cout << "leaving" << std::endl;
	}
};

void foo(int a)
{
	std::cout << "real HT function: " << a << std::endl;
}


template< typename T>
struct filter_helper
{
	static constexpr auto func()
	{
		return std::tuple<>();
	}

	template< class... Args >
	static constexpr auto func(T&&, Args&&...args)
	{
		return filter_helper::func(std::forward<Args>(args)...);
	}

	template< class X, class... Args >
	static constexpr auto func(X&&x, Args&&...args)
	{
		return std::tuple_cat(std::make_tuple(std::forward<X>(x)), filter_helper::func(std::forward<Args>(args)...));
	}
};

template<typename T, typename... Args>
inline auto filter(Args&&... args) {
	return filter_helper<T>::func(std::forward<Args>(args)...);
}

class request {
public:
};

class response {
public:
};

	template<class AlwaysVoid,
		template<class...> class Op, class... Args>
	struct detector {
		using value_t = std::false_type;
	};

	template< template<class...> class Op, class... Args>
	struct detector< std::void_t<Op<Args...>>, Op, Args...> {
		using value_t = std::true_type;
	};

	template<template<class...> class Op, class... Args>
	using is_detected = typename detector<void, Op, Args...>::value_t;

	template<class T, typename... Args>
	using has_before_t = decltype(std::declval<T>().before(std::declval<Args>()...));

	template<class T, typename... Args>
	using has_after_t = decltype(std::declval<T>().after(std::declval<Args>()...));


template<typename T, typename... Args>
using has_before = is_detected<has_before_t, T, Args...>;

template<typename T, typename... Args>
using has_after = is_detected<has_after_t, T, Args...>;

//template <typename... Args, typename F, std::size_t... Idx>
//constexpr void for_each_l(std::tuple<Args...>& t, F&& f, std::index_sequence<Idx...>) {
//	(std::forward<F>(f)(std::get<Idx>(t)), ...);
//}

//class AOP
//{
//public:
//	template<typename Function, typename... AP>
//	typename std::enable_if<has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type invoke(request& req, response& res, Function&& f, AP&&... ap)
//	{
//		std::tuple<AP...> tp(std::move(ap)...);
//	}
//};
