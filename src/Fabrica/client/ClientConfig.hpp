#ifndef FABRICA_CLIENT_CLIENTCONFIG_HPP_
#define FABRICA_CLIENT_CLIENTCONFIG_HPP_

#include "../common/Configuration.hpp"

namespace fab
{

/**
 * @brief Configuration file for the client side
 */
class ClientConfig final
{
public:
	ClientConfig(boost::filesystem::path file): c(file) {}

	void read();
	void write();

	/**
	 * @brief Singleton Instance of the Client configuration.
	 */
	static ClientConfig& instance();

	// Variables
	
	int windowWidth, windowHeight;
	float naviSpeedPara;
	float naviSpeedPerp;
	float naviSpeedVert;

private:
	Configuration c;
};


} // namespace fab

#endif // !FABRICA_CLIENT_CLIENTCONFIG_HPP_
