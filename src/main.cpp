#include <App.hpp>

#ifdef __linux
#include <X11/Xlib.h>
#endif // __linux

int main()
{
#ifdef __linux
    XInitThreads();
#endif //__linux

    App app;
    app.run();

    return 0;
}