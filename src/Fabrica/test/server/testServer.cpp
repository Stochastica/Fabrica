#include "testServer.hpp"

#include <iostream>

#include "../testing.hpp"

namespace fab
{

bool test_c0()
{
	return true;
}

bool testServer(std::string id)
{
	TEST_FUNC(c0);
	std::cerr << "Invalid Test Id\n";
	return true;
}

} // namespace fab

