#include <cstdint>
#include <chrono>
#include <random>
#include <iostream>

int main() {
	const int rows = 4000;
	const int cols = 4000;
	const int size = rows * cols;
	uint64_t *arrs = new uint64_t[2*size];
	uint64_t *rowM = arrs;
	uint64_t *colM = &arrs[size];

	std::mt19937_64 rng(0);

	rng.seed(0);
	for(int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			rowM[r*cols + c] = rng();
		}
	}
	
	rng.seed(0);
	for(int c = 0; c < cols; c++) {
		for (int r = 0; r < rows; r++) {
			colM[r*cols + c] = rng();
		}
	}

	auto t = std::chrono::steady_clock::now();
	int rMSum = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			rMSum += rowM[r*cols + c];
		}
	}
	auto newt = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(newt-t).count() << " " << rMSum << "\n";

	t = std::chrono::steady_clock::now();
	int cMSum = 0;
	for (int c = 0; c < cols; c++) {
		for (int r = 0; r < rows; r++) {
			cMSum += colM[r*cols + c];
		}
	}
	newt = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(newt-t).count() << " " << cMSum << "\n";

	delete[] arrs;
	return 0;
}

