#include "defines.h"
#include "core\logging.h"
#include "core\assertion.h"


int main()
{
    S_FATAL("Hello, %i", 10);
    S_ERROR("Hello, %i", 10);
    S_WARN("Hello, %i", 10);
    S_INFO("Hello, %i", 10);
    S_DEBUG("Hello, %i", 10);
    S_TRACE("Hello, %i", 10);

    return 0;
}