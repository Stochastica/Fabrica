#ifndef FABRICA_TEST_TESTING_HPP_
#define FABRICA_TEST_TESTING_HPP_

/*
 * Unit testing utilities
 */

#include <iostream>

#define TEST_FINAL \
	std::cerr << "Invalid Test Id\n"; \
	return false

/**
 * If the variable "id" is equal to symb, executes the test function
 * test_symb().
 */
#define TEST_FUNC(symb) if (id == #symb) return test_##symb()

#endif // !FABRICA_TEST_TESTING_HPP_
