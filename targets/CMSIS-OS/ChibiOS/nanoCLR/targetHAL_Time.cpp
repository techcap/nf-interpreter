//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL_Types.h>
#include <nanoHAL_Time.h>
#include <target_platform.h>
#include <hal.h>
#include <ch.h>

// Converts CMSIS sysTicks to .NET ticks (100 nanoseconds)
signed __int64 HAL_Time_SysTicksToTime(unsigned int sysTicks) {
    
    // this is a rewrite of ChibiOS ST2US(n) macro because it will overflow if doing the math using uint32_t
    
    // convert to microseconds from CMSIS SysTicks
    int64_t microsecondsFromSysTicks = (((sysTicks) * 1000000ULL + (int64_t)CH_CFG_ST_FREQUENCY - 1ULL) / (int64_t)CH_CFG_ST_FREQUENCY);

    // need to convert from microseconds to 100 nanoseconds
    return  microsecondsFromSysTicks * 10;
}

// Returns the current date time from the system tick or from the RTC if it's available (this depends on the respective configuration option)
signed __int64  HAL_Time_CurrentDateTime(bool datePartOnly)
{
#if defined(HAL_USE_RTC)

    // use RTC to get date time
    SYSTEMTIME st; 
    RTCDateTime _dateTime;

    rtcGetTime(&RTCD1, &_dateTime);

    st.wDay = (unsigned short) _dateTime.day;
    st.wMonth = (unsigned short) _dateTime.month;
    st.wYear = (unsigned short) _dateTime.year;
    st.wDayOfWeek = (unsigned short) _dateTime.dayofweek;

    // zero 'time' fields if date part only is required
    if(datePartOnly)
    {
        st.wMilliseconds = 0;
        st.wSecond = 0;
        st.wMinute = 0;
        st.wMinute = 0;
    }
    else
    {
        // full date&time required, fill in 'time' fields too
        
        st.wMilliseconds =(unsigned short) (_dateTime.millisecond % 1000);
        _dateTime.millisecond /= 1000;
        st.wSecond = (unsigned short) (_dateTime.millisecond % 60);
        _dateTime.millisecond /= 60;
        st.wMinute = (unsigned short) (_dateTime.millisecond % 60);
        _dateTime.millisecond /= 60;
        st.wMinute = (unsigned short) (_dateTime.millisecond % 24);
    }

    return HAL_Time_ConvertFromSystemTime( &st );

#else

    // use system ticks
    return HAL_Time_SysTicksToTime( HAL_Time_CurrentSysTicks() );

#endif
};
