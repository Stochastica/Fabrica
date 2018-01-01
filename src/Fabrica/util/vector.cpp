#include "vector.hpp"

#include <algorithm>

namespace fab
{

Vector3i* genChunkLoadOrder(int radiusXZ, int radiusY) noexcept
{
	assert(radiusXZ > 0 && radiusY > 0 && "util/vector.cpp: Invalid dimensions");

	int const sizeXZ = 2 * radiusXZ - 1;
	int const sizeY = 2 * radiusY - 1;
	Vector3i* v = new Vector3i[sizeXZ * sizeXZ * sizeY];

	// Fill Vector array
	for (int x = -radiusXZ + 1; x <= radiusXZ - 1; ++x)
	{
		int const offsetX = (x + radiusXZ - 1) * sizeXZ;
		for (int z = -radiusXZ + 1; z <= radiusXZ - 1; ++z)
		{
			int const offsetZ = (offsetX + z + radiusXZ - 1) * sizeY;
			for (int y = -radiusY + 1; y <= radiusY - 1; ++y)
			{
				int const offsetY = offsetZ + y + radiusY - 1;

				assert(offsetY < sizeXZ * sizeXZ * sizeY);

				v[offsetY] = Vector3i(x, y, z);
			}
		}
	}

	// Sort by norm
	std::sort(v, v + (sizeXZ * sizeXZ * sizeY),
	          [](Vector3i const& u, Vector3i const& v)
	{
		return u.squaredNorm() < v.squaredNorm();
	});
	return v;
}

} // namespace fab
