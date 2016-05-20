#include "rwlock.h"
#include <unistd.h>
#include <stdio.h>

rwlock_t lock;

void* reader(void* arg)
{
  rwlock_rdlock(&lock);
  printf("%ld: Acquired read-lock\n", (long)arg);
  sleep(2);
  printf("%ld: Releasing read-lock\n", (long)arg);
  rwlock_rdunlock(&lock);

  return NULL;
}

void* writer(void* arg)
{
  rwlock_wrlock(&lock);
  printf("%ld: Acquired write-lock\n", (long)arg);
  sleep(2);
  printf("%ld: Releasing write-lock\n", (long)arg);
  rwlock_wrunlock(&lock);

  return NULL;
}

int main()
{
  pthread_t threads[10];

  rwlock_init(&lock);

  for (int i = 0; i < 10; ++i) {
    if ((i + 1) % 3 == 0) {
      pthread_create(&threads[i], NULL, writer, (void*)i);
    } else {
      pthread_create(&threads[i], NULL, reader, (void*)i);
    }
  }

  for (int i = 0; i < 10; ++i) {
    pthread_join(threads[i], NULL);
  }

  rwlock_destroy(&lock);
}
