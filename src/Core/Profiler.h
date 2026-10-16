
#pragma once

#ifdef ENABLE_TRACY
    #include <tracy/Tracy.hpp>

    #define SE_PROFILE_SCOPE       ZoneScoped
    #define SE_PROFILE_SCOPE_N(n)  ZoneScopedN(n)
    #define SE_PROFILE_FRAME       FrameMark
    #define SE_PROFILE_VALUE(n, v) ZoneValue(v)
#else
    // Profiling disabled: no Tracy include or instrumentation.
    #define SE_PROFILE_SCOPE
    #define SE_PROFILE_SCOPE_N(n)
    #define SE_PROFILE_FRAME
    #define SE_PROFILE_VALUE(n, v)
#endif
