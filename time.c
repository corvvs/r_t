#include "tetris.h"

suseconds_t	usec(t_timeval* t)
{
	return (suseconds_t)(t->tv_sec * 1000000 + t->tv_usec);
}

suseconds_t	diff_time(t_timeval *t0, t_timeval *t1)
{
	return (usec(t1) - usec(t0));
}