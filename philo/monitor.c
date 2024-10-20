/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiambon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:45:27 by rgiambon          #+#    #+#             */
/*   Updated: 2024/10/20 18:57:54 by rgiambon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ending_sim(t_data *data, t_philo *philo, int i)
{
	pthread_mutex_lock(&data->is_dead);
	data->is_over = 1;
	pthread_mutex_unlock(&data->is_dead);
	pthread_mutex_lock(&data->printer);
	printf("[%u] %d %s is dead.%s\n", get_sim_time(data), \
	philo[i].idx + 1, RED, RESET);
	pthread_mutex_unlock(&data->printer);
	pthread_mutex_unlock(&philo[i].meal);
}

void	monitor(t_philo *philo, t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->is_dead);
	while (!data->is_over && !data->all_are_done)
	{
		pthread_mutex_unlock(&data->is_dead);
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_lock(&philo[i].meal);
			if (get_sim_time(data) - philo[i].last_meal > data->time_to_die)
			{
				ending_sim(data, philo, i);
				return ;
			}
			pthread_mutex_unlock(&philo[i].meal);
			all_are_done(philo, data);
			i++;
		}
		pthread_mutex_lock(&data->is_dead);
	}
	pthread_mutex_unlock(&data->is_dead);
	pthread_mutex_lock(&data->printer);
	printf("Every philo had at least %d meals.\n", data->n_meals);
	pthread_mutex_unlock(&data->printer);
}

void	all_are_done(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	if (data->n_meals >= 0)
	{
		while (i < data->philo_num)
		{
			pthread_mutex_lock(&philo[i].meal);
			if (philo[i].meals_eaten < data->n_meals)
			{
				pthread_mutex_unlock(&philo[i].meal);
				return ;
			}
			pthread_mutex_unlock(&philo[i].meal);
			i++;
		}
		pthread_mutex_lock(&data->are_done);
		data->all_are_done = 1;
		pthread_mutex_unlock(&data->are_done);
	}
}
