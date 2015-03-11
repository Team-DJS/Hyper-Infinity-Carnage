#include "Timer.hpp"

using namespace HIC;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Constructor for Timer
Timer::Timer(float endTime) :
	mEndTime(endTime),
	mCurrentTime(0.0f)
{
}

// Destructor for Timer
Timer::~Timer()
{
}

//-----------------------------------
// Other Functions
//-----------------------------------

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
