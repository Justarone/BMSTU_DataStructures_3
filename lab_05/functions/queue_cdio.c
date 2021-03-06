#include "queue_cdio.h"
#include <stdlib.h>
#include <stdio.h>

#define OK 0
#define OVERFLOW_ERROR 1
#define EMPTY_QUEUE 2
#define INSERTION_ERROR 3
#define LOST_APP 4

// this function receives pointer on the tail of the queue and appends
// one more element (returns new tail if successful else NULL)
node_t *push_l(queue_l *const queue, const int value)
{
    if (queue->size >= QUEUE_SIZE)
        return NULL;

    node_t *tmp = (node_t *)malloc(sizeof(node_t));
    // can't allocate memory for new element in queue
    if (!tmp)
        return NULL;

    tmp->next = NULL;
    tmp->data.value = value;
    // old tail points on the new one (the new tail is the last in queue)
    if (queue->pin)
        queue->pin->next = tmp;
    else
        queue->pout = tmp;
    queue->size++;
    return tmp;
}

// this function inserts element on the pos position of the list queue (or in the end
// if pos >= queue size)
int ins_l(queue_l *const queue, const double value, const int pos, void **const address, const double income_time)
{
    int err_code = OK;
    if (queue->size >= QUEUE_SIZE)
    {
        // removing last element of the queue to insert the
        // element of the second type
        int val;
        pop_l(queue, &val);
        err_code = LOST_APP;
    }

    if (queue->size <= pos)
    {
        node_t *res = push_l(queue, value);
        if (!res)
            return INSERTION_ERROR;
        *address = (void *)res;
        queue->pin = res;
        res->data.income_time = income_time;
        return err_code;
    }

    int i;
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (!node)
        return INSERTION_ERROR;
    *address = (void *)node;

    // insertion in the head of the queue
    if (pos == 0)
    {
        node->next = queue->pout;
        node->data.value = value;
        node->data.income_time = income_time;
        queue->pout = node;
        return err_code;
    }

    node_t *tmp;
    for (i = 0, tmp = queue->pout; i < pos - 1 && tmp->next; i++, tmp = tmp->next)
        ;
    queue->size++;
    node->next = tmp->next;
    tmp->next = node;
    node->data.value = value;
    node->data.income_time = income_time;
    return err_code;
}

static void ring_array_insert(queue_a *const queue, const int value, const int pos)
{
    int limit = queue->pin >= pos ? queue->pin : queue->pin + QUEUE_SIZE;
    int i = limit - 1;
    while (i >= pos)
    {
        queue->data[(i + 1) % QUEUE_SIZE] = queue->data[i % QUEUE_SIZE];
        i--;
    }
    queue->pin = (queue->pin + 1) % QUEUE_SIZE;
    queue->data[pos].value = value;
}

// this functions inserts the value element on the pos position in the
// array queue (or in the end of the queue if pos >= queue size)
int ins_a(queue_a *const queue, const int value, const int pos, const double income_time)
{
    int err_code;
    // remove last element of the queue if the queue is full (to insert new element)
    if (!queue->is_empty && queue->pin == queue->pout)
    {
        int val = 0;
        pop_a(queue, &val);
        err_code = OVERFLOW_ERROR;
    }
    else
        err_code = OK;

    int i;
    queue->is_empty = 0;
    int limit = queue->pin >= queue->pout ? queue->pin : queue->pin + QUEUE_SIZE;
    for (i = 0; i < pos + 1 && queue->pout + i < limit; i++)
        ;
    ring_array_insert(queue, value, (queue->pout + i) % QUEUE_SIZE);
    queue->data[(queue->pout + i) % QUEUE_SIZE].income_time = income_time;
    return err_code;
}

// this function appends new element in array-queue if it possible
// returns 0 if successful else error code
int push_a(queue_a *const queue, const int value)
{
    if (!queue->is_empty && queue->pin == queue->pout)
        return OVERFLOW_ERROR;
    queue->data[queue->pin].value = value;
    queue->pin = (queue->pin + 1) % QUEUE_SIZE;
    queue->is_empty = 0;
    return OK;
}

// this fucntion receives queue head, and return 0, if value is read
// (and also moves head to the next element)
// else error code (value returns with argument)
int pop_l(queue_l *const queue, int *const value)
{
    if (queue->size <= 0)
        return EMPTY_QUEUE;

    *value = queue->pout->data.value;
    node_t *tmp = queue->pout;
    queue->pout = queue->pout->next;
    free(tmp);
    queue->size--;
    if (queue->size == 0)
        queue->pin = NULL;
    return OK;
}

// this function moves pout index-pointer if it possible
// returns 1 if successful else error code
// (returns value using argument as functions higher)
int pop_a(queue_a *const queue, int *const value)
{
    if (queue->is_empty)
        return EMPTY_QUEUE;

    *value = queue->data[queue->pout].value;
    queue->pout = (queue->pout + 1) % QUEUE_SIZE;
    if (queue->pout == queue->pin)
        queue->is_empty = 1;
    return OK;
}

void print_queue_l(queue_l *const queue)
{
    if (queue->pout)
        printf("queue->pout is %p\nqueue->pout->next is %p\nand queue->pin is %p\n", queue->pout, queue->pout->next, queue->pin);
    node_t *tmp = queue->pout;
    while (tmp)
    {
        printf("<%d>\n", tmp->data.value);
        tmp = tmp->next;
    }
}

void print_queue(const queue_a *const queue)
{
    printf("pout = %d; pin = %d\n", queue->pout, queue->pin);
    if (queue->is_empty)
        return;
    int limit = (queue->pin > queue->pout) ? queue->pin : queue->pin + QUEUE_SIZE;
    for (int i = queue->pout; i < limit; i++)
        printf("%d %lf\n", queue->data[i % QUEUE_SIZE].value, queue->data[i % QUEUE_SIZE].income_time);
}