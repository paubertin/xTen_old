#pragma once

#include "xVec.h"
#include "xmaths.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <cassert>

namespace xten {
	namespace xmaths {

#define BASIC_MAT(n_row,n_col)															\
public:																					\
	/*              - Array accessors -                      */							\
	inline T& operator ()(int i, int j)													\
	{																					\
		assert(i >= 0 && i<n_row);														\
		assert(j >= 0 && j<n_col);														\
		return data[j*n_row + i];														\
	}																					\
																						\
	inline const T& operator()(int i, int j) const										\
	{																					\
		assert(i >= 0 && i<n_row);														\
		assert(j >= 0 && j<n_col);														\
		return data[j*n_row + i];														\
	}																					\
																						\
	/*              - Constructors -                         */							\
	/*default (0 initialized)*/															\
	explicit xMat() : data{ T(0) } {}													\
	/*from list*/																		\
	explicit xMat(std::initializer_list<T> list)										\
	{																					\
		int m = std::min(size, int(list.size()));										\
		auto iter = list.begin();														\
		for (int i = 0; i < m; ++i, ++iter)												\
			data[i] = T(*iter);															\
		for (int i = m; i < size; ++i)													\
			data[i] = T(0);																\
	}																					\
	/*copy constructor*/																\
	xMat(const xMat &src)																\
	{																					\
		std::memcpy(data, src.data, sizeof(T)*size);									\
	}																					\
																						\
	template <typename T, typename U>													\
	explicit xMat(const xMat<U, n_row, n_col> & src)									\
	{																					\
		for (int i = 0; i < size; ++i)													\
		{																				\
			data[i] = static_cast<T>(src.data[i]);										\
		}																				\
	}																					\
																						\
	xMat& operator=(const xMat &src)													\
	{																					\
		std::memcpy(data, src.data, sizeof(T)*size);									\
		return *this;																	\
	}																					\
																						\
	template <typename T, typename U>													\
	xMat& operator=(const xMat<U, n_row, n_col> &src)									\
	{																					\
		for (int i = 0; i < size; ++i)													\
			data[i] = static_cast<T>(src.data[i]);										\
		return *this;																	\
	}																					\
																						\
	/*destructor*/																		\
	~xMat() {};																			\
																						\
	/*              - Unary operations -                         */						\
	inline xMat operator +() { return *this; }											\
																						\
	inline xMat operator -() const														\
	{																					\
		xMat res;																		\
		for (int i = 0; i < size; ++i)													\
			res.data[i] = -data[i];														\
		return res;																		\
	}																					\
																						\
	/*              - Scalar operations -                         */					\
	/*multiplication*/																	\
	xMat &operator *=(const T s)														\
	{																					\
		for (int i = 0; i < size; ++i)													\
			data[i] *= s;																\
		return *this;																	\
	}																					\
																						\
	xMat &operator /=(const T s)														\
	{																					\
		auto den = T(1) / s;															\
		for (int i = 0; i < size; ++i)													\
			data[i] *= den;																\
		return *this;																	\
	}																					\
																						\
	/*              - Matrix operations -                         */					\
	/*addition*/																		\
	friend xMat operator+(xMat m1, const xMat &m2)										\
	{																					\
		return m1 += m2;																\
	}																					\
																						\
	xMat& operator +=(const xMat &m)													\
	{																					\
		for (int i = 0; i < size; ++i)													\
			data[i] += m.data[i];														\
		return *this;																	\
	}																					\
																						\
	/*subtraction*/																		\
	friend xMat operator-(xMat m1, const xMat &m2)										\
	{																					\
		return m1 -= m2;																\
	}																					\
																						\
	xMat& operator -=(const xMat &m)													\
	{																					\
		for (int i = 0; i < size; ++i)													\
			data[i] -= m.data[i];														\
		return *this;																	\
	}																					\
																						\
	/*multiplication*/																	\
	template<int n2_row, int n2_col>													\
	xMat<T, n_row, n2_col> operator*(xMat<T, n2_row, n2_col> rhs) const					\
	{																					\
		assert(n_col == n2_row);														\
		static xMat<T, n_row, n2_col> res;												\
		for (int i = 0; i < n_row; ++i)													\
		{																				\
			for (int j = 0; j < n2_col; ++j)											\
			{																			\
				T c = 0;																\
				for (int k = 0; k < n_col; ++k)											\
				{																		\
					c += (*this)(i, k)*rhs(k, j);										\
				}																		\
				res(i, j) = c;															\
			}																			\
		}																				\
		return res;																		\
	}																					

#define SQ_MAT(n)																		\
	/*diag*/																			\
	xMat(T d)																			\
	{																					\
		for (int i = 0; i < size; ++i)													\
		{																				\
			data[i] = (i % (n + 1)) ? 0 : d;											\
		}																				\
	}																					\
	/*set identity*/																	\
	void setIdentity()																	\
	{																					\
		for (int i = 0; i < size; ++i)													\
		{																				\
			data[i] = (i % (n + 1)) ? 0 : T(1);											\
		}																				\
	}																					\
																						\
	static xMat identity()													     		\
	{																					\
		return xMat(T(1));																\
	}																					\
																						\
	/*in place matrix multiplication*/													\
	xMat & operator *=(const xMat & rhs)												\
	{																					\
		xMat res;																		\
		for (int i = 0; i < n; ++i)														\
		{																				\
			for (int j = 0; j < n; ++j)													\
			{																			\
				T c = 0;																\
				for (int k = 0; k < n; ++k)												\
				{																		\
					c += data[k*n + i] * rhs(k, j);										\
				}																		\
				res.data[j*n + i] = c;													\
			}																			\
		}																				\
		(*this) = xMat(res);															\
		return *this;																	\
	}

		//General template
		template<typename T, int n_row, int n_col>
		class xMat
		{
		public:
			union {
				T data[n_row*n_col];
				xVec<T, n_row> col[n_col];
			};
			int size = n_row * n_col;
			int m_row = n_row;
			int m_col = n_col;
			BASIC_MAT(n_row, n_col);
		};

#pragma warning(push)
#pragma warning(disable: 4201)

		//Specialization square
		template<typename T, int n>
		class xMat<T, n, n>
		{
		public:
			union {
				T data[n*n];
				xVec<T, n> col[n];
			};
			int size = n*n;
			int m_row = n;
			int m_col = n;
			BASIC_MAT(n, n);
			SQ_MAT(n);
		};

		//Specialization square 2
		template<typename T>
		class xMat<T, 2, 2>
		{
		public:
			union {
				T data[4];
				xVec<T, 2> col[2];
			};
			int size = 4;
			int m_row = 2;
			int m_col = 2;
			BASIC_MAT(2, 2);
			SQ_MAT(2);
			xMat(const xVec<T, 2> &c1, const xVec<T, 2> &c2)
			{
				col[0] = c1;
				col[1] = c2;
			}
		};

		//inverse
		template<typename T>
		xMat<T, 2, 2> inverse(const xMat<T, 2, 2> & mat)
		{
			xMat<T, 2, 2> result;
			T det = mat.col[0][0] * mat.col[1][1] - mat.col[0][1] * mat.col[1][0];
			T invdet = T(1 / det);
			result.col[0][0] = mat.col[1][1] * invdet;
			result.col[1][0] = -mat.col[1][0] * invdet;
			result.col[0][1] = -mat.col[0][1] * invdet;
			result.col[1][1] = mat.col[0][0] * invdet;

			return result;
		}

		//det
		template<typename T>
		T det(const xMat<T, 2, 2> & mat)
		{
			return mat.col[0][0] * mat.col[1][1] - mat.col[0][1] * mat.col[1][0];
		}

		typedef xMat<float, 2, 2> xMat2f;
		typedef xMat<float, 2, 2> mat2;
		typedef xMat<double, 2, 2> xMat2d;
		typedef xMat<int, 2, 2> xMat2i;

		//Specialization square 3
		template<typename T>
		class xMat<T, 3, 3>
		{
		public:
			union {
				T data[9];
				xVec<T, 3> col[3];
			};
			int size = 9;
			int m_row = 3;
			int m_col = 3;
			BASIC_MAT(3, 3);
			SQ_MAT(3);
			xMat(const xVec<T, 3> &c1, const xVec<T, 3> &c2, const xVec<T, 3> &c3)
			{
				col[0] = c1;
				col[1] = c2;
				col[2] = c3;
			}
		};

		//inverse
		template<typename T>
		xMat<T, 3, 3> inverse(const xMat<T, 3, 3> & mat)
		{
			xMat<T, 3, 3> result;
			xVec<T, 3> cof(
				cofactor<T, 0, 0>(mat),
				cofactor<T, 1, 0>(mat),
				cofactor<T, 2, 0>(mat));
			T det = dot(cof, mat.col[0]);
			T invdet = T(1 / det);
			result.col[0][0] = cof[0] * invdet;
			result.col[1][0] = cof[1] * invdet;
			result.col[2][0] = cof[2] * invdet;
			result.col[0][1] = cofactor<T, 0, 1>(mat) * invdet;
			result.col[1][1] = cofactor<T, 1, 1>(mat) * invdet;
			result.col[2][1] = cofactor<T, 2, 1>(mat) * invdet;
			result.col[0][2] = cofactor<T, 0, 2>(mat) * invdet;
			result.col[1][2] = cofactor<T, 1, 2>(mat) * invdet;
			result.col[2][2] = cofactor<T, 2, 2>(mat) * invdet;

			return result;
		}

		//det
		template<typename T>
		T det(const xMat<T, 3, 3> & mat)
		{
			xVec<T, 3> cof(
				cofactor<T, 0, 0>(mat),
				cofactor<T, 1, 0>(mat),
				cofactor<T, 2, 0>(mat));
			return dot(cof, mat.col[0]);
		}

		//cofactor
		template<typename T, int i, int j>
		T cofactor(const xMat<T, 3, 3> & mat)
		{
			enum
			{
				i1 = (i + 1) % 3,
				i2 = (i + 2) % 3,
				j1 = (j + 1) % 3,
				j2 = (j + 2) % 3
			};
			return mat.data[3 * j1 + i1] * mat.data[3 * j2 + i2] - mat.data[3 * j2 + i1] * mat.data[3 * j1 + i2];
		}

		typedef xMat<float, 3, 3> xMat3f;
		typedef xMat<float, 3, 3> mat3;
		typedef xMat<double, 3, 3> xMat3d;
		typedef xMat<int, 3, 3> xMat3i;



		//Specialization square 4
		template<typename T>
		class xMat<T, 4, 4>
		{
		public:
			union {
				T data[16];
				xVec<T, 4> col[4];
			};
			int size = 16;
			int m_row = 4;
			int m_col = 4;
			BASIC_MAT(4, 4);
			SQ_MAT(4);
			xMat(const xVec<T, 4> &c1, const xVec<T, 4> &c2, const xVec<T, 4> &c3, const xVec<T, 4> &c4)
			{
				col[0] = c1;
				col[1] = c2;
				col[2] = c3;
				col[3] = c4;
			}

			static xMat<T, 4, 4> orthographic(T left, T right, T bottom, T top, T near_, T far_)
			{
				xMat<T, 4, 4> res(T(1));
				res(0, 0) = T(2 / (right - left));
				res(1, 1) = T(2 / (top - bottom));
				res(2, 2) = T(2 / (near_ - far_));

				res(0, 3) = T((left + right) / (left - right));
				res(1, 3) = T((bottom + top) / (bottom - top));
				res(2, 3) = T((near_ + far_) / (near_ - far_));

				return res;
			}


			static xMat perspective(T fov, T aspectRatio, T near, T far)
			{
				xMat result;
				T q = T(1 / tan(deg2rad(0.5*fov)));
				T a = T(q / aspectRatio);
				T d = T(1 / (near - far));

				result(0, 0) = a;
				result(1, 1) = q;
				result(2, 2) = (near + far)*d;
				result(2, 3) = 2 * far*near*d;
				result(3, 2) = -T(1);

				return result;
			}

			static xMat lookAt(const xVec<T, 3> & camera_pos, const xVec<T, 3> & camera_target, const xVec<T, 3> & world_up)
			{
				xMat M2 = xMat::translation(-camera_pos);
				xMat M1;
				xVec<T, 3> cameraDir = normalize(camera_pos - camera_target);
				xVec<T, 3> cameraRight = normalize(cross(world_up, cameraDir));
				xVec<T, 3> cameraUp = cross(cameraDir, cameraRight);

				M1.col[0][0] = cameraRight.data[0];
				M1.col[1][0] = cameraRight.data[1];
				M1.col[2][0] = cameraRight.data[2];
				M1.col[0][1] = cameraUp.data[0];
				M1.col[1][1] = cameraUp.data[1];
				M1.col[2][1] = cameraUp.data[2];
				M1.col[0][2] = cameraDir.data[0];
				M1.col[1][2] = cameraDir.data[1];
				M1.col[2][2] = cameraDir.data[2];
				M1.col[3][3] = (T)1;

				return M1*M2;
			}

			static xMat translation(const xVec<T, 3> & translation)
			{
				xMat result(T(1));
				result(0, 3) = translation.x;
				result(1, 3) = translation.y;
				result(2, 3) = translation.z;
				return result;
			}

			static xMat rotation(T angle, const xVec<T, 3> & V)
			{
				xMat result(T(1));
				T r = deg2rad(angle);
				T c = T(cos(r));
				T omc = T(1 - c);
				T s = T(sin(r));
				xVec<T, 3> Vnorm = normalize(V);
				T x = Vnorm.x;
				T y = Vnorm.y;
				T z = Vnorm.z;

				result(0, 0) = x*x*omc + c;
				result(1, 1) = y*y*omc + c;
				result(2, 2) = z*z*omc + c;

				result(1, 0) = x*y*omc + z*s;
				result(2, 0) = x*z*omc - y*s;

				result(0, 1) = x*y*omc - z*s;
				result(2, 1) = y*z*omc + x*s;

				result(0, 2) = x*z*omc + y*s;
				result(1, 2) = y*z*omc - x*s;

				return result;
			}

			static xMat scale(const xVec<T, 3> & scale)
			{
				xMat result(T(1));
				result(0, 0) = scale.x;
				result(1, 1) = scale.y;
				result(2, 2) = scale.z;
				return result;
			}

			// mat4 * vec3 mult
			/*xVec<T, 3> operator*(const xVec<T, 3> &v) const
			{
			xVec<T, 3> b;
			for (int i = 0; i < 3; ++i)
			{
			for (int j = 0; j < 3; ++j)
			{
			b.data[i] += col[j][i] * v.data[j];
			}
			b.data[i] += col[3][i];
			}
			return b;
			}*/

			xVec<T, 3> multiply(const xVec<T, 3> & other) const
			{
				return xVec<T, 3>(
					col[0][0] * other.data[0] + col[1][0] * other.data[1] + col[2][0] * other.data[2] + col[3][0],
					col[0][1] * other.data[0] + col[1][1] * other.data[1] + col[2][1] * other.data[2] + col[3][1],
					col[0][2] * other.data[0] + col[1][2] * other.data[1] + col[2][2] * other.data[2] + col[3][2]
					);
			}

			friend xVec<T, 3> operator*(const xMat & M, const xVec<T, 3>& v)
			{
				return M.multiply(v);
			}

		};

		//inverse =====> BETTER in release
		template<typename T>
		xMat<T, 4, 4> inverse(const xMat<T, 4, 4> & mat)
		{
			xMat<T, 4, 4> result;
			xVec<T, 4> cof(
				cofactor<T, 0, 0>(mat),
				-cofactor<T, 1, 0>(mat),
				cofactor<T, 2, 0>(mat),
				-cofactor<T, 3, 0>(mat));
			T det = dot(cof, mat.col[0]);
			T invdet = T(1 / det);
			result.col[0][0] = cof[0] * invdet;
			result.col[1][0] = cof[1] * invdet;
			result.col[2][0] = cof[2] * invdet;
			result.col[3][0] = cof[3] * invdet;
			result.col[0][1] = -cofactor<T, 0, 1>(mat) * invdet;
			result.col[1][1] = cofactor<T, 1, 1>(mat) * invdet;
			result.col[2][1] = -cofactor<T, 2, 1>(mat) * invdet;
			result.col[3][1] = cofactor<T, 3, 1>(mat) * invdet;
			result.col[0][2] = cofactor<T, 0, 2>(mat) * invdet;
			result.col[1][2] = -cofactor<T, 1, 2>(mat) * invdet;
			result.col[2][2] = cofactor<T, 2, 2>(mat) * invdet;
			result.col[3][2] = -cofactor<T, 3, 2>(mat) * invdet;
			result.col[0][3] = -cofactor<T, 0, 3>(mat) * invdet;
			result.col[1][3] = cofactor<T, 1, 3>(mat) * invdet;
			result.col[2][3] = -cofactor<T, 2, 3>(mat) * invdet;
			result.col[3][3] = cofactor<T, 3, 3>(mat) * invdet;

			return result;
		}

		//inverse2 =====> BETTER in debug 
		template<typename T>
		xMat<T, 4, 4> inv(const xMat<T, 4, 4> & mat)
		{
			T inv[16], det;
			int i;
			xMat<T, 4, 4> result;

			inv[0] = mat.data[5] * mat.data[10] * mat.data[15] -
				mat.data[5] * mat.data[11] * mat.data[14] -
				mat.data[9] * mat.data[6] * mat.data[15] +
				mat.data[9] * mat.data[7] * mat.data[14] +
				mat.data[13] * mat.data[6] * mat.data[11] -
				mat.data[13] * mat.data[7] * mat.data[10];

			inv[4] = -mat.data[4] * mat.data[10] * mat.data[15] +
				mat.data[4] * mat.data[11] * mat.data[14] +
				mat.data[8] * mat.data[6] * mat.data[15] -
				mat.data[8] * mat.data[7] * mat.data[14] -
				mat.data[12] * mat.data[6] * mat.data[11] +
				mat.data[12] * mat.data[7] * mat.data[10];

			inv[8] = mat.data[4] * mat.data[9] * mat.data[15] -
				mat.data[4] * mat.data[11] * mat.data[13] -
				mat.data[8] * mat.data[5] * mat.data[15] +
				mat.data[8] * mat.data[7] * mat.data[13] +
				mat.data[12] * mat.data[5] * mat.data[11] -
				mat.data[12] * mat.data[7] * mat.data[9];

			inv[12] = -mat.data[4] * mat.data[9] * mat.data[14] +
				mat.data[4] * mat.data[10] * mat.data[13] +
				mat.data[8] * mat.data[5] * mat.data[14] -
				mat.data[8] * mat.data[6] * mat.data[13] -
				mat.data[12] * mat.data[5] * mat.data[10] +
				mat.data[12] * mat.data[6] * mat.data[9];

			inv[1] = -mat.data[1] * mat.data[10] * mat.data[15] +
				mat.data[1] * mat.data[11] * mat.data[14] +
				mat.data[9] * mat.data[2] * mat.data[15] -
				mat.data[9] * mat.data[3] * mat.data[14] -
				mat.data[13] * mat.data[2] * mat.data[11] +
				mat.data[13] * mat.data[3] * mat.data[10];

			inv[5] = mat.data[0] * mat.data[10] * mat.data[15] -
				mat.data[0] * mat.data[11] * mat.data[14] -
				mat.data[8] * mat.data[2] * mat.data[15] +
				mat.data[8] * mat.data[3] * mat.data[14] +
				mat.data[12] * mat.data[2] * mat.data[11] -
				mat.data[12] * mat.data[3] * mat.data[10];

			inv[9] = -mat.data[0] * mat.data[9] * mat.data[15] +
				mat.data[0] * mat.data[11] * mat.data[13] +
				mat.data[8] * mat.data[1] * mat.data[15] -
				mat.data[8] * mat.data[3] * mat.data[13] -
				mat.data[12] * mat.data[1] * mat.data[11] +
				mat.data[12] * mat.data[3] * mat.data[9];

			inv[13] = mat.data[0] * mat.data[9] * mat.data[14] -
				mat.data[0] * mat.data[10] * mat.data[13] -
				mat.data[8] * mat.data[1] * mat.data[14] +
				mat.data[8] * mat.data[2] * mat.data[13] +
				mat.data[12] * mat.data[1] * mat.data[10] -
				mat.data[12] * mat.data[2] * mat.data[9];

			inv[2] = mat.data[1] * mat.data[6] * mat.data[15] -
				mat.data[1] * mat.data[7] * mat.data[14] -
				mat.data[5] * mat.data[2] * mat.data[15] +
				mat.data[5] * mat.data[3] * mat.data[14] +
				mat.data[13] * mat.data[2] * mat.data[7] -
				mat.data[13] * mat.data[3] * mat.data[6];

			inv[6] = -mat.data[0] * mat.data[6] * mat.data[15] +
				mat.data[0] * mat.data[7] * mat.data[14] +
				mat.data[4] * mat.data[2] * mat.data[15] -
				mat.data[4] * mat.data[3] * mat.data[14] -
				mat.data[12] * mat.data[2] * mat.data[7] +
				mat.data[12] * mat.data[3] * mat.data[6];

			inv[10] = mat.data[0] * mat.data[5] * mat.data[15] -
				mat.data[0] * mat.data[7] * mat.data[13] -
				mat.data[4] * mat.data[1] * mat.data[15] +
				mat.data[4] * mat.data[3] * mat.data[13] +
				mat.data[12] * mat.data[1] * mat.data[7] -
				mat.data[12] * mat.data[3] * mat.data[5];

			inv[14] = -mat.data[0] * mat.data[5] * mat.data[14] +
				mat.data[0] * mat.data[6] * mat.data[13] +
				mat.data[4] * mat.data[1] * mat.data[14] -
				mat.data[4] * mat.data[2] * mat.data[13] -
				mat.data[12] * mat.data[1] * mat.data[6] +
				mat.data[12] * mat.data[2] * mat.data[5];

			inv[3] = -mat.data[1] * mat.data[6] * mat.data[11] +
				mat.data[1] * mat.data[7] * mat.data[10] +
				mat.data[5] * mat.data[2] * mat.data[11] -
				mat.data[5] * mat.data[3] * mat.data[10] -
				mat.data[9] * mat.data[2] * mat.data[7] +
				mat.data[9] * mat.data[3] * mat.data[6];

			inv[7] = mat.data[0] * mat.data[6] * mat.data[11] -
				mat.data[0] * mat.data[7] * mat.data[10] -
				mat.data[4] * mat.data[2] * mat.data[11] +
				mat.data[4] * mat.data[3] * mat.data[10] +
				mat.data[8] * mat.data[2] * mat.data[7] -
				mat.data[8] * mat.data[3] * mat.data[6];

			inv[11] = -mat.data[0] * mat.data[5] * mat.data[11] +
				mat.data[0] * mat.data[7] * mat.data[9] +
				mat.data[4] * mat.data[1] * mat.data[11] -
				mat.data[4] * mat.data[3] * mat.data[9] -
				mat.data[8] * mat.data[1] * mat.data[7] +
				mat.data[8] * mat.data[3] * mat.data[5];

			inv[15] = mat.data[0] * mat.data[5] * mat.data[10] -
				mat.data[0] * mat.data[6] * mat.data[9] -
				mat.data[4] * mat.data[1] * mat.data[10] +
				mat.data[4] * mat.data[2] * mat.data[9] +
				mat.data[8] * mat.data[1] * mat.data[6] -
				mat.data[8] * mat.data[2] * mat.data[5];

			det = mat.data[0] * inv[0] + mat.data[1] * inv[4] + mat.data[2] * inv[8] + mat.data[3] * inv[12];

			if (det == 0)
				return false;

			det = 1.0 / det;

			for (i = 0; i < 16; i++)
				result.data[i] = inv[i] * det;

			return result;
		}

		//det
		template<typename T>
		T det(const xMat<T, 4, 4> & mat)
		{
			xVec<T, 4> cof(
				cofactor<T, 0, 0>(mat),
				-cofactor<T, 1, 0>(mat),
				cofactor<T, 2, 0>(mat),
				-cofactor<T, 3, 0>(mat));
			return dot(cof, mat.col[0]);
		}

		//det_helper
		template<typename T>
		T det_helper(const xMat<T, 4, 4> & mat, int i1, int i2, int i3, int j1, int j2, int j3)
		{
			return mat.col[j1][i1] * (mat.col[j2][i2] * mat.col[j3][i3] - mat.col[j3][i2] * mat.col[j2][i3]);
		}

		//cofactor
		template<typename T, int i, int j>
		T cofactor(const xMat<T, 4, 4> & mat)
		{
			enum
			{
				i1 = (i + 1) % 4,
				i2 = (i + 2) % 4,
				i3 = (i + 3) % 4,
				j1 = (j + 1) % 4,
				j2 = (j + 2) % 4,
				j3 = (j + 3) % 4
			};
			return det_helper(mat, i1, i2, i3, j1, j2, j3)
				+ det_helper(mat, i2, i3, i1, j1, j2, j3)
				+ det_helper(mat, i3, i1, i2, j1, j2, j3);
		}

		typedef xMat<float, 4, 4> xMat4f;
		typedef xMat<float, 4, 4> mat4;
		typedef xMat<double, 4, 4> xMat4d;
		typedef xMat<int, 4, 4> xMat4i;

#undef BASIC_VEC
#undef SQ_MAT

		//template<typename T, typename U, int n_row, int n_col, int n2_row, int n2_col>
		//inline xMat<T, n_row, n2_col> operator *(xMat<T, n_row, n_col> M1, const xMat<T, n2_row, n2_col> & M2)
		//{
		//	return M1 *= M2;
		//}

		template<typename T, int n_row, int n_col>
		inline xMat<T, n_row, n_col> operator *(const T s, xMat<T, n_row, n_col> M)
		{
			return M *= s;
		}

		template<typename T, int n_row, int n_col>
		inline xMat<T, n_row, n_col> operator *(xMat<T, n_row, n_col> M, const T s)
		{
			return M *= s;
		}

		template<typename T, int n_row, int n_col>
		inline xMat<T, n_row, n_col> operator /(xMat<T, n_row, n_col> M, const T s)
		{
			return M /= s;
		}

		/*transpose*/
		template <typename T, int n_row, int n_col>
		xMat<T, n_col, n_row> transpose(const xMat<T, n_row, n_col> & M)
		{
			xMat<T, n_col, n_row> res;
			for (int i = 0; i < n_row; ++i)
			{
				for (int j = 0; j < n_col; ++j)
				{
					res.col[i][j] = M.col[j][i];
				}
			}
			return res;
		}

		/*matrix-vector multiply*/
		template <typename T, int n_row, int n_col>
		xVec<T, n_row> operator*(const xMat<T, n_row, n_col> & A, const xVec<T, n_col> & v)
		{
			xVec<T, n_row> b;
			for (int i = 0; i < n_row; ++i)
			{
				for (int j = 0; j < n_col; ++j)
				{
					b.data[i] += A.col[j][i] * v.data[j];
				}
			}
			return b;
		}

		template <typename T, int n_row, int n_col>
		xVec<T, n_col> operator*(const xVec<T, n_row> & v, const xMat<T, n_row, n_col> & A)
		{
			xVec<T, n_col> b;
			for (int i = 0; i < n_col; ++i)
			{
				b.data[i] = dot(v, A.col[i]);
			}
			return b;
		}

		template <typename T, int n_row, int n_col>
		std::ostream& operator << (std::ostream &os, const xMat<T, n_row, n_col> &M)
		{
			for (int i = 0; i < n_row; ++i)
			{
				os << "|";
				for (int j = 0; j < n_col; ++j)
					os << M.data[j*n_row + i] << ((j != (n_col - 1)) ? " " : "|");
				os << std::endl;
			}
			return os;
		}


		/*static xMat<float, 4, 4> perspectiveGL(float fov, float aspectRatio, float near, float far)
		{
			xMat<float, 4, 4> matrix;

			float ymax, xmax;
			float temp, temp2, temp3, temp4;
			float left, right, bottom, top;

			ymax = near * tanf(fov * 3.141592653589793238462643383 / 360.0f);
			xmax = ymax * aspectRatio;

			left = -xmax;
			right = xmax;
			bottom = -ymax;
			top = ymax;

			temp = 2.0f * near;
			temp2 = right - left;
			temp3 = top - bottom;
			temp4 = far - near;

			matrix.data[0] = temp / temp2;
			matrix.data[1] = 0.0f;
			matrix.data[2] = 0.0f;
			matrix.data[3] = 0.0f;
			matrix.data[4] = 0.0f;
			matrix.data[5] = temp / temp3;
			matrix.data[6] = 0.0f;
			matrix.data[7] = 0.0f;
			matrix.data[8] = (right + left) / temp2;
			matrix.data[9] = (top + bottom) / temp3;
			matrix.data[10] = (-far - near) / temp4;
			matrix.data[11] = -1.0f;
			matrix.data[12] = 0.0f;
			matrix.data[13] = 0.0f;
			matrix.data[14] = (-temp * far) / temp4;
			matrix.data[15] = 0.0f;

			return matrix;
		}*/

	}
}