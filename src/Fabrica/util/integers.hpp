#ifndef FABRICA_UTIL_INTEGERS_HPP_
#define FABRICA_UTIL_INTEGERS_HPP_

namespace fab
{

/**
 * @brief Divides two integers, return the floor.
 */
constexpr int divide_floor(int x, int y)
{
	int q = x / y;
	int r = x % y;
	if ((r != 0) && ((r < 0) != (y < 0))) --q;
	return q;
}

constexpr int modulo_floor(int x, int y)
{
	int r = x % y;
	if ((r != 0) && ((r < 0) != (y < 0))) { r += y; }
	return r;
}

} // namespace fab

#endif // !FABRICA_UTIL_INTEGERS_HPP_
