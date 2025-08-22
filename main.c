#include "philo.h"

static int init_all(struct s_sim *sim, struct s_philo **out_ph)
{
        int i;

        if (!(sim->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sim->conf.n)))
                return (write(2, "Error\n", 6), 1);
        *out_ph = (struct s_philo *)malloc(sizeof(struct s_philo) * sim->conf.n);
        if (!*out_ph)
                return (free(sim->forks), write(2, "Error\n", 6), 1);
        pthread_mutex_init(&sim->print_mx, NULL);
        pthread_mutex_init(&sim->stop_mx, NULL);
        pthread_mutex_init(&sim->done_mx, NULL);
        i = 0;
        while (i < sim->conf.n)
        {
                pthread_mutex_init(&sim->forks[i], NULL);
                i++;
        }
        sim->stop = 0;
        sim->done = 0;
        sim->start_ms = now_ms();
        i = 0;
        while (i < sim->conf.n)
        {
                (*out_ph)[i].id = i + 1;
                (*out_ph)[i].left = i;
                (*out_ph)[i].right = (i + 1) % sim->conf.n;
                (*out_ph)[i].sim = sim;
                (*out_ph)[i].meals = 0;
                (*out_ph)[i].last_meal_ms = sim->start_ms;
                pthread_mutex_init(&(*out_ph)[i].mx, NULL);
                i++;
        }
        return (0);
}

static void destroy_all(struct s_sim *sim, struct s_philo *ph)
{
        int i;
        i = 0;
        while (i < sim->conf.n)
        {
                pthread_mutex_destroy(&sim->forks[i]);
                i++;
        }
        i = 0;
        while (i < sim->conf.n)
        {
                pthread_mutex_destroy(&ph[i].mx);
                i++;
        }
        pthread_mutex_destroy(&sim->print_mx);
        pthread_mutex_destroy(&sim->stop_mx);
        pthread_mutex_destroy(&sim->done_mx);
        free(sim->forks);
        free(ph);
}

int main(int ac, char **av)
{
        struct s_sim sim;
        struct s_philo *ph;
        int i;

        if (parse_args(ac, av, &sim.conf))
                return (1);
        if (init_all(&sim, &ph))
                return (1);
        i = 0;
        while(i < sim.conf.n)
        {
                pthread_create(&ph[i].th, NULL, run, &ph[i]);
                i++;        
        }
        while (i < sim.conf.n)
        {
                pthread_join(ph[i].th, NULL);
                i++;
        }
        destroy_all(&sim, ph);
        return (0);
}
