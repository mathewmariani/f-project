#pragma once

// C/C++
#include <math.h> 

#include "matn.h"
#include "vecn.h"

template <typename T>
class mat4 : public matn<T, 4> {
	// constructors
public:
	inline mat4() = default;
	inline mat4(const matn<T, 4>& m) : matn<T, 4>(m) {}

	// static functions
public:
	static inline mat4 transform(T x, T y, T z, T angle, T rx, T ry, T rz, T sx, T sy, T sz) {
		mat4 ret = mat4::identity();

		const float rad = (angle * 0.0174533f);
		const float c = cosf(rad);
		const float s = sinf(rad);
		const float ux = (rx * rx);
		const float uy = (ry * ry);
		const float uz = (rz * rz);

		ret[0][0] = sx * (c + ux * (1 - c));
		ret[0][1] = sy * ((y * x) * (1 - c) + z * s);
		ret[0][2] = sz * ((z * x) * (1 - c) - y * s);

		ret[1][0] = sx * ((x * y) * (1 - c) - z * s);
		ret[1][1] = sy * (c + uy * (1 - c));
		ret[1][2] = sz * ((z * y) * (1 - c) + x * s);

		ret[2][0] = sx * ((x * z) * (1 - c) + y * s);
		ret[2][1] = sy * ((y * z) * (1 - c) - x * s);;
		ret[2][2] = sz * (c + uz * (1 - c));

		ret[3][0] = x * ret[0][0] + y * ret[1][0] + z * ret[2][0];
		ret[3][1] = x * ret[0][1] + y * ret[1][1] + z * ret[2][1];
		ret[3][2] = x * ret[0][2] + y * ret[1][2] + z * ret[2][2];

		return ret;
	}

	static inline mat4 translate(T x, T y, T z) {
		mat4 ret = mat4::identity();
		ret[3][0] = x;
		ret[3][1] = y;
		ret[3][2] = z;
		return ret;
	}

	static inline mat4 scale(T sx, T sy, T sz) {
		mat4 ret = mat4::identity();
		ret[0][0] = sx;
		ret[1][1] = sy;
		ret[2][2] = sz;
		return ret;
	}

	static inline mat4 scale(T s) {
		mat4 ret = mat4::identity();
		ret[0][0] = s;
		ret[1][1] = s;
		ret[2][2] = s;
		return ret;
	}

	static inline mat4 rotate(T angle, T x, T y, T z) {
		const float rad = (angle * 0.0174533f);
		const float c = cosf(rad);
		const float s = sinf(rad);
		const float ux = (x * x);
		const float uy = (y * y);
		const float uz = (z * z);

		mat4 ret = mat4::identity();
		ret[0][0] = (c + ux * (1 - c));
		ret[0][1] = ((y * x) * (1 - c) + z * s);
		ret[0][2] = ((z * x) * (1 - c) - y * s);
		ret[1][0] = ((x * y) * (1 - c) - z * s);
		ret[1][1] = (c + uy * (1 - c));
		ret[1][2] = ((z * y) * (1 - c) + x * s);
		ret[2][0] = ((x * z) * (1 - c) + y * s);
		ret[2][1] = ((y * z) * (1 - c) - x * s);
		ret[2][2] = (c + uz * (1 - c));
		return ret;
	}

	static inline mat4 rotateX(T angle) {
		const float rad = angle * 0.0174533f;
		const float c = cosf(rad);
		const float s = sinf(rad);

		mat4 ret = mat4::identity();
		ret[1][1] = (c);
		ret[1][2] = (s);
		ret[2][1] = (-s);
		ret[2][2] = (c);
		return ret;
	}

	static inline mat4 rotateY(T angle) {
		const float rad = angle * 0.0174533f;
		const float c = cosf(rad);
		const float s = sinf(rad);

		mat4 ret = mat4::identity();
		ret[0][0] = (c);
		ret[0][2] = (-s);
		ret[2][0] = (s);
		ret[2][2] = (c);
		return ret;
	}

	static inline mat4 rotateZ(T angle) {
		const float rad = angle * 0.0174533f;
		const float c = cosf(rad);
		const float s = sinf(rad);

		mat4 ret = mat4::identity();
		ret[0][0] = (c);
		ret[0][1] = (s);
		ret[1][0] = (-s);
		ret[1][1] = (c);
		return ret;
	}

	static inline mat4 lookAt(const vecn<T, 3>& eye, const vecn<T, 3>& center, const vecn<T, 3>& up) {
		const vecn<T, 3> f = vecn<T, 3>::normalize(center - eye);
		const vecn<T, 3> s = vecn<T, 3>::cross(f, vecn<T, 3>::normalize(up));
		const vecn<T, 3> u = vecn<T, 3>::cross(s, f);

		mat4 ret = mat4::identity();
		ret[0][0] = s[0];
		ret[1][0] = s[1];
		ret[2][0] = s[2];
		ret[3][0] = T(0);

		ret[0][1] = u[0];
		ret[1][1] = u[1];
		ret[2][1] = u[2];
		ret[3][1] = T(0);

		ret[0][2] = -f[0];
		ret[1][2] = -f[1];
		ret[2][2] = -f[2];
		ret[3][2] = T(0);

		ret[0][3] = T(0);
		ret[1][3] = T(0);
		ret[2][3] = T(0);
		ret[3][3] = T(1);

		return (ret * translate(-eye[0], -eye[1], -eye[2]));
	}

	static inline mat4 perspective(T fovy, T aspect, T n, T f) {
		mat4 ret = mat4::identity();
		float scale = (1.0f / tanf(fovy / 2.0f));
		float ratio = (scale / aspect);
		ret[0][0] = (ratio);
		ret[1][1] = (scale);
		ret[2][2] = (-(f) / (f - n));
		ret[2][3] = (-1.0f);
		ret[3][2] = (-(f * n) / (f - n));
		ret[3][3] = (0.0f);
		return ret;
	};

	static inline mat4 ortho(T left, T right, T bottom, T top, T n, T f) {
		mat4 ret = mat4::identity();
		ret[0][0] = (2.0f / (right - left));
		ret[1][1] = (2.0f / (top - bottom));
		ret[2][2] = (-2.0f / (f - n));
		ret[3][0] = (-(right + left) / (right - left));
		ret[3][1] = (-(top + bottom) / (top - bottom));
		ret[3][2] = (-(f + n) / (f - n));
		return ret;
	}
};

template <typename T, const int N, const int M>
static inline vecn<T, N> operator * (const vecn<T, N>& v, const matn<T, M>& m) {
	vecn<T, 3> ret;
	for (auto i = 0; i < N; ++i) {
		for (auto j = 0; j < M; ++j) {
			ret[i] += v[j] * m[j][i];
		}
	}
	return ret;
}

// compile template functions
typedef mat4<float> mat4f;
