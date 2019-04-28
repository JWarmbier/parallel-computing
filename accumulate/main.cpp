#include <iostream>
#include <thread>
#include <numeric>
#include <bits/stl_vector.h>
#include <algorithm>
#include <functional>

template <typename Iterator, typename T>
struct accumulateBlock{
    void operator()(Iterator first, Iterator last, T& result){
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init){
    unsigned long const length = std::distance (first, last);

    if(!length)
            return init;

    unsigned long const minPerThread = 25;
    unsigned long const maxThreads = (length+minPerThread -1 )/minPerThread;
    unsigned long const hardwareThreads = std::thread::hardware_concurrency(); // return number of processor's threads

    unsigned long const numThreads = std::min(hardwareThreads != 0? hardwareThreads:2,maxThreads);

    unsigned long const blockSize = length/numThreads;

    std::vector<T> results(numThreads); // container with results from threads

    std::vector<std::thread> threads(numThreads-1); // container with threads

    Iterator blockStart = first;

    for(unsigned long i = 0; i < (numThreads -1); ++i){
        Iterator blockEnd = blockStart;
        std::advance(blockEnd, blockSize); // increments blockEnd iterator by blockSize
        threads[i] = std::thread (accumulateBlock<Iterator, T>(), blockStart, blockEnd, std::ref(results[i]));
        blockStart = blockEnd;
    }

    accumulateBlock<Iterator, T>()(blockStart, last, results[numThreads-1]); //main thread do the last part of array
    std::for_each(threads.begin(),threads.end(), std::mem_fn(&std::thread::join)); //wait for threads to finish

    return std::accumulate(results.begin(), results.end(), init);

}

int main() {
    std::cout << "Number of processor's threads: " << std::thread::hardware_concurrency() << std::endl;

    // exmaple
    std::vector<long> numbers;

    for(int i = 0 ; i < 1000000; ++i){
        numbers.push_back(i);
    }

    std::cout << "Result: " << parallelAccumulate<std::vector<long>::const_iterator, long>(numbers.begin(),numbers.end(), numbers.front()) << std::endl;

    return 0;
}