#ifndef FABRICA_UTIL_RESOURCELOCATION_HPP_
#define FABRICA_UTIL_RESOURCELOCATION_HPP_

#include <string>

namespace fab
{

/**
 * @brief Represents the location of an asset.
 *
 * {@code ResourceLocation} are resolved via {@code class ModuleLoader}, so
 * Modules need to be first loaded before any resolution of
 * {@code ResourceLocation}.
 */
class ResourceLocation
{
public:
	ResourceLocation() = default;
	/**
	 * Creates a {@code ResourceLocation} instance with domain and path.
	 */
	ResourceLocation(std::string domain,
	                 std::string path):
		domain(domain), path(path) {}

	std::string string() const noexcept;
	std::string getDomain() const noexcept
	{
		return domain;
	}
	std::string getPath() const noexcept
	{
		return path;
	}

protected:
	std::string domain, path;
};

bool operator <(ResourceLocation const&, ResourceLocation const&) noexcept;

// Implementations

inline std::string
ResourceLocation::string() const noexcept
{
	return domain + ':' + path;
}
inline bool
operator <(ResourceLocation const& rl0, ResourceLocation const& rl1) noexcept
{
	if (rl0.getDomain() < rl1.getDomain()) return true;
	else if (rl0.getDomain() > rl1.getDomain()) return false;
	else return rl0.getPath() < rl1.getPath();
}

} // namespace fab

#endif // !FABRICA_UTIL_RESOURCELOCATION_HPP_
