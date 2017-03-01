#ifndef BankAccount_h
#define BankAccount_h

#include "RWLock.h"
#include <chrono>

struct BankAccount
  {
  void deposit(double amount)
    {
    m_lock.lockW();
    m_balance += amount;
    m_lock.unlockW();
    }

  double getBalance() const
    {
    m_lock.lockR();
    auto current = m_balance;
    m_lock.unlockR();
    return current;
    }

  size_t getReaders() const
    {
    return m_lock.getReaders();
    }

  private:
    mutable RWLock m_lock;
    double m_balance{};
  };

#endif
