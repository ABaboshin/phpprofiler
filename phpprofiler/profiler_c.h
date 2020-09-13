#pragma once

struct Profiler;

#ifdef __cplusplus
extern "C"
#endif

Profiler* createProfilerInstance();

#ifdef __cplusplus
extern "C"
#endif
void injectLoader(Profiler*);
