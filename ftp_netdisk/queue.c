#include "queue.h"
int queue_create(queue *q,int size)
{
	if(q==NULL) return -1;
	*q=(queue)calloc(sizeof(queue_member)+size,1);
	(*q)->num=0;
	(*q)->size=size;
	queue_pmember pm=(queue_pmember)calloc(sizeof(queue_member)+size,1);
	(*q)->back=(*q)->front=pm;
	return 0;
}
int queue_push(queue q,void *p)
{
	if(q==NULL&&p==NULL) return -1;
	queue_pmember pm=(queue_pmember)calloc(sizeof(queue_member)+q->size,1);
	memcpy(pm+1,p,q->size);
	q->back->next=pm;
	q->back=pm;
	q->num++;
	return 0;
}
int queue_pop(queue q)
{
	if(q==NULL||!q->num) return -1;
	queue_pmember pm=q->front->next;
	q->front->next=pm->next;
	free(pm);
	q->num--;
	if(!q->num) q->back=q->front;
	return 0;
}
int queue_front(queue q,void *p)
{
	if(q==NULL||!q->num||p==NULL) return -1;
	memcpy(p,q->front->next+1,q->size);
	return 0;
}
int queue_back(queue q,void *p)
{
	if(q==NULL||!q->num||p==NULL) return -1;
	memcpy(p,q->back+1,q->size);
	return 0;
}
int queue_destroy(queue *q)
{
	if(q==NULL||(*q)==NULL) return -1;
	queue_pmember pm;
	while((*q)->back!=NULL)
	{
		pm=(*q)->back;
		(*q)->back=(*q)->back->next;
		(*q)->num--;
		free(pm);
	}
	free(*q);
	*q=NULL;
	return 0;
}
