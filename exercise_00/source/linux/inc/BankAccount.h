#ifndef BankAccount_h
#define BankAccount_h

#include "RWLock.h"
#include <chrono>

struct BankAccount
  {
  void deposit(double amount)
    {
    (void)amount; // TODO
    }

  double getBalance() const
    {
    // TODO
    return 0;
    }

  size_t getReaders() const
    {
    // TODO
    return 0;
    }

  private:
    mutable RWLock m_lock;
    double m_balance{};
  };

#endif
