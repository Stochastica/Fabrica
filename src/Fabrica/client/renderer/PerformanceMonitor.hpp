#ifndef FABRICA_CLIENT_RENDERER_PERFORMANCEMONITOR_HPP_
#define FABRICA_CLIENT_RENDERER_PERFORMANCEMONITOR_HPP_

#include <atomic>
#include <chrono>

namespace fab
{

class PerformanceMonitor final
{
public:
	PerformanceMonitor();

	void update(); ///< Called every frame

	float getDuration() const noexcept { return duration; }
	float getFPS() const noexcept { return fps; }
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> timeStamp;
	float duration;
	float fps;
};

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_PERFORMANCEMONITOR_HPP_
