#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* config */
struct s_conf
{
        int n;
        long time_die;
        long time_eat;
        long time_sleep;
        int must_eat; /* -1 if not provided */
};

/* sim state (no globals) */
struct s_sim
{
        struct s_conf conf;
        long start_ms;

        int stop; /* guarded by stop_mx */
        pthread_mutex_t stop_mx;

        pthread_mutex_t print_mx; /* serialize logs */
        pthread_mutex_t *forks;   /* n mutexes */

        int done; /* count reached must_eat */
        pthread_mutex_t done_mx;
};

/* philosopher */
struct s_philo
{
        int id;    /* 1..n */
        int left;  /* fork index */
        int right; /* fork index */

        long last_meal_ms; /* guarded by mx */
        int meals;
        pthread_mutex_t mx;

        pthread_t th;
        struct s_sim *sim;
};

/* args */
int parse_args(int ac, char **av, struct s_conf *conf);

/* time */
long now_ms(void);
long since_start(struct s_sim *sim);
int msleep_check(struct s_philo *p, long ms); /* 1ms slices + self-death check */

/* print / stop */
void print_state(struct s_philo *p, const char *msg); /* exact lines, serialized */
int get_stop(struct s_sim *sim);
void set_stop(struct s_sim *sim, int v);

/* forks */
void take_forks(struct s_philo *p);
void put_forks(struct s_philo *p);

/* life */
void *run(void *arg);

#endif
