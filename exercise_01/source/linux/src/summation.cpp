#include <iostream>
#include <omp.h>
#include "Stopwatch.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
// Explicit computation
constexpr long long sum(const long long n)
  {
  return n*(n + 1)/2;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Sequential summation
long long sumSerial(const int n)
  {
  long long sum = 0;
  for (int i=1; i <= n; i++)
    {
    sum += i;
    }
  return sum;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Parallel summation with critical section
long long sumPar1(const int n)
  {
  (void)n; // TODO
  return 0;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Parallel summation with atomic access
long long sumPar2(const int n)
  {
  (void)n; // TODO
  return 0;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Parallel summation with reduction
long long sumPar3(const int n)
  {
  (void)n; // TODO
  return 0;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Parallel summation with explicit locks
long long sumPar4(const int n)
  {
  (void)n; // TODO
  return 0;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
// Different summation tests
void summation()
  {
  cout << "\nSummation Tests" << endl;

  int64_t const N{10000000};
  Stopwatch sw{};

  sw.Start();
  int64_t sum0 = sum(N);
  sw.Stop();
  cout << "Explicit: " << sum0 << " in " << sw.GetElapsedTimeMilliseconds() << " ms\n\n" << flush;

  sw.Start();
  int64_t sumS = sumSerial(N);
  sw.Stop();
  cout << "Serial: " << sumS << " in " << sw.GetElapsedTimeMilliseconds() << " ms" << endl;
  cout << boolalpha << "The two operations produce the same results: " << (sumS == sum0) << endl << endl;

  sw.Start();
  int64_t sum1 = sumPar1(N);
  sw.Stop();
  cout << "Critical section: " << sum1 << " in " << sw.GetElapsedTimeMilliseconds() << " ms" << endl;
  cout << boolalpha << "The two operations produce the same results: " << (sum1 == sum0) << endl << endl;

  sw.Start();
  int64_t sum2 = sumPar2(N);
  sw.Stop();
  cout << "Atomic access: " << sum2 << " in " << sw.GetElapsedTimeMilliseconds() << " ms" << endl;
  cout << boolalpha << "The two operations produce the same results: " << (sum2 == sum0) << endl << endl;

  sw.Start();
  int64_t sum3 = sumPar3(N);
  sw.Stop();
  cout << "Reduction: " << sum3 << " in " << sw.GetElapsedTimeMilliseconds() << " ms" << endl;
  cout << boolalpha << "The two operations produce the same results: " << (sum3 == sum0) << endl << endl;

  sw.Start();
  int64_t sum4 = sumPar4(N);
  sw.Stop();
  cout << "Explicit locks: " << sum4 << " in " << sw.GetElapsedTimeMilliseconds() << " ms" << endl;
  cout << boolalpha << "The two operations produce the same results: " << (sum4 == sum0) << endl << endl;
  }