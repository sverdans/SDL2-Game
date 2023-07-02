#include <chrono>

class Time
{
private:
	Time() = default;
	~Time() = default;

	std::chrono::_V2::system_clock::time_point mPreviousTime;
	std::chrono::_V2::system_clock::time_point mCurrentTime;
	
	double mUpdateDuration;
	double mDeltaTime;

public:
	Time(const Time&) = delete;
	Time(Time&&) = delete;

	Time& operator=(const Time&) = delete;
	Time& operator=(Time&&) = delete;

	static Time& Instance()
	{
		static Time instance;
		return instance;
	}

	void SetUpdateFrequency(const double fps)
	{
		mUpdateDuration = 1000.0 / fps;
	}

	void Start()
	{
		mPreviousTime = std::chrono::high_resolution_clock::now();
	}

	void Update()
	{
		mCurrentTime = std::chrono::high_resolution_clock::now();
		mDeltaTime = std::chrono::duration<double, std::milli>(mCurrentTime - mPreviousTime).count();
	}

	bool Time::CheckFPS()
	{
		if (mDeltaTime >= mUpdateDuration)
		{
			mPreviousTime = mCurrentTime;
			return true;
		}
		
		return false;
	}

	double DeltaTime() { return mDeltaTime; }
};
