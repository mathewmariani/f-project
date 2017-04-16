template <typename T, const std::size_t length>
inline matn<T, length>::matn() {
	memset(&data, 0, sizeof(data));
}

template <typename T, const std::size_t length>
inline matn<T, length>::matn(const matn<T, length>& m) {
	memcpy(&data, m.data, sizeof(data));
}

template <typename T, const std::size_t length>
inline matn<T, length>::operator T* () {
	return &data[0][0];
}

template <typename T, const std::size_t length>
inline matn<T, length>::operator const T* () const {
	return &data[0][0];
}

template <typename T, const std::size_t length>
inline T* matn<T, length>::operator [] (int i) {
	return data[i];
}

template <typename T, const std::size_t length>
inline const T* matn<T, length>::operator [] (int i) const {
	return data[i];
}

template <typename T, const std::size_t length>
inline matn<T, length>& matn<T, length>::operator = (const matn<T, length>& m) {
	memcpy(&data, m.data, sizeof(data));
	return *this;
}

template <typename T, const std::size_t length>
matn<T, length> matn<T, length>::operator * (const  matn<T, length>& m) const {
	matn<T, length> ret;
	for (auto i = 0; i < length; ++i) {
		for (auto j = 0; j < length; ++j) {
			T sum{ 0 };
			for (auto k = 0; k < length; ++k) {
				sum += data[k][j] * m.data[i][k];
			}
			ret[i][j] = sum;
		}
	}
	return ret;
}

template <typename T, const std::size_t length>
void matn<T, length>::operator *= (const matn<T, length>& m) {
	*this = (*this * m);
}

template <typename T, const std::size_t length>
matn<T, length> matn<T, length>::identity() {
	matn<T, length> ret;

	for (auto i = 0; i < length; ++i) {
		ret.data[i][i] = 1;
	}

	return ret;
}
