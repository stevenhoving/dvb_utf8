#ifndef stop_watch_h__
#define stop_watch_h__

#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class stop_watch
{
public:
    stop_watch()
        : resolution_(0.0)
        , sampled_time_(0.0)
        , start_time_(0)
    {
        std::int64_t freq;

        // Check if we have a performance counter
        if( QueryPerformanceFrequency( (LARGE_INTEGER *)&freq ) )
        {
            // Counter resolution is 1 / counter frequency
            resolution_ = 1.0 / (double)freq;

            // Set start time for timer
            (void)QueryPerformanceCounter( (LARGE_INTEGER *)&start_time_ );
        }
    }

    void time_start()
    {
        //store so we can easily do something like delta time
        sampled_time_ = _get_time();
    }

    double time_since()
    {
        return _get_time() - sampled_time_;
    }

protected:
    double _get_time()
    {
        double  t;
        int64_t t_64;

        (void)QueryPerformanceCounter( (LARGE_INTEGER *)&t_64 );

        t = (double)(t_64 - start_time_);

        /* Calculate the current time */
        return t * resolution_;
    }

private:
    double resolution_;
    double sampled_time_;
    int64_t start_time_;
};
#endif // stop_watch_h__
