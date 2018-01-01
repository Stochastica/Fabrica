#ifndef FABRICA_COMMON_CONFIGMANAGER_HPP_
#define FABRICA_COMMON_CONFIGMANAGER_HPP_

#include <stack>

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace fab
{

/**
 * @brief Represents a configuration that can be read.
 *
 * Modules obtain Configurations by using a {@code StateEvent}. This class
 * loads a property tree (XML) from the configuration file. Subtrees in the
 * property tree are pushed into a stack.
 *
 * The correct way to use this class is (with {@code Configuration config;})
 *
 * @code
 * // Default values
 * val1 = 12;
 * val2 = 24;
 *
 * // Reading
 * config.fileRead();
 *
 * if (config.readSubtree("tree1"))
 * {
 *	config.read(&val1, "val1");
 *	config.read(&val2, "val2");
 *
 *	config.popSubtree();
 * }
 * config.popSubtree();
 *
 * @endcode
 *
 * @code
 * // Writing
 *
 * config.pushTree();
 *
 * config.beginSubtree()
 * {
 *	config.write(val1, "val1");
 *	config.write(val2, "val2");
 * }
 * config.endSubtree("tree1");
 *
 * config.fileWrite();
 * config.popSubtree();
 *
 * @endcode
 */
class Configuration final
{
public:
	/**
	 * @brief Loads an empty {@code Configuration} object. For placeholder.
	 */
	Configuration() = default;
	Configuration(boost::filesystem::path file);

	/**
	 * @brief Pops the top subtree from the stack.
	 * @warning Do not call this if the stack is empty.
	 *
	 * This method should be called after each subtree reading/writing is
	 * complete, and a final invocation after all reading/writing is complete.
	 */
	void popSubtree();

	// Reading functions
	/**
	 * @brief Reads the property tree from the config file.
	 * @return True if the file can be read.
	 *
	 * Call this before reading in any property. If the file cannot be read, an
	 * empty tree will be pushed into the stack.
	 */
	bool fileRead();
	/**
	 * @brief Reads a property from the top of the stack.
	 * @param[out] p
	 *	The property being read. If the property does not exist in the tree, the
	 *	{@code *p} is unchanged.
	 * @param[in] name Name of the property.
	 */
	template <typename T> void read(T* p, std::string name);

	/**
	 * @brief Reads a subtree of the property tree and push it into the stack.
	 * @warning If this returns false, the stack is unchanged.
	 * @return True if the subtree exists.
	 *
	 * This function should be coupled with {@code popSubtree()}
	 */
	bool readSubtree(std::string name);

	/**
	 * @brief Pushes the "base tree" into the tree stack
	 */
	void pushTree();

	// Writing functions
	/**
	 * Pushes a subtree whose parent is the top of the stack and of given name
	 * into the stack.
	 */
	void beginSubtree();

	/**
	 * Pop the topmost subtree and add it as a child to the second topmost
	 * subtree.
	 */
	void endSubtree(std::string name);

	/**
	 * @brief Writes a property to the top of the stack.
	 */
	template <typename T> void write(T const& p, std::string name);

	/**
	 * @brief Writes the entire property tree to the config file.
	 */
	void fileWrite();


private:
	std::stack<boost::property_tree::ptree> trees;
	boost::filesystem::path file;
};

// Implementations

inline Configuration::Configuration(boost::filesystem::path file):
	file(file)
{
}
inline void Configuration::popSubtree()
{
	trees.pop();
}
template <typename T> inline void
Configuration::read(T* p, std::string name)
{
	*p = trees.top().get<T>(name, *p);
}
inline bool Configuration::readSubtree(std::string name)
{
	auto subtree = trees.top().get_child_optional(name);
	if (subtree)
	{
		trees.push(*subtree);
		return true;
	}
	else
		return false;
}
inline void Configuration::pushTree()
{
	assert(trees.empty());
	trees.push(boost::property_tree::ptree());
}
inline void Configuration::beginSubtree()
{
	trees.push(boost::property_tree::ptree());
}
inline void Configuration::endSubtree(std::string name)
{
	boost::property_tree::ptree subtree(std::move(trees.top()));
	trees.pop();
	trees.top().put_child(name, subtree);
}
template <typename T> inline void
Configuration::write(T const& p, std::string name)
{
	trees.top().put<T>(name, p);
}

} // namespace fab

#endif // !FABRICA_COMMON_CONFIGMANAGER_HPP_
