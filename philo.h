#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* config */
typedef struct s_conf
{
        int n;
        long time_die;
        long time_eat;
        long time_sleep;
        int must_eat; /* -1 if not provided */
}t_conf;

/* sim state (no globals) */
typedef struct s_sim
{
        struct s_conf conf;
        long start_ms;

        int stop; /* guarded by stop_mx */
        pthread_mutex_t stop_mx;

        pthread_mutex_t print_mx; /* serialize logs */
        pthread_mutex_t *forks;   /* n mutexes */

        int done; /* count reached must_eat */
        pthread_mutex_t done_mx;
}t_sim;

/* philosopher */
typedef struct s_philo
{
        int id;    /* 1..n */
        int left;  /* fork index */
        int right; /* fork index */

        long last_meal_ms; /* guarded by mx */
        int meals;
        pthread_mutex_t mx;

        pthread_t th;
        struct s_sim *sim;
}t_philo;

/* args */
int parse_args(int ac, char **av, t_conf *conf);

/* time */
long now_ms(void);
long since_start(t_sim *sim);
int msleep_check(t_philo *p, long ms); /* 1ms slices + self-death check */

/* print / stop */
void print_state(t_philo *p, const char *msg); /* exact lines, serialized */
int get_stop(t_sim *sim);
void set_stop(t_sim *sim, int v);

/* forks */
void take_forks(t_philo *p);
void put_forks(t_philo *p);

/* life */
void *run(void *arg);

#endif
