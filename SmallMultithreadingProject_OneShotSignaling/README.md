# One-Shot Signaling — Multithreading Practice

## Concepts Covered

- `std::condition_variable` — blocks a thread until a condition is met
- `std::mutex` + `std::lock_guard` — protect shared state when setting the flag
- `std::unique_lock` — required by `cv.wait()` since it must unlock/relock mid-wait
- Spurious wakeup protection — predicate `[]{ return ready; }` re-checks the flag on every wakeup
- Lost wakeup protection — same predicate catches signals sent before `wait()` is reached
- `std::promise<void>` — one-shot signal sender; fulfills exactly once via `set_value()`
- `std::future<void>` — receiver side; blocks on `wait()` until the promise is fulfilled
- `std::move` on future — futures are not copyable, ownership must be transferred into the thread

## What Was Built

**First implementation — `std::condition_variable`**  
Requires a mutex, a condition variable, and a boolean flag. Worker calls `condVar.wait(lock, predicate)`. The main thread sets the flag inside a lock, releases the lock, then calls `notify_one()`.

**Second implementation — `std::promise<void>`**  
Worker calls `startFuture.wait()`. The main thread calls `startSignal.set_value()`. No mutex, no flag, no predicate needed. One-shot behaviour is encoded in the type itself.

## Reflection

| Aspect | `condition_variable` | `promise<void>` |
|---|---|---|
| Needed types | mutex + cv + bool flag | promise + future |
| Spurious wakeup protection | manual predicate required | built into type |
| Lost wakeup protection | flag check in predicate | built into type |
| Reusable after signal | yes — reset the flag | no — one-shot only |
| Best for | repeated signals | one-time signals |