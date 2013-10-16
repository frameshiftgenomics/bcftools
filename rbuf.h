#ifndef __RBUF_H__
#define __RBUF_H__

typedef struct
{
    int m,n,f;    // m: allocated size, n: number of elements in the buffer, f: first element
}
rbuf_t;

/**
 *  rbuf_init() - initialize round buffer
 *  @rbuf:  the rbuf_t holder
 *  @size:  the maximum number of elements
 *
 */
static inline void rbuf_init(rbuf_t *rbuf, int size)
{
    rbuf->m = size; rbuf->n = rbuf->f = 0;
}
/**
 *  rbuf_last() - get index of the last element of the round buffer
 *  @rbuf:  the rbuf_t holder
 *
 */
static inline int rbuf_last(rbuf_t *rbuf)
{
    if ( !rbuf->n ) return -1;
    int i  = rbuf->n + rbuf->f - 1;
    if ( i >= rbuf->m ) i -= rbuf->m;
    return i;
}
/**
 *  rbuf_next() - get index of the next element in the round buffer
 *  @rbuf:  the rbuf_t holder
 *  @i:     pointer to the last rbuf index. Set to -1 before the first call.
 *
 *  Sets i to the next position in the buffer. The return value indicates if
 *  the position points to a valid element (1) or if there are no more elements
 *  after *i (0). When reaching the end, *i is set to the first element in the
 *  buffer.
 */
static inline int rbuf_next(rbuf_t *rbuf, int *i)
{
    if ( !rbuf->n ) return 0;
    if ( *i==-1 ) { *i = rbuf->f; return 1; }
    int n = (rbuf->f <= *i) ? *i - rbuf->f + 1 : *i + rbuf->m - rbuf->f + 1;
    if ( ++(*i) >= rbuf->m ) *i = 0;
    if ( n < rbuf->n ) return 1;
    *i = rbuf->f;
    return 0;
}
/**
 *  rbuf_prev() - get index of the previous element in the round buffer
 *  @rbuf:  the rbuf_t holder
 *  @i:     pointer to the last rbuf index. Set to -1 before the first call.
 *
 *  Sets i to the previous position in the buffer. The return value indicates if
 *  the position points to a valid element (1) or if there are no more elements
 *  before *i (0).
 */
static inline int rbuf_prev(rbuf_t *rbuf, int *i)
{
    if ( !rbuf->n || *i==rbuf->f ) return 0;
    if ( *i==-1 )
    {
        *i = rbuf_last(rbuf);
        return 1;
    }
    if ( --(*i) < 0 ) *i = rbuf->m - 1;
    return 1;
}
/**
 *  rbuf_add() - register new element in the round buffer
 *  @rbuf:  the rbuf_t holder
 *
 *  Returns index of the newly inserted element.
 */
static inline int rbuf_add(rbuf_t *rbuf)
{
    if ( rbuf->n < rbuf->m )
    {
        rbuf->n++;
        int i = rbuf->f + rbuf->n;
        return i <= rbuf->m ? i - 1 : i - rbuf->m - 1;
    }

    rbuf->f++;
    if ( rbuf->f >= rbuf->m ) 
    {
        rbuf->f = 0;
        return rbuf->m - 1;
    }
    return rbuf->f - 1;
}
/**
 *  rbuf_shift() - removes first element of the buffer
 *  @rbuf:  the rbuf_t holder
 *
 *  Returns index of the removed element.
 */
static inline int rbuf_shift(rbuf_t *rbuf)
{
    if ( !rbuf->n ) return -1;
    int ret = rbuf->f;
    rbuf->f++;
    if ( rbuf->f >= rbuf->m ) rbuf->f = 0;
    rbuf->n--;
    return ret;
}

#endif
