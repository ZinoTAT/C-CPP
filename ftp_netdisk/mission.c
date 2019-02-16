#include"mission.h"
int useMission(queue q,void *ptask,pthread_mutex_t *plock,pthread_cond_t* pcond)
{
	pthread_mutex_lock(plock);
	while(!q->num)
	{
		pthread_cond_wait(pcond,plock);
	}
	queue_front(q,ptask);
	queue_pop(q);
	pthread_mutex_unlock(plock);
	return 0;
}
int addMission(queue q,void *ptask,pthread_mutex_t *plock,pthread_cond_t* pcond)
{
	pthread_mutex_lock(plock);
	queue_push(q,ptask);
	pthread_cond_signal(pcond);
	pthread_mutex_unlock(plock);
	return 0;

}
