#pragma once

namespace HIC
{
	class Timer
	{
	public:
		// Default constructor for Timer
		Timer(float endTime);

		// Destructor for Timer
		~Timer();
	public:
		// Update the timer by increasing its timer by the given frame time
		void Update(float frameTime);

		// Returns true if the timer has reached its end time
		bool IsComplete();

		// Resets the timer to zero
		void Reset();

		// Resets timer to zero and gives a new end time
		void Reset(float endTime);

	private:
		float mCurrentTime;	//The current time count for this timer
		float mEndTime;		//The end time for this timer
	};
}
