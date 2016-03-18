#ifndef REDSTRAIN_MOD_UTIL_STATIC_ASSERT_HPP
#define REDSTRAIN_MOD_UTIL_STATIC_ASSERT_HPP

#define REDSTRAIN_STATIC_ASSERT(name, condition) typedef char _ ## name[(condition) ? 1 : -1]

#endif /* REDSTRAIN_MOD_UTIL_STATIC_ASSERT_HPP */
