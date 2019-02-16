#ifndef __MISSION_H__
#define __MISSION_H__
#include "func.h"
int addMission(queue q,void *,pthread_mutex_t *plock,pthread_cond_t *);
int useMission(queue q,void *,pthread_mutex_t *plock,pthread_cond_t *);
#endif
