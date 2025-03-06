#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <algorithm>
 
using namespace std;
 
mutex mtx;
 
bool checkPrime(int num)
{
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0) return false;
    }
 
    return true;
}
 
 
 
void workerFunction(int start, int end, vector<int>& result) {
 
   for (int num = start; num <= end; num++)
   {
       if (checkPrime(num))
       {
           result.push_back(num);
       }
   }
}
 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./prime_numbers <limit>\n";
        return 1;
    }
 
    int limit = stoi(argv[1]);  // Convert command-line argument to integer
 
    // Determine the number of threads to use
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 2;  // Default to 2 threads if hardware_concurrency() can't determine the value
    }
 
    cout << "Calculating prime numbers up to " << limit << " using " << numThreads << " threads.\n";
 
    // Divide the work into ranges for each thread
    int rangeSize = limit / numThreads;
    vector<thread> threads;
    vector<vector<int>> results(numThreads);
 
    // Create threads and assign each one a range of numbers
    for (unsigned int i = 0; i < numThreads; ++i) {
        int start = i * rangeSize + 1;
        int end = (i == numThreads - 1) ? limit : (start + rangeSize - 1); // Last thread handles the remainder
        threads.emplace_back(workerFunction, start, end, ref(results[i]));
    }
 
    // Join all threads
    for (auto& t : threads) {
        t.join();
    }
 
    // Combine all results from the threads
    vector<int> allPrimes;
    for (const auto& localPrimes : results) {
        allPrimes.insert(allPrimes.end(), localPrimes.begin(), localPrimes.end());
    }
 
    // Sort the prime numbers
    sort(allPrimes.begin(), allPrimes.end());
 
    // Output the prime numbers
    cout << "Prime numbers less than or equal to " << limit << ":\n";
    for (const int prime : allPrimes) {
        cout << prime << " ";
    }
    cout << endl;
 
    return 0;
}
