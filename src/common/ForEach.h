#ifndef GP_COMMON_FOREACH_H
#define GP_COMMON_FOREACH_H

namespace gp
{
  
template<size_t N>
struct IntToType {};
typedef IntToType<true> ForEachContinueLoop;
typedef IntToType<false> ForEachTerminateLoop;

template<template<size_t> class Functor, size_t COUNT, typename... Args>
void for_each(Args... args)
{
  for_each<Functor, 0, COUNT, Args...>(IntToType<(0 < COUNT)>(), args...);
}

template<template<size_t> class Functor, size_t INDEX, size_t COUNT, typename... Args>
void for_each(ForEachContinueLoop, Args... args)
{
  Functor<INDEX> functor;
  functor(args...);
  for_each<Functor, INDEX+1, COUNT, Args...>(IntToType<(INDEX+1 < COUNT)>(), args...);
}

template<template<size_t> class, size_t, size_t, typename... Args>
void for_each(ForEachTerminateLoop, Args...)
{
}

}

#endif // GP_COMMON_FOREACH_H
