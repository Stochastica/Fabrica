#ifndef FABRICA_COMMON_FABRICA_HPP_
#define FABRICA_COMMON_FABRICA_HPP_

#include <string>

#define FABRICA_NAME "Fabrica"
/**
 * @brief Version string of Fabrica.
 */
#define FABRICA_VERSION "0.0.1"

/*
 * Define the macro FABRICA_SERVER_STANDALONE to switch to standalone server
 * compilation.
 */
//#define FABRICA_SERVER_STANDALONE

namespace fab
{

/**
 * @brief String for carrying player messages and names.
 */
typedef std::string String;

} // namespace fab

#endif // !FABRICA_COMMON_FABRICA_HPP_
