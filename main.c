/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarahgh8 <sarahgh8@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 02:36:36 by sarahgh8          #+#    #+#             */
/*   Updated: 2025/08/23 03:30:53 by sarahgh8         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	void 	init_ph(t_philo **out_ph, t_sim *sim)
{
	int i;

	i = -1;
	while (++i < sim->conf.n)
	{
		(*out_ph)[i].id = i + 1;
		(*out_ph)[i].left = i;
		(*out_ph)[i].right = (i + 1) % sim->conf.n;
		(*out_ph)[i].sim = sim;
		(*out_ph)[i].meals = 0;
		(*out_ph)[i].last_meal_ms = sim->start_ms;
		pthread_mutex_init(&(*out_ph)[i].mx, NULL);
	}
}

static int	init_sim(struct s_sim *sim)
{
	int	i;

	sim->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sim->conf.n);
    if(!sim->forks)
		return (1);
	pthread_mutex_init(&sim->print_mx, NULL);
	pthread_mutex_init(&sim->stop_mx, NULL);
	pthread_mutex_init(&sim->done_mx, NULL);
	i = -1;
	while (++i < sim->conf.n)
		pthread_mutex_init(&sim->forks[i], NULL);
	sim->stop = 0;
	sim->done = 0;
	return (0);
}

static void	destroy_all(struct s_sim *sim, struct s_philo *ph)
{
	int	i;

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

int	main(int ac, char **av)
{
	struct s_sim	sim;
	struct s_philo	*ph;
	int				i;

	if (parse_args(ac, av, &sim.conf))
		return (1);
	ph = malloc(sizeof(struct s_philo) * sim.conf.n);
	if (init_sim(&sim))
		return (1);
	sim.start_ms = now_ms();
	init_ph(&ph, &sim);
	i = 0;
	while (i < sim.conf.n)
	{
		pthread_create(&ph[i].th, NULL, run, &ph[i]);
		i++;
	}
	i = 0;
	while (i < sim.conf.n)
	{
		pthread_join(ph[i].th, NULL);
		i++;
	}
	destroy_all(&sim, ph);
	return (0);
}
