#ifndef FABRICA_COMMON_LOGMANAGER_HPP_
#define FABRICA_COMMON_LOGMANAGER_HPP_

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>

#include <boost/core/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/tee.hpp>

namespace fab
{

class LogManager; // Forward declaration

/**
 * @brief Provides a thread-safe, time marked alternative to std::cout.
 *
 * Example: If out is a std::ostream, creating a {@code Logger} with signature
 * "Test" and outputing "test message" can be done with:
 *
 * @code{.cpp}
 *
 * LogManager lm(&out);
 * Logger l = lm.create("Test");
 * l << "test message";
 *
 * ...
 */
class Logger final
{
public:
	// Creates a default logger
	Logger(): logManager(nullptr) {}
	/**
	 * @brief Prints a message
	 * Outputs a message through the logger. There will automatically be a line
	 * break at the end.
	 *
	 * The format is
	 * [Y-m-d H:M:S][head]message
	 */
	void operator()(std::string message);

	/**
	 * @brief Similar to {@code operator()(std::string)} but prepends [WARNING]
	 */
	void warn(std::string message);
	void error(std::string message);

private:
	Logger(LogManager* const, std::string head);

	LogManager* logManager;
	std::string head;

	friend class LogManager;
};

/**
 * The singleton {@code LogManager} creates all the {@code Loggers}. It outputs
 * to std::cout in a non-thread safe way, and thus only one instance should be
 * present in the entire program.
 */
class LogManager final: boost::noncopyable
{
public:

	/**
	 * @brief Splits the {@code LogManager}'s output to the given file.
	 * @warning Do not call this function twice if it succeeds the first time.
	 * @return True if successful.
	 */
	bool splitOutput(std::string file);

	/**
	 * @brief Create a new {@code Logger} with a signature.
	 * @param[in] head
	 *  A signature of the created {@code Logger}.
	 *
	 * This function can be called even when LogManager is not initialised.
	 */
	static Logger create(std::string head);
	/**
	 * @brief Return the singleton instance.
	 */
	static LogManager& instance();

private:
	LogManager(); // Deleted constructor
	/**
	 * Print a message. The message must have no newlines.
	 */
	void print(std::string message);

	std::ostream* out;
	std::ofstream outFile;
	std::mutex mutex;

	friend class Logger;
};

// Implementations

inline void Logger::operator()(std::string message)
{
	assert(logManager && "Logger is not initialised");
	logManager->print(head + message);
}
inline void Logger::warn(std::string message)
{
	assert(logManager && "Logger is not initialised");
	logManager->print(head + "[WARNING]" + message);
}
inline void Logger::error(std::string message)
{
	assert(logManager && "Logger is not initialised");
	logManager->print(head + "[ERROR]" + message);
}
inline Logger::Logger(LogManager* const lm, std::string head):
	logManager(lm), head(head)
{
	assert(lm);
}

// LogManager

inline bool LogManager::splitOutput(std::string file)
{
	assert(out == &std::cout && !outFile.is_open() &&
	       "Do not call this function twice.");

	outFile.open(file.c_str());
	if (!outFile.is_open())
	{
		return false;
	}
	typedef boost::iostreams::tee_device<std::ostream, std::ofstream> Tee;
	Tee tee(std::cout, outFile);
	out = new boost::iostreams::stream<Tee>(tee);
	return true;
}
inline LogManager::LogManager(): out(&std::cout) {}
inline void LogManager::print(std::string message)
{
	auto t = std::time(nullptr);
	auto time = *std::localtime(&t);

	mutex.lock();
	(*out) << std::put_time(&time, "[%Y-%m-%d %H:%M:%S]")
	       << message << std::endl;
	mutex.unlock();
}
inline Logger LogManager::create(std::string head)
{
	return Logger(&instance(), '[' + head + ']');
}
inline LogManager& LogManager::instance()
{
	static LogManager inst;
	return inst;
}


} // namespace fab

#endif // !FABRICA_COMMON_LOGMANAGER_HPP_
