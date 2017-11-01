#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <set>
#include "matrix.hpp"
#include "utility.hpp"
#include <benchmark/benchmark.h>

using namespace std;

void BM_vector(benchmark::State& state)
{
    while(state.KeepRunning()){
        vector<int> v;
        v.push_back(0);
    }
}

void BM_nothing(benchmark::State& state){
    while(state.KeepRunning()){
    }
}

BENCHMARK(BM_vector);
BENCHMARK(BM_nothing);

BENCHMARK_MAIN();
