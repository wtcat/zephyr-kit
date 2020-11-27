#ifndef BASE_CLOCK_TOD_H_
#define BASE_CLOCK_TOD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

struct time_tod {
  uint32_t   year;
  uint32_t   month;
  uint32_t   day;
  uint32_t   hour;
  uint32_t   minute;
  uint32_t   second;
  uint32_t   ticks;
};

int clock_get_tod(struct time_tod *time);

#ifdef __cplusplus
}
#endif
#endif /* BASE_CLOCK_TOD_H_ */
