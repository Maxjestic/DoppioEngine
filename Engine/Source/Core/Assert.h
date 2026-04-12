#pragma once

#include <print>

#ifdef _MSC_VER

#define DE_DEBUG_BREAK() __debugbreak()

#elif defined(__clang__) || defined(__GNUC__)

#define DE_DEBUG_BREAK() __builtin_trap()

#else

#include <csignal>
#define DE_DEBUG_BREAK() raise(SIGTRAP)

#endif

#ifdef DOPPIO_DEBUG

#define DE_CORE_ASSERT(Condition, ...)							\
	do {														\
	if (!(Condition)) {											\
	std::print( stderr, "\x1b[31m[ASSERT FAILED]\x1b[0m " );	\
	std::println( stderr, __VA_ARGS__ );                        \
	DE_DEBUG_BREAK();                                           \
	}															\
	} while (0)

#else

#define DE_CORE_ASSERT(Condition, ...)

#endif
