#include "testUtil.hpp"

#include "../testing.hpp"
#include "../../util/vector.hpp"

namespace fab
{

bool test_u1()
{
	int const radiusXZ = 3;
	int const radiusY = 2;
	int const size = (2 * radiusXZ - 1) * (2 * radiusXZ - 1) * (2 * radiusY - 1);
	Vector3i* order = genChunkLoadOrder(radiusXZ, radiusY);

	for (int i = 0; i < size; ++i)
	{
		int x = order[i].x();
		int y = order[i].y();
		int z = order[i].z();
		assert(-radiusXZ + 1 <= x && x <= radiusXZ - 1);
		assert(-radiusXZ + 1 <= z && z <= radiusXZ - 1);
		assert(-radiusY + 1 <= y && y <= radiusY - 1);
		std::cout << '[' << x << ", " << y << ", " << z << "]\n";
	}
	delete[] order;
	return true;
}

bool testUtil(std::string id)
{
	TEST_FUNC(u1);

	TEST_FINAL;
}

} // namespace fab
