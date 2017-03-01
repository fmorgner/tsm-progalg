#ifndef RWLock_h
#define RWLock_h

#include <condition_variable>
#include <mutex>
#include <iostream>

struct RWLock
  {
  size_t getReaders() const
    {
    // TODO
    return 0;
    }

  void lockR()
    {
    // TODO
    }

  void unlockR()
    {
    // TODO
    }

  void lockW()
    {
    // TODO
    }

  void unlockW()
    {
    // TODO
    }

  private:
    std::mutex mutable m_mutex{};
    std::condition_variable m_readingAllowed{};
    std::condition_variable m_writingAllowed{};
    size_t m_readLocked{};
    bool m_writeLocked{};
  };

#endif
