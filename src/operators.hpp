#pragma once

template<typename A, typename B>
inline bool operator == (const A& a, const B& b)
{
	return !(a < b) && !(b < a);
}
template<typename A, typename B>
inline bool operator != (const A& a, const B& b)
{
	return (a < b || b < a);
}
template<typename A, typename B>
inline bool operator <= (const A& a, const B& b)
{
	return a < b || !(b < a);
}
template<typename A, typename B>
inline bool operator > (const A& a, const B& b)
{
	return b < a;
}
template<typename A, typename B>
inline bool operator >= (const A& a, const B& b)
{
	return b < a || !(a < b);
}
