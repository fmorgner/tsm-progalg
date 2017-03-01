#ifndef RWLock_h
#define RWLock_h

#include <condition_variable>
#include <mutex>
#include <iostream>

struct RWLock
  {
  size_t getReaders() const
    {
    auto && lock = std::unique_lock<std::mutex>{m_mutex};
    (void)lock;
    return m_readLocked;
    }

  void lockR()
    {
    auto && lock = std::unique_lock<std::mutex>{m_mutex};
    m_readingAllowed.wait(lock, [&]{ return !m_writeLocked; });
    ++m_readLocked;
    m_readingAllowed.notify_all();
    }

  void unlockR()
    {
    auto && lock = std::unique_lock<std::mutex>{m_mutex};
    --m_readLocked;
    m_writingAllowed.notify_all();
    m_readingAllowed.notify_all();
    (void)lock;
    }

  void lockW()
    {
    auto && lock = std::unique_lock<std::mutex>{m_mutex};
    m_writingAllowed.wait(lock, [&]{ return !(m_readLocked || m_writeLocked); });
    m_writeLocked = true;
    }

  void unlockW()
    {
    auto && lock = std::unique_lock<std::mutex>{m_mutex};
    m_writeLocked = false;
    m_writingAllowed.notify_all();
    m_readingAllowed.notify_all();
    (void)lock;
    }

  private:
    std::mutex mutable m_mutex{};
    std::condition_variable m_readingAllowed{};
    std::condition_variable m_writingAllowed{};
    size_t m_readLocked{};
    bool m_writeLocked{};
  };

#endif
