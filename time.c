#include <sys/time.h>

typedef struct timeval t_timeval;

static suseconds_t	usec(t_timeval* t)
{
	return (suseconds_t)(t->tv_sec * 1000000 + t->tv_usec);
}

suseconds_t			get_current_time()
{
	t_timeval	t;
	gettimeofday(&t, NULL);
	return usec(&t);
}
