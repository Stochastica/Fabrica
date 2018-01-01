#ifndef FABRICA_UTIL_FACING_HPP_
#define FABRICA_UTIL_FACING_HPP_

#include <cassert>
#include <cstdint>

#include "vector.hpp"

namespace fab
{

#define ENUM_FLAG(Enum, IType) \
	constexpr inline Enum operator|(Enum a, Enum b) \
	{ \
		return static_cast<Enum>(static_cast<IType>(a) | static_cast<IType>(b));\
	} \
	constexpr inline Enum operator&(Enum a, Enum b) \
	{ \
		return static_cast<Enum>(static_cast<IType>(a) & static_cast<IType>(b));\
	}


/**
 * @brief Represents six sides of a cube
 * @warning Combinations of Facings, such as DOWN | UP, are used as flags only.
 */
enum class Facing3: std::uint8_t
{
	Down  = 1,  // ( 0,-1, 0)
	Up    = 2,  // ( 0, 1, 0)
	North = 4,  // ( 0, 0,-1)
	South = 8,  // ( 0, 0, 1)
	West  = 16, // (-1, 0, 0)
	East  = 32  // ( 1, 0, 0)
};

// Macro for iterating through Facing3 values
#define FACING3_VALUES {Facing3::Down, \
		Facing3::Up, \
		Facing3::North, \
		Facing3::South, \
		Facing3::West, \
		Facing3::East} \

ENUM_FLAG(Facing3, std::uint8_t)

constexpr bool isEmpty(Facing3 f)
{
	return 0 == static_cast<std::uint8_t>(f);
}
constexpr int toShortInt(Facing3 f)
{
	switch (f)
	{
	case Facing3::Down:  return 0;
	case Facing3::Up:    return 1;
	case Facing3::North: return 2;
	case Facing3::South: return 3;
	case Facing3::West:  return 4;
	case Facing3::East:  return 5;
	}
}
inline Vector3i toFace(Facing3 f)
{
	switch (f)
	{
	case Facing3::Down:  return Vector3i( 0,-1, 0);
	case Facing3::Up:    return Vector3i( 0, 1, 0);
	case Facing3::North: return Vector3i( 0, 0,-1);
	case Facing3::South: return Vector3i( 0, 0, 1);
	case Facing3::West:  return Vector3i(-1, 0, 0);
	case Facing3::East:  return Vector3i( 1, 0, 0);
	}
}
constexpr Facing3 opposite(Facing3 f)
{
	switch (f)
	{
	case Facing3::Down:  return Facing3::Up;
	case Facing3::Up:    return Facing3::Down;
	case Facing3::North: return Facing3::South;
	case Facing3::South: return Facing3::North;
	case Facing3::West:  return Facing3::East;
	case Facing3::East:  return Facing3::West;
	}
}

// Tests
static_assert(!isEmpty(Facing3::Down | Facing3::North),
              "util/facing.hpp: Facing flag check failed");
static_assert(isEmpty((Facing3::North | Facing3::South | Facing3::East) &
                      Facing3::Down),
              "util/facing.hpp: Facing flag check failed");
static_assert(!isEmpty((Facing3::North | Facing3::South | Facing3::East) &
                      Facing3::East),
              "util/facing.hpp: Facing flag check failed");


} // namespace fab

#endif // !FABRICA_UTIL_FACING_HPP_
