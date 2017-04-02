#pragma once

template <typename T, const std::size_t length>
class vecn {
// type definitions
protected:

// constructors
public:
	vecn();
	vecn(const vecn& v);

// operators
public:
	operator T* ();
	operator const T* () const;

	T& operator [] (int i);
	const T& operator [] (int i) const;

	vecn& operator = (const vecn& v);

	vecn operator + (const vecn& v) const;
	void operator += (const vecn& v);

	vecn operator - (const vecn& v) const;
	void operator -= (const vecn& v);

	vecn operator * (const vecn& v) const;
	void operator *= (const vecn& v);

	vecn operator / (const vecn& v) const;
	void operator /= (const vecn& v);

	vecn operator * (const T& s) const;
	void operator *= (const T& s);

	vecn operator / (const T& s) const;
	void operator /= (const T& s);

	// static functions
	public:
		static T angle(const vecn& a, const vecn& b);
		static T distance(const vecn& a, const vecn& b);
		static T dot(const vecn& a, const vecn& b);
		static T magnitude(const vecn& v);
		static vecn max(const vecn& a, const vecn& b);
		static vecn min(const vecn& a, const vecn& b);
		static vecn normalize(const vecn& v);

// protected variables
protected:
	T data[length];
};	// vecn

#include "vecn.inl"
