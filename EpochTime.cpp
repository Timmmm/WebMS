#include "EpochTime.h"

#include <sys/time.h>

unsigned long long CurrentEpochTime()
{
	timeval tv;
	int ret = gettimeofday(&tv, 0);
	if (ret != 0)
		return 0;

	return tv.tv_sec;
}
