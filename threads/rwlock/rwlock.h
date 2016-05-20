#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _rwlock
{
  pthread_mutex_t lock;
  pthread_cond_t reader_entry;
  pthread_cond_t writer_entry;

  unsigned int reader_count;
  unsigned int writer_count;
  unsigned int blocked_writer_count;
} rwlock_t;

void rwlock_init(rwlock_t *rwlock);
void rwlock_destroy(rwlock_t *rwlock);

void rwlock_rdlock(rwlock_t *rwlock);
void rwlock_rdunlock(rwlock_t *rwlock);
void rwlock_wrlock(rwlock_t *rwlock);
void rwlock_wrunlock(rwlock_t *rwlock);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RWLOCK_H */
