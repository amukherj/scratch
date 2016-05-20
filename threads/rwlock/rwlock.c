#include "rwlock.h"
#include "assert.h"

#ifdef __cplusplus
extern "C" {
#endif

void rwlock_init(rwlock_t *rwlock)
{
  pthread_mutex_init(&rwlock->lock, NULL);
  pthread_cond_init(&rwlock->reader_entry, NULL);
  pthread_cond_init(&rwlock->writer_entry, NULL);

  rwlock->reader_count = rwlock->writer_count = rwlock->blocked_writer_count = 0;
}

void rwlock_destroy(rwlock_t *rwlock)
{
  pthread_cond_destroy(&rwlock->reader_entry);
  pthread_cond_destroy(&rwlock->writer_entry);
  pthread_mutex_destroy(&rwlock->lock);
}

void rwlock_rdlock(rwlock_t *rwlock)
{
  pthread_mutex_lock(&rwlock->lock);

  // as long as there is no writer and no blocked threads
  while (rwlock->writer_count > 0 
#ifdef RWLOCK_WRITEPREF
      || rwlock->blocked_writer_count > 0
#endif
      ) {
    pthread_cond_wait(&rwlock->reader_entry, &rwlock->lock);
  }

  // add some TSD key
  ++(rwlock->reader_count);
  pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_rdunlock(rwlock_t *rwlock)
{
  pthread_mutex_lock(&rwlock->lock);

  assert(rwlock->reader_count > 0);
  // do TSD-based asserts
  if (--(rwlock->reader_count) == 0) {
    pthread_cond_broadcast(&rwlock->writer_entry);
  }

  pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_wrlock(rwlock_t *rwlock)
{
  pthread_mutex_lock(&rwlock->lock);

  while (rwlock->reader_count > 0 || rwlock->writer_count > 0) {
    // do TSD checks
    ++(rwlock->blocked_writer_count);
    pthread_cond_wait(&rwlock->writer_entry, &rwlock->lock);
    --(rwlock->blocked_writer_count);
  }

  ++(rwlock->writer_count);

  pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_wrunlock(rwlock_t *rwlock)
{
  pthread_mutex_lock(&rwlock->lock);

  assert(rwlock->writer_count > 0);
  // do TSD-based asserts
  if (--(rwlock->writer_count) == 0) {
    pthread_cond_broadcast(&rwlock->reader_entry);

    if (rwlock->blocked_writer_count) {
      pthread_cond_signal(&rwlock->writer_entry);
    }
  }

  pthread_mutex_unlock(&rwlock->lock);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

