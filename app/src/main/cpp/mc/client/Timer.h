//
// Created by TheChuan1503 on 2025/7/18.
//

#ifndef CIRCLORALPHA_TIMER_H
#define CIRCLORALPHA_TIMER_H

#include <functional>

class Timer {
public:
    typedef float (*getTimeScale_t) (Timer*);

    float mTicksPerSecond;
    int mTicks;
    float mAlpha;
    float mTimeScale;
    float mPassedTime;
    float mFrameStepAlignmentRemainder;
    float mLastTimeSeconds;
    float mLastTimestep;
    float mOverflowTime;
    int64_t mLastMs;
    int64_t mLastMsSysTime;
    float mAdjustTime;
    float mSteppingTick;
    std::function<int64_t()> mGetTimeMSCallback;
};


#endif //CIRCLORALPHA_TIMER_H
