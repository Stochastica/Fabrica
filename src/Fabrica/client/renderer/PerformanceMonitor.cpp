#include "PerformanceMonitor.hpp"

namespace fab
{

PerformanceMonitor::PerformanceMonitor():
	duration(1.f / 30.f), fps(30.f)
{
}
void PerformanceMonitor::update()
{
	namespace sc = std::chrono;
	auto timeStamp2 = sc::high_resolution_clock::now();
	duration = sc::duration_cast<sc::microseconds>(
	           timeStamp2 - timeStamp).count() * 0.000001f;
	timeStamp = timeStamp2;

	fps = fps * .95f + .05f / duration;
}

} // namespace fab
