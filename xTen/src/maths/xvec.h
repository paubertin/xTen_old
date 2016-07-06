#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <cassert>

namespace xten {
	namespace xmaths {

#define BASIC_VEC(n)															\
	public:																		\
		/*              - Array accessors -                      */				\
		inline T& operator [](int i) { assert (i>=0 && i<n); return data[i]; }	\
		inline const T& operator [](int i) const								\
		{																		\
			assert (i>=0 && i<n); return data[i];								\
		}																		\
																				\
		/*              - Constructors -                         */				\
		/*default (0 initialized)*/												\
		explicit xVec() : data{ T(0) } {}										\
		/*from list*/															\
		explicit xVec(std::initializer_list<T> list)							\
		{																		\
			int m = std::min(n, int(list.size()));								\
			auto iter = list.begin();											\
			for (int i = 0; i < m; ++i , ++iter)								\
				data[i] = T(*iter);												\
			for (int i = m; i < n; ++i)											\
				data[i] = T(0);													\
		}																		\
		/*from value*/															\
		explicit xVec(const T a)												\
		{																		\
			for (int i = 0; i < n; ++i)											\
				data[i] = a;													\
		}																		\
		/*copy constructor*/                                                    \
		template <typename U, int n2>											\
		explicit xVec(const xVec<U, n2> &v)										\
		{																		\
			int m = std::min(n, int(n2));										\
			for (int i = 0; i < m; ++i)											\
				data[i] = static_cast<T>(v.data[i]);							\
			for (int i = m; i < n; ++i)											\
				data[i] = T(0);													\
		}																		\
																				\
		xVec& operator=(const xVec &v)											\
		{																		\
			for (int i = 0; i < n; ++i)											\
				data[i] = v.data[i];											\
			return *this;														\
		}																		\
																				\
		/*destructor*/															\
		~xVec() {};																\
																				\
		/*              - Unary operations -                         */			\
		inline xVec operator +() { return *this; }								\
																				\
		inline xVec operator -() const											\
		{																		\
			xVec res;															\
			for (int i = 0; i < n; ++i)											\
				res.data[i] = -data[i];											\
			return res;															\
		}																		\
																				\
		/*              - Scalar operations -                         */		\
		/*multiplication*/														\
		xVec &operator *=(const T s)											\
		{																		\
			for (int i = 0; i < n; ++i)											\
				data[i] *= s;													\
			return *this;														\
		}																		\
																				\
		/*division*/															\
		xVec &operator /=(const T s)											\
		{																		\
			auto den = T(1)/s;												\
			for (int i = 0; i < n; ++i)											\
				data[i] *= den;													\
			return *this;														\
		}																		\
																				\
		/*              - Vector operations -                         */		\
		/*addition*/															\
		friend xVec operator+(xVec v1, const xVec &v2)							\
		{																		\
			return v1+=v2;														\
		}																		\
																				\
		xVec& operator +=(const xVec &v)										\
		{																		\
			for (int i = 0; i < n; ++i)											\
				data[i] += v.data[i];											\
			return *this;														\
		}																		\
																				\
		/*subtraction*/															\
		friend xVec operator-(xVec v1, const xVec &v2)							\
		{																		\
			return v1-=v2;														\
		}																		\
																				\
		xVec& operator -=(const xVec &v)										\
		{																		\
			for (int i = 0; i < n; ++i)											\
				data[i] -= v.data[i];											\
			return *this;														\
		}																		\
																				\
		/*dot product*/															\
		friend T dot(const xVec &v1, const xVec &v2)							\
		{																		\
			T res(0);															\
			for (int i = 0; i < n; ++i)											\
				res += v1.data[i] * v2.data[i];									\
			return res;															\
		}																		\
																				\
		/*              - Checks -                         */					\
		bool operator ==(const xVec& v2) const									\
		{																		\
			for (int i = 0; i < n; ++i)											\
			{																	\
				if (data[i] != v2.data[i])										\
				return false;													\
			}																	\
			return true;														\
		}																		\
																				\
		bool operator !=(const xVec& v2) const									\
		{																		\
			return !(*this == v2);												\
		}																		\
																				\
		xVec& clamp(const xVec &v1, const xVec &v2)								\
		{																		\
			for (int i = 0; i < n; ++i)											\
			{																	\
				data[i] = std::max(std::min(data[i], v2.data[i]), v1.data[i]);	\
			}																	\
			return *this;														\
		}																		




		//Generic class
		template <typename T, int n>
		class xVec
		{
		public:
			T data[n];
			BASIC_VEC(n);
		};



#pragma warning(push)
#pragma warning(disable: 4201)

		//Specialization n=2
		template <typename T>
		class xVec<T, 2>
		{
		public:
			union
			{
				T data[2];
				struct { T x, y; };
			};
			BASIC_VEC(2)
				xVec(T xx, T yy) : x(xx), y(yy) {};
			friend T det(const xVec & v1, const xVec & v2)
			{
				return (v1.x * v2.y - v1.y * v2.x);
			}
			xVec orthoVec() const
			{
				return xVec(-y, x);
			}
			xVec<T, 2>& operator=(const xVec<T, 3> &v) { data[0] = v.data[0]; data[1] = v.data[1]; return *this; }
		};

		typedef xVec<float, 2> xVec2f;
		typedef xVec<float, 2> vec2;
		typedef xVec<double, 2> xVec2d;
		typedef xVec<int, 2> xVec2i;
		typedef xVec<int, 2> vec2i;

		//Specialization n=3
		template <typename T>
		class xVec<T, 3>
		{
		public:
			union
			{
				T data[3];
				struct { T x, y, z; };
				struct { T r, g, b; };
			};
			BASIC_VEC(3)
				xVec(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {};
			friend xVec cross(const xVec & v1, const xVec & v2)
			{
				return xVec(v1.y * v2.z - v1.z * v2.y,
					v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x);
			}
			xVec orthoVec() const
			{
				return (abs(x) > abs(z) ? xVec(-y, x, T(0))
					: xVec(T(0), -z, y));
			}
		};

		typedef xVec<float, 3> xVec3f;
		typedef xVec<float, 3> vec3;
		typedef xVec<double, 3> xVec3d;
		typedef xVec<int, 3> xVec3i;
		typedef xVec<int, 3> vec3i;

		//Specialization n=4
		template <typename T>
		class xVec<T, 4>
		{
		public:
			union
			{
				T data[4];
				struct { T x, y, z, w; };
				struct { T r, g, b, a; };
			};
			BASIC_VEC(4)
				xVec(T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) {};
		};

		typedef xVec<float, 4> xVec4f;
		typedef xVec<float, 4> vec4;
		typedef xVec<double, 4> xVec4d;
		typedef xVec<int, 4> xVec4i;
		typedef xVec<int, 4> vec4i;

#pragma warning(pop)
#undef BASIC_VEC

		template <typename T, int n>
		inline xVec<T, n> operator *(const T s, xVec<T, n> v)
		{
			return v *= s;
		}

		template <typename T, int n>
		inline xVec<T, n> operator *(xVec<T, n> v, const T s)
		{
			return v *= s;
		}

		template <typename T, int n>
		inline xVec<T, n> operator /(xVec<T, n> v, const T s)
		{
			return v /= s;
		}

		/*              - Norm... -                         */
		template <typename T, int n>
		inline T sq_norm(const xVec<T, n> &v)
		{
			return dot(v, v);
		}

		template <typename T, int n>
		inline T norm(const xVec<T, n> &v)
		{
			return sqrt(sq_norm(v));
		}

		template <typename T, int n>
		xVec<T, n> normalize(xVec<T, n> v)
		{
			auto norm_ = norm(v);
			assert(norm_ > 0);
			auto inv_norm = T(1) / norm_;
			return v *= inv_norm;
		}

		template <typename T, int n>
		std::ostream& operator << (std::ostream &os, const xVec<T, n> &v)
		{
			//os << "[";
			for (int i = 0; i < n; ++i)
				//os << v.data[i] << ((i != (n - 1)) ? " " : "]'");
				os << v.data[i] << std::endl;
			return os;
		}

	}
}