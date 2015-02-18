#include "Timer.hpp"

using namespace HIC;

// Default constructor for Timer
Timer::Timer(float endTime) :
	mEndTime(endTime)
{
}

// Destructor for Timer
Timer::~Timer()
{
}

// Update the timer by increasing its timer by the given frame time
void Timer::Update(float frameTime)
{
	mCurrentTime += frameTime;
}

// Returns true if the timer has reached its end time
bool Timer::IsComplete()
{
	return mCurrentTime >= mEndTime;
}

// Resets the timer to zero
void Timer::Reset()
{
	mCurrentTime = 0.0f;
}
