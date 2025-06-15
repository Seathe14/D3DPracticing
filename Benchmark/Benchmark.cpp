#include "benchmark/benchmark.h"
//#include "BenchmarkApp.h"
//#include "Graphics.h"

void BM_DoFrame(benchmark::State& state) {
	std::vector<char> vec(10000000, 'a');
	for (auto _ : state) {
		for(const auto&i : vec)
		{
			char c = i;
		}
	}
}
BENCHMARK(BM_DoFrame)->Iterations(100000);


void BM_DoFrame2(benchmark::State& state) {
	std::vector<char> vec(10000000, 'a');
	for (auto _ : state) {
		for (const auto i : vec)
		{
			char c = i;
		}
	}
}
BENCHMARK(BM_DoFrame2)->Iterations(100000);


BENCHMARK_MAIN();