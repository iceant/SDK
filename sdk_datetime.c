#include <sdk_datetime.h>

time_t sdk_datetime_current_ms(void){
#if defined(_WIN32) || defined(_WIN64)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (time_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (time_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
}


