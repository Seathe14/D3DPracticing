#include "benchmark/benchmark.h"
#include "BenchmarkApp.h"
#include "Graphics.h"

BenchmarkApp app;
void BM_DoFrame(benchmark::State& state) {
	for (auto _ : state) {
		app.DrawTestTriangles();
	}
}

BENCHMARK(BM_DoFrame)->Iterations(1000);
BENCHMARK_MAIN();