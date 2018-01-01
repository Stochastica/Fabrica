#include "Configuration.hpp"

#include <iostream>
#include <fstream>

#include <boost/property_tree/xml_parser.hpp>

namespace fab
{

bool Configuration::fileRead()
{
	namespace bpt = boost::property_tree;

	trees.push(bpt::ptree());
	std::ifstream in;
	in.open(file.string());
	if (!in.is_open()) return false;

	try
	{
		bpt::read_xml(in, trees.top(), bpt::xml_parser::trim_whitespace);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	in.close();

	return true;
}
void Configuration::fileWrite()
{
	namespace bpt = boost::property_tree;

	bpt::xml_writer_settings<std::string> settings('\t', 1);

	try
	{
		bpt::write_xml(file.string(),
		               trees.top(),
		               std::locale(),
		               settings);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

} // namespace fab
