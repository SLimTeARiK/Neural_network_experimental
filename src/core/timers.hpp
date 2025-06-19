

//template<typename T>

    struct timespec ts;

inline float setCurrentTime() {
    float timer;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    timer = ts.tv_sec  + ts.tv_nsec / 1.0e9;
    return timer;
}

inline void setCurrentTime(float &time) {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    time = ts.tv_sec  + ts.tv_nsec / 1.0e9;
    ts.tv_sec = 0;ts.tv_nsec = 0;
}



static struct timespec times[2];

inline void setCurrentTimeBegin() {
    clock_gettime(CLOCK_MONOTONIC, &times[0]);
}

inline void setCurrentTimeEnd(double& timer) {
    clock_gettime(CLOCK_MONOTONIC, &times[1]);
    timer = (unsigned)((times[1].tv_sec - times[0].tv_sec) + (times[1].tv_nsec - times[0].tv_nsec) / 1.0e9);
}
