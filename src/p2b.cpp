#include <random>
#include <cmath>
#include <iostream>
#include <cassert>
#include "p2a.h"
#include "timer.h"

#define DIMSIZE 256
#define KSIZE 4
#define KSTRIDE 4

size_t rMIdx(size_t x, size_t y, size_t z) {
	return z*std::pow(DIMSIZE, 2) + y*DIMSIZE + x;
}

size_t rMIdxK(size_t x, size_t y, size_t z) {
	return z*std::pow(KSIZE, 2) + y*KSIZE + x;
}

size_t rMIdxConv(size_t x, size_t y, size_t z) {
	size_t side = 1 + (DIMSIZE - KSIZE)/KSTRIDE;
	return z*std::pow(side, 2) + y*side + x;
}

uint64_t *rMConvolve(uint64_t *a) {
	const size_t kLen = std::pow(KSIZE, 3);
	uint64_t kernel[kLen];
	for (int x = 0; x < KSIZE; x++)
		for (int y = 0; y < KSIZE; y++)
			for (int z = 0; z < KSIZE; z++)
				kernel[rMIdxK(x, y, z)] = x + y + z;

	const size_t steps = 1 + (DIMSIZE - KSIZE)/KSTRIDE;	// integer division will round down
	const size_t outLen = std::pow(steps, 3);
	uint64_t *out = new uint64_t[outLen];
	
	for (int xs = 0; xs + KSIZE - 1 < DIMSIZE; xs += KSTRIDE) {
		for (int ys = 0; ys + KSIZE - 1 < DIMSIZE; ys += KSTRIDE) {
			for (int zs = 0; zs + KSIZE - 1 < DIMSIZE; zs += KSTRIDE) {
				uint64_t sum = 0;

				for (int x = 0; x < KSIZE; x++)
					for (int y = 0; y < KSIZE; y++)
						for (int z = 0; z < KSIZE; z++)
							sum += kernel[rMIdxK(x, y, z)] * a[rMIdx(x+xs, y+ys, z+zs)];
				out[rMIdxConv(xs/KSTRIDE, ys/KSTRIDE, zs/KSTRIDE)] = sum;
			}
		}
	}
	return out;
}

uint64_t *mConvolve(uint64_t *a) {
	const size_t kLen = std::pow(KSIZE, 3);
	uint64_t kernel[kLen];
	for (int x = 0; x < KSIZE; x++)
		for (int y = 0; y < KSIZE; y++)
			for (int z = 0; z < KSIZE; z++)
				kernel[morton3d(x, y, z)] = x + y + z;

	const size_t steps = 1 + (DIMSIZE - KSIZE)/KSTRIDE;	// integer division will round down
	const size_t outLen = std::pow(steps, 3);
	uint64_t *out = new uint64_t[outLen];
	size_t outIdx = 0;
	for (size_t kstart = 0; kstart < std::pow(DIMSIZE, 3); kstart+=std::pow(KSTRIDE, 3)) {
		uint64_t sum = 0;
		for (size_t kidx = 0; kidx < std::pow(KSIZE, 3); kidx++) {
			sum += a[kstart + kidx] * kernel[kidx];
		}
		out[outIdx++] = sum;
	}
	return out;
}

int main() {
	const int size = std::pow(DIMSIZE, 3);

	std::mt19937_64 rng(0);

	uint64_t *rowM = new uint64_t[size];
	uint64_t *morton = new uint64_t[size];

	for (int i = 0; i < size; i++) {
		rowM[i] = rng();
	}
	for (int x = 0; x < DIMSIZE; x++) {
		for (int y = 0; y < DIMSIZE; y++) {
			for (int z = 0; z < DIMSIZE; z++) {
				morton[morton3d(x, y, z)] = rowM[rMIdx(x, y, z)];
			}
		}
	}

	Timer t;
	uint64_t *rMConv = rMConvolve(rowM);
	std::cout << t.click<Timer::Micros>() << "\n";
	uint64_t *mConv = mConvolve(morton);
	std::cout << t.click<Timer::Micros>() << "\n";

	const size_t steps = 1 + (DIMSIZE - KSIZE)/KSTRIDE;
	for (size_t x = 0; x < steps; x++) {
		for (size_t y = 0; y < steps; y++) {
			for (size_t z = 0; z < steps; z++) {
				assert(rMConv[rMIdxConv(x, y, z)] == mConv[morton3d(x, y, z)]);
			}
		}
	}

	delete[] rMConv;
	delete[] mConv;
	delete[] rowM;
	delete[] morton;
	return 0;
}
