#include "philo.h"

long now_ms(void)
{
        struct timeval tv;
        gettimeofday(&tv, 0);
        return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long since_start(struct s_sim *sim)
{
        return (now_ms() - sim->start_ms);
}

/* returns 1 if stop/death, else 0 */
static int check_death_or_stop(struct s_philo *p)
{
        long last, diff;

        if (get_stop(p->sim))
                return (1);
        pthread_mutex_lock(&p->mx);
        last = p->last_meal_ms;
        pthread_mutex_unlock(&p->mx);
        diff = now_ms() - last;
        if (diff > p->sim->conf.time_die)
        {
                pthread_mutex_lock(&p->sim->stop_mx);
                if (!p->sim->stop)
                {
                        p->sim->stop = 1;
                        pthread_mutex_unlock(&p->sim->stop_mx);
                        print_state(p, "died"); /* within ~10ms */
                }
                else
                        pthread_mutex_unlock(&p->sim->stop_mx);
                return (1);
        }
        return (0);
}

/* sleep in ~1ms slices; check between slices (meets ≤10ms death log) */
int msleep_check(struct s_philo *p, long ms)
{
        long end = now_ms() + ms;

        while (now_ms() < end)
        {
                if (check_death_or_stop(p))
                        return (1);
                usleep(1000);
        }
        return (check_death_or_stop(p));
}
