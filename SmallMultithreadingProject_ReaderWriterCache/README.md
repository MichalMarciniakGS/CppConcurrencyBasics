Reader-Writer Cache - Multithreading Practice

Concepts covered:
- std::shared_mutex - allows multiple simultaneous reades, one writer
- std::shared_lock - acquired by readers, concurrent with other shared_lock's
- std::unique_lock - acquired by writers, exclusive, blocks other locks
- mutable mutex member - allows locking inside const method
- double check for locking - safe cache check logic between read lock / write lock
- std::jthread - RAII thread with automatic join on destruction usage
- FIFO eviciton - insertion order tracking via list, bounded by max size