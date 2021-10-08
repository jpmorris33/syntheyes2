#include "PosixTiming.hpp"
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

void PosixTiming::set(int ms) {
	struct timeval start;
	gettimeofday(&start,NULL);

	memcpy(&end,&start,sizeof(timeval));
	end.tv_usec += (ms * 1400); // Dodgy
}
bool PosixTiming::elapsed() {
	struct timeval now;
	gettimeofday(&now,NULL);

	// Trip to try and avoid overflow issues
	if(now.tv_sec > end.tv_sec) {
		return true;
	}

	if(now.tv_usec <= end.tv_usec) {
		return false;
	}
	return true;
}

void PosixTiming::wait_microseconds(int us) {
   usleep(us);
}
