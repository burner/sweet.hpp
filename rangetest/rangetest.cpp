#include <vector>
#include <stdint.h>

#include <logger.hpp>

typedef std::vector<int> VectorInt;

struct VectorFloat {
	inline VectorFloat(VectorInt::const_iterator b,
		VectorInt::const_iterator e, int32_t l) : beginIt(b), endIt(e),
		len(l)
	{}

	inline double& operator*() {
		this->cur = 0.0;

		VectorInt::const_iterator it{this->beginIt};
		int32_t cnt{0};

		for(; cnt <= this->len && it != this->endIt; ++cnt, ++it) {
			this->cur += *it;
		}

		this->cur /= cnt;

		return this->cur;
	}

	inline VectorFloat& operator++() {
		++this->beginIt;
		return *this;
	}

	inline bool operator!=(const VectorFloat& that) const {
		return this->beginIt != that.beginIt;
	}

private:
	VectorInt::const_iterator beginIt;
	VectorInt::const_iterator endIt;
	int32_t len;
	double cur;
};

struct VectorRange {
	inline VectorRange(const VectorInt& v) : vec(v) {}

	inline VectorFloat begin() const {
		return VectorFloat(vec.cbegin(), vec.cend(), 3);
	}

	inline VectorFloat end() const {
		return VectorFloat(vec.cend(), vec.cend(), 3);
	}

	const VectorInt& vec;
};

int main() {
	VectorInt vi {1,2,3,4,5,6,7,8};

	for(auto& v : VectorRange(vi)) {
		LOG("%f", v);
	}
}
