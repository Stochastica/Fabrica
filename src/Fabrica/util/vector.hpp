#ifndef FABRICA_UTIL_VECTOR_HPP_
#define FABRICA_UTIL_VECTOR_HPP_

#include <Eigen/Dense>

namespace fab
{

typedef Eigen::Vector2f Vector2f;
typedef Eigen::Vector2i Vector2i;

//typedef Eigen::Vector2f Vector2f;
typedef Eigen::Vector3f Vector3f;
typedef Eigen::Vector3i Vector3i;

template <typename T, std::size_t n>
using Matrix = Eigen::Matrix<T, n, n>;

/**
 * @brief Generates the order for chunk loading.
 * @param[in] radiusXZ Horizontal radius
 * @param[in] radiusY Vertical radius
 * @return An array of all vectors [x,y,z] such that
 *	-radiusXZ < x < +radiusXZ
 *	-radiusXZ < z < +radiusXZ
 *	 -radiusY < y < +radiusY
 *	sorted by their distances to [0,0,0].
 */
Vector3i* genChunkLoadOrder(int radiusXZ, int radiusY) noexcept;

} // namespace fab

#endif // !FABRICA_UTIL_VECTOR_HPP_
