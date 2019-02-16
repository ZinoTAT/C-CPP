#ifndef __QUEUQ_H__
#define __QUEUQ_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct queue_member{
	struct queue_member *next;
	int size;
}queue_member,*queue_pmember;
typedef struct{
	queue_pmember front,back;
	int num,size;
}queue_head,*queue_phead,*queue;
int queue_create(queue *,int size);//(第一个参数为申请的queue，第二个参数为每个成员的大小，第三个参数为该队列大部分时候成员数量的估计值，不确定也可直接填0由系统自动设置
int queue_push(queue,void *);
int queue_pop(queue);
int queue_front(queue q,void*);
int queue_back(queue q,void *);
int queue_destroy(queue *q);
//void *queue_front(void *,int len);

#endif
