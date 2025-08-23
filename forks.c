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
        usleep(100 * p->id);
        if(p->left < p->right)
        {
                pthread_mutex_lock(&p->sim->forks[p->left]);
                print_state(p, "has taken a fork");
                pthread_mutex_lock(&p->sim->forks[p->right]);
                print_state(p, "has taken a fork");
        }
        else
        {
                pthread_mutex_lock(&p->sim->forks[p->right]);
                print_state(p, "has taken a fork");
                pthread_mutex_lock(&p->sim->forks[p->left]);
                print_state(p, "has taken a fork"); 
        }

}

void put_forks(struct s_philo *p)
{
        pthread_mutex_unlock(&p->sim->forks[p->left]);
        pthread_mutex_unlock(&p->sim->forks[p->right]);
}
