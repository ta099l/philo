#include "philo.h"

void set_stop(struct s_sim *sim, int v)
{
        pthread_mutex_lock(&sim->stop_mx);
        sim->stop = v;
        pthread_mutex_unlock(&sim->stop_mx);
}

/* order lock: min->max to avoid deadlock */
void take_forks(struct s_philo *p)
{
        int a = (p->left < p->right) ? p->left : p->right;
        int b = p->left ^ p->right ^ a;

        pthread_mutex_lock(&p->sim->forks[a]);
        print_state(p, "has taken a fork");
        pthread_mutex_lock(&p->sim->forks[b]);
        print_state(p, "has taken a fork");
}

void put_forks(struct s_philo *p)
{
        pthread_mutex_unlock(&p->sim->forks[p->left]);
        pthread_mutex_unlock(&p->sim->forks[p->right]);
}
