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
        cerr << "Usage: ./prime_numbers <limit> was incorrect. Please give the proper formatting\n";
        return 1;
    }
 
    int limit = stoi(argv[1]);  // convert command-line argument to integer
 
    //determine the number of threads to use
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 2;  // Default to 2 threads if hardware_concurrency() can't determine the value
    }
 
    cout << "Calculating prime numbers up to " << limit << " using " << numThreads << " threads.\n";
 
    //divide the work into ranges for each thread
    int rangeSize = limit / numThreads; //calculate size of range
    vector<thread> threads; //vector stores threads
    vector<vector<int>> results(numThreads); //vector of vectors to store prime numbers
 
    //create threads and assign each one a range of numbers
    for (unsigned int i = 0; i < numThreads; ++i) {
        int start = i * rangeSize + 1;
        int end = start + rangeSize - 1;

        //if its the last thread, handl the remainer of the range
        if (i == numThreads - 1)
        {
              end = limit;
        }
    }
 
    //wait for all the threads to complete
    for (auto& t : threads) {
        t.join();
    }
 
    //combine all results from the threads
    vector<int> allPrimes;
    for (const auto& localPrimes : results) {
        allPrimes.insert(allPrimes.end(), localPrimes.begin(), localPrimes.end());
    }
 
    // sort the prime numbers
    sort(allPrimes.begin(), allPrimes.end());
 
    // output the prime numbers
    cout << "Prime numbers less than or equal to " << limit << ":\n";
    for (const int prime : allPrimes) {
        cout << prime << " ";
    }
    cout << endl;
 
    return 0;
}
