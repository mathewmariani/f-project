#pragma once

// C/C++
#include <math.h>
#include "matn.h"

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
		const float c = std::cosf(rad);
		const float s = std::sinf(rad);
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
		const float c = std::cosf(rad);
		const float s = std::sinf(rad);
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

	static inline mat4 perspective(T fovy, T aspect, T near, T far) {
		mat4 ret = mat4::identity();
		float scale = (1.0f / std::tanf(fovy / 2.0f);
		float ratio = (scale / aspect);
		ret[0][0] = (ratio);
		ret[1][1] = (scale);
		ret[2][2] = (-(far) / (far - near));
		ret[2][3] = (-1.0f));
		ret[3][2] = (-(far * near) / (far - near));
		ret[3][3] = (0.0f);
		return ret;
	};

	static inline mat4 ortho(T left, T right, T bottom, T top, T near, T far) {
		mat4 ret = mat4::identity();
		ret[0][0] = (2.0f / (right - left));
		ret[1][1] = (2.0f / (top - bottom));
		ret[2][2] = (-2.0f / (far - near));
		ret[3][0] = (-(right + left) / (right - left));
		ret[3][1] = (-(top + bottom) / (top - bottom));
		ret[3][2] = (-(far + near) / (far - near));
		return ret;
	}
};

// compile template functions
typedef mat4<float> mat4f;
