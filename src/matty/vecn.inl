#include <algorithm>

template <typename T, const std::size_t length>
inline vecn<T, length>::vecn() {
	memset(&data, 0, sizeof(data));
}

template <typename T, const std::size_t length>
inline vecn<T, length>::vecn(const vecn<T, length>& v) {
	for (int i = 0; i < length; ++i) {
		data[i] = v.data[i];
	}
}

template <typename T, const std::size_t length>
inline vecn<T, length>::operator T* () {
	return &data[0];
}

template <typename T, const std::size_t length>
inline vecn<T, length>::operator const T* () const {
	return &data[0];
}

template <typename T, const std::size_t length>
inline T& vecn<T, length>::operator [] (int i) {
	return data[i];
}

template <typename T, const std::size_t length>
inline const T& vecn<T, length>::operator [] (int i) const {
	return data[i];
}

template <typename T, const std::size_t length>
inline vecn<T, length>& vecn<T, length>::operator = (const vecn<T, length>& v) {
	memcpy(&data, v.data, sizeof(data));
	return (*this);
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator + (const vecn& v) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] + v.data[i];
	}
	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator += (const vecn& v) {
	(*this) = (*this) + v;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator - (const vecn& v) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] - v.data[i];
	}
	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator -= (const vecn& v) {
	(*this) = (*this) - v;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator * (const vecn& v) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] * v.data[i];
	}
	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator *= (const vecn& v) {
	(*this) = (*this) * v;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator / (const vecn& v) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] / v.data[i];
	}

	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator /= (const vecn& v) {
	(*this) = (*this) / v;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator * (const T& s) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] * s;
	}
	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator *= (const T& s) {
	(*this) = (*this) * s;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::operator / (const T& s) const {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret.data[i] = data[i] / s;
	}
	return ret;
}

template <typename T, const std::size_t length>
inline void vecn<T, length>::operator /= (const T& s) {
	(*this) = (*this) / s;
}

template <typename T, const std::size_t length>
inline T vecn<T, length>::angle(const vecn<T, length>& a, const vecn<T, length>& b) {
	return (std::acos(dot(a, b) / (magnitude(a) * magnitude(b))) / 0.0174533f);
}

template <typename T, const std::size_t length>
inline T vecn<T, length>::distance(const vecn<T, length>& a, const vecn<T, length>& b) {
	T sum{ 0 };
	for (auto i = 0; i < length; ++i) {
		sum += ((a[i] - b[i])*(a[i] - b[i]));
	}
	return (T)std::sqrt(sum);
}

template <typename T, const std::size_t length>
inline T vecn<T, length>::dot(const vecn<T, length>& a, const vecn<T, length>& b) {
	T sum{ 0 };
	for (auto i = 0; i < length; ++i) {
		sum += (a[i] * b[i]);
	}
	return sum;
}

template <typename T, const std::size_t length>
inline T vecn<T, length>::magnitude(const vecn<T, length>& v) {
	T sum{ 0 };
	for (auto i = 0; i < length; ++i) {
		sum += (v[i] * v[i]);
	}
	return (T)std::sqrt(sum);
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::max(const vecn<T, length>& a, const vecn<T, length>& b) {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret[i] = (std::max)(a[i], b[i]);
	}
	return ret;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::min(const vecn<T, length>& a, const vecn<T, length>& b) {
	vecn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		ret[i] = (std::min)(a[i], b[i]);
	}
	return ret;
}

template <typename T, const std::size_t length>
inline vecn<T, length> vecn<T, length>::normalize(const vecn<T, length>& v) {
	return (v / vecn<T, length>::magnitude(v));
}
