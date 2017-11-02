#pragma once
#include <algorithm>
#include <cmath>
#include <limits>
#include <math.h>


const float XM_PI = 3.141592654f;
const float XM_2PI = 6.283185307f;
const float XM_1DIVPI = 0.318309886f;
const float XM_1DIV2PI = 0.159154943f;
const float XM_PIDIV2 = 1.570796327f;
const float XM_PIDIV4 = 0.785398163f;

#ifndef kPI
#define kPI 3.14159265358979323846264338327950288419716939937510F
#endif

const float kBiggestFloatSmallerThanOne = 0.99999994f;
const double kBiggestDoubleSmallerThanOne = 0.99999999999999989;

#if defined(_XBOX)
#define __FSELF __fself
#elif defined(SN_TARGET_PS3)
#define __FSELF __fsels
#endif

inline float FloatMin(float a, float b)
{
#if defined(_XBOX) || defined(SN_TARGET_PS3)
	return __FSELF((a)-(b), b, a);
#else
	return (std::min)(a, b);
#endif
}


inline float FloatMax(float a, float b)
{
#if defined(_XBOX) || defined(SN_TARGET_PS3)
	return __FSELF((a)-(b), a, b);
#else
	return (std::max)(a, b);
#endif
}

inline float Abs(float v)
{
#if defined(__ppc__) && (defined(__MWERKS__) || defined(SN_TARGET_PS3))
	return __fabsf(v);
#elif defined(_XBOX)
	return __fabs(v);
#else
	return v < 0.0F ? -v : v;
#endif
}

inline double Abs(double v)
{
	return v < 0.0 ? -v : v;
}

inline int Abs(int v)
{
	return v < 0 ? -v : v;
}


inline int FloorfToInt(float f)
{
	DebugAssertIf(f < INT_MIN || f > INT_MAX);
	return f >= 0 ? (int)f : (int)(f - kBiggestFloatSmallerThanOne);
}

inline UInt32 FloorfToIntPos(float f)
{
	DebugAssertIf(f < 0 || f > UINT_MAX);
	return (UInt32)f;
}

inline float Floorf(float f)
{
	// Use std::floor().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return floor(f);
}

inline double Floord(double f)
{
	// Use std::floor().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return floor(f);
}


inline int CeilfToInt(float f)
{
	DebugAssertIf(f < INT_MIN || f > INT_MAX);
	return f >= 0 ? (int)(f + kBiggestFloatSmallerThanOne) : (int)(f);
}

inline UInt32 CeilfToIntPos(float f)
{
	DebugAssertIf(f < 0 || f > UINT_MAX);
	return (UInt32)(f + kBiggestFloatSmallerThanOne);
}

inline float Ceilf(float f)
{
	// Use std::ceil().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return ceil(f);
}

inline double Ceild(double f)
{
	// Use std::ceil().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return ceil(f);
}


inline int RoundfToInt(float f)
{
	return FloorfToInt(f + 0.5F);
}

inline UInt32 RoundfToIntPos(float f)
{
	return FloorfToIntPos(f + 0.5F);
}

inline float Roundf(float f)
{
	return Floorf(f + 0.5F);
}

inline double Roundf(double f)
{
	return Floord(f + 0.5);
}


///  Fast conversion of float [0...1] to 0 ... 65535
inline int NormalizedToWord(float f)
{
	f = FloatMax(f, 0.0F);
	f = FloatMin(f, 1.0F);
	return RoundfToIntPos(f * 65535.0f);
}

///  Fast conversion of float [0...1] to 0 ... 65535
inline float WordToNormalized(int p)
{
	DebugAssertIf(p < 0 || p > 65535);
	return (float)p / 65535.0F;
}

///  Fast conversion of float [0...1] to 0 ... 255
inline int NormalizedToByte(float f)
{
	f = FloatMax(f, 0.0F);
	f = FloatMin(f, 1.0F);
	return RoundfToIntPos(f * 255.0f);
}

///  Fast conversion of float [0...1] to 0 ... 255
inline float ByteToNormalized(int p)
{
	DebugAssertIf(p < 0 || p > 255);
	return (float)p / 255.0F;
}


// Returns float remainder for t / length
inline float Repeat(float t, float length)
{
	return t - Floorf(t / length) * length;
}

// Returns double remainder for t / length
inline double RepeatD(double t, double length)
{
	return t - floor(t / length) * length;
}

// Returns relative angle on the interval (-pi, pi]
inline float DeltaAngleRad(float current, float target)
{
	float delta = Repeat((target - current), 2 * kPI);
	if (delta > kPI)
		delta -= 2 * kPI;
	return delta;
}

// Returns true if the distance between f0 and f1 is smaller than epsilon
inline bool CompareApproximately(float f0, float f1, float epsilon = 0.000001F)
{
	float dist = (f0 - f1);
	dist = Abs(dist);
	return dist < epsilon;
}

/// CopySignf () returns x with its sign changed to y's.
inline float CopySignf(float x, float y)
{
	union
	{
		float f;
		UInt32 i;
	} u, u0, u1;
	u0.f = x; u1.f = y;
	UInt32 a = u0.i;
	UInt32 b = u1.i;
	SInt32 mask = 1 << 31;
	UInt32 sign = b & mask;
	a &= ~mask;
	a |= sign;

	u.i = a;
	return u.f;
}

// Returns the t^2
template<class T>
T Sqr(const T& t)
{
	return t * t;
}

#define kDeg2Rad (2.0F * kPI / 360.0F)
#define kRad2Deg (1.F / kDeg2Rad)

inline float Deg2Rad(float deg)
{
	// TODO : should be deg * kDeg2Rad, but can't be changed, 
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return deg / 360.0F * 2.0F * kPI;
}

inline float Rad2Deg(float rad)
{
	// TODO : should be rad * kRad2Deg, but can't be changed, 
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return rad / 2.0F / kPI * 360.0F;
}

inline float Lerp(float from, float to, float t)
{
	return to * t + from * (1.0F - t);
}

inline bool IsNAN(float value)
{
#if defined __APPLE_CC__
	return value != value;
#elif _MSC_VER
	return _isnan(value) != 0;
#else
	return isnan(value);
#endif
}

inline bool IsNAN(double value)
{
#if defined __APPLE_CC__
	return value != value;
#elif _MSC_VER
	return _isnan(value) != 0;
#else
	return isnan(value);
#endif
}

inline bool IsPlusInf(float value) { return value == std::numeric_limits<float>::infinity(); }
inline bool IsMinusInf(float value) { return value == -std::numeric_limits<float>::infinity(); }

inline bool IsFinite(const float& value)
{
	// Returns false if value is NaN or +/- infinity
	UInt32 intval = *reinterpret_cast<const UInt32*>(&value);
	return (intval & 0x7f800000) != 0x7f800000;
}

inline bool IsFinite(const double& value)
{
	// Returns false if value is NaN or +/- infinity
	UInt64 intval = *reinterpret_cast<const UInt64*>(&value);
	return (intval & 0x7ff0000000000000LL) != 0x7ff0000000000000LL;
}

inline float InvSqrt(float p) { return 1.0F / sqrt(p); }
inline float Sqrt(float p) { return sqrt(p); }

inline float SqrtImpl(float f)
{
	return sqrt(f);
}

inline float FastSqrt(float value)
{
	return sqrtf(value);
}

inline float FastInvSqrt(float f)
{
	// The Newton iteration trick used in FastestInvSqrt is a bit faster on
	// Pentium4 / Windows, but lower precision. Doing two iterations is precise enough,
	// but actually a bit slower.
	if (fabs(f) == 0.0F)
		return f;
	return 1.0F / sqrtf(f);
}

inline float FastestInvSqrt(float f)
{
	union
	{
		float f;
		int i;
	} u;
	float fhalf = 0.5f*f;
	u.f = f;
	int i = u.i;
	i = 0x5f3759df - (i >> 1);
	u.i = i;
	f = u.f;
	f = f*(1.5f - fhalf*f*f);
	// f = f*(1.5f - fhalf*f*f); // uncommenting this would be two iterations
	return f;
}


inline float Sin(float f)
{
	return sinf(f);
}

inline float Pow(float f, float f2)
{
	return powf(f, f2);
}

inline float Cos(float f)
{
	return cosf(f);
}

inline float Sign(float f)
{
#if defined(_XBOX)
	return __fsel(f, 1.0f, -1.0f);
#else
	if (f < 0.0F)
		return -1.0F;
	else
		return 1.0;
#endif
}