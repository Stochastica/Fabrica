#ifndef FABRICA_TEST_TEST_HPP_
#define FABRICA_TEST_TEST_HPP_

#include <map>
#include <string>

namespace fab
{

/**
 * @brief Returns a map of possible test id's and their descriptions.
 */
std::map<std::string, std::string> testInfo();

/**
 * Execute test [id].
 *
 * Depends on the first character of id, executes different tests:
 *	c: Client
 *	s: Server
 * 
 * The id with first character removed will be send to the
 * subordinating test functions.
 *
 * e.g. test("cr1") calls testClient("r1").
 */
bool test(std::string id);

} // namespace fab

#endif // !FABRICA_TEST_TEST_HPP_
