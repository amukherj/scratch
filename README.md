#### Caveat
This implementation can be abused in the sense that you could call rwlock_rdunlock from a thread that didn't call rwlock_rdlock. Likewise for write locks. This can be easily fixed by using thread-specific data and keeping track of whether a thread holds a lock using that thread-specific data.
