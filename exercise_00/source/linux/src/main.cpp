#include "BankAccount.h"

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

auto constexpr kThreads{8};
auto constexpr kRuns{1000};

int main()
  {
  using namespace std::chrono_literals;
  auto && outputMutex = std::mutex{};
  auto && safeAccount = BankAccount{};
  auto && unsafeAccount = 0.0;

  std::array<std::thread, kThreads> threadPool{};

  std::cout << "main thread id is '" << std::this_thread::get_id()
            << "', hardware concurrency is '" << std::thread::hardware_concurrency()
            << "'\n" << std::flush;

  auto && randomDevice = std::random_device{"/dev/urandom"};

  for(auto && thread : threadPool)
    {
    thread = std::thread{
      [&, twister=std::mt19937{randomDevice()}, distribution=std::uniform_real_distribution<double>{0, 1000} ]() mutable {
      for (auto run{0}; run < kRuns; ++run)
        {
        if(run & 1)
          {
          auto amount = distribution(twister);

          auto && outputLock = std::unique_lock<std::mutex>{outputMutex};
          std::cout << "====\n"
                    << "thread '" << std::this_thread::get_id()
                    << "' deposits '" << amount
                    << "'\n====\n"
                    << std::flush;
          outputLock.unlock();

          safeAccount.deposit(amount);
          std::this_thread::sleep_for(1ms);
          unsafeAccount += amount;

          }

        auto balance = safeAccount.getBalance();
        auto readers = safeAccount.getReaders();

        auto && outputLock = std::unique_lock<std::mutex>{outputMutex};
        std::cout << "====\n"
                  << "thread '" << std::this_thread::get_id()
                  << "' safe balance is '" << balance
                  << "'\nunsafe balance is '" << unsafeAccount
                  << "'\nconcurrent readers '" << readers
                  << "'\n====\n"
                  << std::flush;
        outputLock.unlock();
        }
    }};
    }

  for(auto && thread : threadPool)
    {
    thread.join();
    }

  std::cerr << "safe balance is '" << safeAccount.getBalance()
            << "' unsafe balance is '" << unsafeAccount
            << "' difference is '" << unsafeAccount - safeAccount.getBalance()
            << "'\n";
  }
