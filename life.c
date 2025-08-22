#include "philo.h"

static void mark_done_if_needed(struct s_philo *p)
{
        struct s_sim *s = p->sim;

        if (s->conf.must_eat < 0)
                return;
        pthread_mutex_lock(&p->mx);
        if (p->meals == s->conf.must_eat)
        {
                pthread_mutex_unlock(&p->mx);
                pthread_mutex_lock(&s->done_mx);
                s->done += 1;
                if (s->done == s->conf.n)
                        set_stop(s, 1);
                pthread_mutex_unlock(&s->done_mx);
        }
        else
                pthread_mutex_unlock(&p->mx);
}

static int eat_act(struct s_philo *p)
{
        print_state(p, "is eating");
        pthread_mutex_lock(&p->mx);
        p->last_meal_ms = now_ms();
        p->meals += 1;
        pthread_mutex_unlock(&p->mx);
        if (msleep_check(p, p->sim->conf.time_eat))
                return (1);
        mark_done_if_needed(p);
        return (0);
}

static void think_act(struct s_philo *p) { print_state(p, "is thinking"); }

static int sleep_act(struct s_philo *p)
{
        print_state(p, "is sleeping");
        return (msleep_check(p, p->sim->conf.time_sleep));
}

static void solo(struct s_philo *p)
{
        think_act(p);
        pthread_mutex_lock(&p->sim->forks[p->left]);
        print_state(p, "has taken a fork");
        msleep_check(p, p->sim->conf.time_die + 1);
        pthread_mutex_unlock(&p->sim->forks[p->left]);
}

void *run(void *arg)
{
        struct s_philo *p = (struct s_philo *)arg;
        struct s_conf *c = &p->sim->conf;

        if (p->id % 2 == 0)
                usleep(500);
        if (c->n == 1)
                return (solo(p), NULL);
        while (!get_stop(p->sim))
        {
                think_act(p);
                take_forks(p);
                if (eat_act(p))
                {
                        put_forks(p);
                        break;
                }
                put_forks(p);
                if (sleep_act(p))
                        break;
        }
        return (NULL);
}
