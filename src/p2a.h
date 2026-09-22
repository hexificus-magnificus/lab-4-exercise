#ifndef MORTON_H
#define MORTON_H
#include <cstdint>
inline uint64_t expand(uint64_t input, uint32_t scale) {
	if (input == 0)
		return 0;
	uint64_t out = 0;
	int size = 0;

	while ((input >> ++size) >= 1) {}

	for (int i = 0; i < size; i++) {
		if (i*scale >= 64)
			break;
		uint64_t current = (input >> i) & 0b1;
		out |= current << (i * scale);
	}

	return out;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
	return expand(x, 3) | (expand(y, 3) << 1) | (expand(z, 3) << 2);
}

#endif
