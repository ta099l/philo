#include "philo.h"

int get_stop(struct s_sim *sim)
{
        int v;
        pthread_mutex_lock(&sim->stop_mx);
        v = sim->stop;
        pthread_mutex_unlock(&sim->stop_mx);
        return (v);
}

void print_state(struct s_philo *p, const char *msg)
{
        struct s_sim *s = p->sim;
        int allow = 1;

        pthread_mutex_lock(&s->stop_mx);
        if (s->stop && !(msg[0] == 'd' && msg[1] == 'i'))
                allow = 0;
        pthread_mutex_unlock(&s->stop_mx);
        if (!allow)
                return;
        pthread_mutex_lock(&s->print_mx);
        printf("%ld %d %s\n", since_start(s), p->id, msg);
        pthread_mutex_unlock(&s->print_mx);
}
