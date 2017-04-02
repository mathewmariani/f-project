#pragma once

#include "vecn.h"

template <typename T>
class vec3 : public vecn<T, 3> {
public:
	inline vec3() = default;
	inline vec3(const vecn<T, 3>& v) : vecn<T, 3>(v) {}
	inline vec3(T x, T y, T z) {
		vecn<T, 3>::data[0] = x;
		vecn<T, 3>::data[1] = y;
		vecn<T, 3>::data[2] = z;
	}

	static inline vecn<T, 3> cross(const vecn<T, 3>& a, const vecn<T, 3>& b) {
		vecn<T, 3> ret;
		ret[0] = a[1] * b[2] - b[1] * a[2];
		ret[0] = a[2] * b[0] - b[2] * a[0];
		ret[0] = a[0] * b[1] - b[0] * a[1];
		return ret;
	}
};

// compile template functions
typedef vec3<float> vec3f;
