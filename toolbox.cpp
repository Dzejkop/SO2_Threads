#include "toolbox.hpp"
#include <unistd.h>

void Toolbox::Sleep(unsigned int time)
{
    usleep(time);
}