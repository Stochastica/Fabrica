#ifndef FABRICA_API_STATEEVENTS_STATEEVENT_HPP_
#define FABRICA_API_STATEEVENTS_STATEEVENT_HPP_

#include <string>

namespace fab
{

/**
 * @brief Signals a stage change in module loading, e.g. pre-initialisation
 */
class StateEvent
{
public:
	~StateEvent() = default;

protected:

	/**
	 * @brief Name of the module this state event will be fed into.
	 */
	std::string modName;

private:
	void setName(std::string n) noexcept { modName = n; }

	friend class ModuleLoader;
};

// Implementations

} // namespace fab

#endif // !FABRICA_API_STATEEVENTS_STATEEVENT_HPP_
