#include "mevenshtein.h"
#include <vector>

uint32_t mevenshtein::run(const std::string& a, const std::string& b)
{
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<uint32_t>> d(len_a + 1, std::vector<uint32_t>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}
