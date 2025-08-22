#include "philo.h"

static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static long parse_pos_long(const char *s)
{
        long v = 0;
        int i = 0;

        if (!s || !s[0])
                return (-1);
        if (s[0] == '+')
                i++;
        while (s[i])
        {
                if (!is_digit(s[i]))
                        return (-1);
                v = v * 10 + (s[i] - '0');
                if (v < 0)
                        return (-1);
                i++;
        }
        return (v);
}

int parse_args(int ac, char **av, struct s_conf *conf)
{
        long v;

        if (ac != 5 && ac != 6)
                return (write(2, "Error\n", 6), 1);
        v = parse_pos_long(av[1]);
        if (v <= 0)
                return (write(2, "Error\n", 6), 1);
        conf->n = (int)v;
        v = parse_pos_long(av[2]);
        if (v <= 0)
                return (write(2, "Error\n", 6), 1);
        conf->time_die = v;
        v = parse_pos_long(av[3]);
        if (v <= 0)
                return (write(2, "Error\n", 6), 1);
        conf->time_eat = v;
        v = parse_pos_long(av[4]);
        if (v <= 0)
                return (write(2, "Error\n", 6), 1);
        conf->time_sleep = v;
        if (ac == 6)
        {
                v = parse_pos_long(av[5]);
                if (v <= 0)
                        return (write(2, "Error\n", 6), 1);
                conf->must_eat = (int)v;
        }
        else
                conf->must_eat = -1;
        return (0);
}
