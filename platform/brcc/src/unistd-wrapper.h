// Wrapper file to help deal with flex requiring a POSIXish build environment.
//
// (note that some newer versions wrap the #include <unistd.h> conditionally
// to avoid this problem, but many build environments still use older versions
// so we include this hackery here to make the build more portable)

#ifdef _WIN32

// insert any definitions needed from the real unistd.h here - isatty() is 
// quite commonly required, but it depends on the specific flex version
// you have installed

#else

extern "C" {
#include <unistd.h>
}

#endif
