#include <errno.h>
#include <stdbool.h>
#include <posix/time.h>

#include "base/clock_tod.h"

#define ZEPHYR_SECS_PER_MINUTE (60UL)
#define ZEPHYR_MINUTE_PER_HOUR (60UL)
#define ZEPHYR_SECS_PER_HOUR   (ZEPHYR_SECS_PER_MINUTE * ZEPHYR_MINUTE_PER_HOUR)
#define ZEPHYR_HOURS_PER_DAY   (24UL)
#define ZEPHYR_SECS_PER_DAY    (ZEPHYR_SECS_PER_HOUR * ZEPHYR_HOURS_PER_DAY)
#define ZEPHYR_DAYS_PER_YEAR   (365UL)
#define ZEPHYR_YEAR_BASE       (1970UL)

/*
 *  The following array contains the number of days in all months
 *  up to the month indicated by the index of the second dimension.
 *  The first dimension should be 1 for leap years, and 0 otherwise.
 */
static const uint16_t tod_days_to_date[2][13] = {
  { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
  { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 }
};


static bool leap_year(uint32_t year)
{
    return (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
}

static uint32_t leap_years_before(uint32_t year)
{
    year -= 1;
    return (year / 4) - (year / 100) + (year / 400);
}

static uint32_t leap_years_between(uint32_t from, uint32_t to)
{
    return leap_years_before(to) - leap_years_before(from + 1);
}

static uint32_t year_day_as_month(uint32_t year, uint32_t *day)
{
    const uint16_t* days_to_date;
    uint32_t        month = 0;

    if ( leap_year( year ) )
        days_to_date = tod_days_to_date[1];
    else
        days_to_date = tod_days_to_date[0];

    days_to_date += 2;

    while (month < 11) {
        if (*day < *days_to_date)
            break;
        ++month;
        ++days_to_date;
    }

    *day -= *(days_to_date - 1);
    return month;
}

int clock_get_tod(struct time_tod *time)
{
    struct timespec now;
    uint32_t days;
    uint32_t day_secs;
    uint32_t year;
    uint32_t year_days;
    uint32_t leap_years;

    if ( !time )
        return -EINVAL;

    /* Obtain the current time */
    clock_gettime(CLOCK_REALTIME, &now);

    /* How many days and how many seconds in the day ? */
    days = now.tv_sec / ZEPHYR_SECS_PER_DAY;
    day_secs = now.tv_sec % ZEPHYR_SECS_PER_DAY;

    /* How many non-leap year years ? */
    year = (days / ZEPHYR_DAYS_PER_YEAR) + ZEPHYR_YEAR_BASE;

    /* Determine the number of leap years. */
    leap_years = leap_years_between(ZEPHYR_YEAR_BASE, year);

    /* Adjust the remaining number of days based on the leap years. */
    year_days = (days - leap_years) % ZEPHYR_DAYS_PER_YEAR;

    /* Adjust the year and days in the year if in the leap year overflow. */
    if (leap_years > (days % ZEPHYR_DAYS_PER_YEAR)) {
        year -= 1;
        if (leap_year(year))
            year_days += 1;
    }

    time->year   = year;
    time->month  = year_day_as_month( year, &year_days ) + 1;
    time->day    = year_days + 1;
    time->hour   = day_secs / ZEPHYR_SECS_PER_HOUR;
    time->minute = day_secs % ZEPHYR_SECS_PER_HOUR;
    time->second = time->minute % ZEPHYR_SECS_PER_MINUTE;
    time->minute = time->minute / ZEPHYR_SECS_PER_MINUTE;
    time->ticks  = now.tv_nsec / (1000000000ul / CONFIG_SYS_CLOCK_TICKS_PER_SEC);

    return 0;
}

