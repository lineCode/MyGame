#include "Quaternion.h"

Quaternionf Slerp(const Quaternionf& q1, const Quaternionf& q2, float t)
{
	//	Quaternionf q3 = new Quaternionf();
	float dot = Dot(q1, q2);

	// dot = cos(theta)
	// if (dot < 0), q1 and q2 are more than 90 degrees apart,
	// so we can invert one to reduce spinning
	Quaternionf tmpQuat;
	if (dot < 0.0f)
	{
		dot = -dot;
		tmpQuat.Set(-q2.x,
			-q2.y,
			-q2.z,
			-q2.w);
	}
	else
		tmpQuat = q2;


	if (dot < 0.95f)
	{
		float angle = acos(dot);
		float sinadiv, sinat, sinaomt;
		sinadiv = 1.0f / sin(angle);
		sinat = sin(angle*t);
		sinaomt = sin(angle*(1.0f - t));
		tmpQuat.Set((q1.x*sinaomt + tmpQuat.x*sinat)*sinadiv,
			(q1.y*sinaomt + tmpQuat.y*sinat)*sinadiv,
			(q1.z*sinaomt + tmpQuat.z*sinat)*sinadiv,
			(q1.w*sinaomt + tmpQuat.w*sinat)*sinadiv);
		//		AssertIf (!CompareApproximately (SqrMagnitude (tmpQuat), 1.0F));
		return tmpQuat;

	}

	// if the angle is small, use linear interpolation

	else
	{
		return Lerp(q1, tmpQuat, t);
	}

}

// Version of QuaternionToEuler that prevents "snapping" on X when getting
// close to gimbal lock.  Noticeably changes behavior compared to version
// above, so deactivated for now.

Vector3f QuaternionToEuler(const Quaternionf& q)
{
	const float sqw = q.w * q.w;
	const float sqx = q.x * q.x;
	const float sqy = q.y * q.y;
	const float sqz = q.z * q.z;

	const float unit = sqx + sqy + sqz + sqw;
	const float test = q.x * q.y + q.z * q.w;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	// North pole singularity
	if (test > 0.499f * unit)
	{
		yaw = 2.0f * atan2(q.x, q.w);
		pitch = kPI * 0.5f;
		roll = 0.0f;
	}

	// South pole singularity
	else if (test < -0.499f * unit)
	{
		yaw = -2.0f * atan2(q.x, q.w);
		pitch = -kPI * 0.5f;
		roll = 0.0f;
	}

	else
	{
		yaw = atan2(2.0f * q.y * q.w - 2.0f * q.x * q.z, sqx - sqy - sqz + sqw);
		pitch = asin(2.0f * test / unit);
		roll = atan2(2.0f * q.x * q.w - 2.0f * q.y * q.z, -sqx + sqy - sqz + sqw);
	}

	// Keep angles [0..360].
	if (Sign(yaw) < 0.f)
		yaw = Deg2Rad(360.f) + yaw;
	if (Sign(pitch) < 0.f)
		pitch = Deg2Rad(360.f) + pitch;
	if (Sign(roll) < 0.f)
		roll = Deg2Rad(360.f) + roll;

	return Vector3f(roll, yaw, pitch);
}

std::vector<Vector3f> GetEquivalentEulerAngles(const Quaternionf& quat)
{
	Matrix3x3f m;
	Vector3f rot;

	std::vector<Vector3f> euler_triples;

	QuaternionToMatrix(quat, m);
	MatrixToEuler(m, rot);

	euler_triples.push_back(rot);

	euler_triples.push_back(Vector3f(rot.x + 180.0f, -rot.y, rot.z + 180.0f));
	euler_triples.push_back(Vector3f(rot.x - 180.0f, -rot.y, rot.z - 180.0f));
	euler_triples.push_back(Vector3f(-rot.x, rot.y + 180.0f, -rot.z));
	euler_triples.push_back(Vector3f(-rot.x, rot.y - 180.0f, -rot.z));

	return euler_triples;
}

Quaternionf EulerToQuaternion(const Vector3f& someEulerAngles)
{
	float cX(cos(someEulerAngles.x / 2.0f));
	float sX(sin(someEulerAngles.x / 2.0f));

	float cY(cos(someEulerAngles.y / 2.0f));
	float sY(sin(someEulerAngles.y / 2.0f));

	float cZ(cos(someEulerAngles.z / 2.0f));
	float sZ(sin(someEulerAngles.z / 2.0f));

	Quaternionf qX(sX, 0.0F, 0.0F, cX);
	Quaternionf qY(0.0F, sY, 0.0F, cY);
	Quaternionf qZ(0.0F, 0.0F, sZ, cZ);

	Quaternionf q = (qY * qX) * qZ;
	DebugAssertIf(!CompareApproximately(SqrMagnitude(q), 1.0F));
	return q;
}

void QuaternionToMatrix(const Quaternionf& q, Matrix3x3f& m)
{
	// If q is guaranteed to be a unit quaternion, s will always
	// be 1.  In that case, this calculation can be optimized out.
#if DEBUGMODE
	if (!CompareApproximately(SqrMagnitude(q), 1.0F, Vector3f::epsilon))
	{
		AssertString(Format("Quaternion To Matrix conversion failed because input Quaternion is invalid {%f, %f, %f, %f} l=%f", q.x, q.y, q.z, q.w, SqrMagnitude(q)));
	}
#endif
	//float norm = GetNorm (q);
	//float s = (norm > 0.0) ? 2.0/norm : 0;

	// Precalculate coordinate products
	float x = q.x * 2.0F;
	float y = q.y * 2.0F;
	float z = q.z * 2.0F;
	float xx = q.x * x;
	float yy = q.y * y;
	float zz = q.z * z;
	float xy = q.x * y;
	float xz = q.x * z;
	float yz = q.y * z;
	float wx = q.w * x;
	float wy = q.w * y;
	float wz = q.w * z;

	// Calculate 3x3 matrix from orthonormal basis
	m.m_Data[0] = 1.0f - (yy + zz);
	m.m_Data[1] = xy + wz;
	m.m_Data[2] = xz - wy;

	m.m_Data[3] = xy - wz;
	m.m_Data[4] = 1.0f - (xx + zz);
	m.m_Data[5] = yz + wx;

	m.m_Data[6] = xz + wy;
	m.m_Data[7] = yz - wx;
	m.m_Data[8] = 1.0f - (xx + yy);
}

void QuaternionToMatrix(const Quaternionf& q, Matrix4x4f& m)
{
	// If q is guaranteed to be a unit quaternion, s will always
	// be 1.  In that case, this calculation can be optimized out.
#if DEBUGMODE
	if (!CompareApproximately(SqrMagnitude(q), 1.0F, Vector3f::epsilon))
	{
		AssertString(Format("Quaternion To Matrix conversion failed because input Quaternion is invalid {%f, %f, %f, %f} l=%f", q.x, q.y, q.z, q.w, SqrMagnitude(q)));
	}
#endif

	//float norm = GetNorm (q);
	//float s = (norm > 0.0) ? 2.0/norm : 0;

	// Precalculate coordinate products
	float x = q.x * 2.0F;
	float y = q.y * 2.0F;
	float z = q.z * 2.0F;
	float xx = q.x * x;
	float yy = q.y * y;
	float zz = q.z * z;
	float xy = q.x * y;
	float xz = q.x * z;
	float yz = q.y * z;
	float wx = q.w * x;
	float wy = q.w * y;
	float wz = q.w * z;

	// Calculate 3x3 matrix from orthonormal basis
	m.m_Data[0] = 1.0f - (yy + zz);
	m.m_Data[1] = xy + wz;
	m.m_Data[2] = xz - wy;
	m.m_Data[3] = 0.0F;

	m.m_Data[4] = xy - wz;
	m.m_Data[5] = 1.0f - (xx + zz);
	m.m_Data[6] = yz + wx;
	m.m_Data[7] = 0.0F;

	m.m_Data[8] = xz + wy;
	m.m_Data[9] = yz - wx;
	m.m_Data[10] = 1.0f - (xx + yy);
	m.m_Data[11] = 0.0F;

	m.m_Data[12] = 0.0F;
	m.m_Data[13] = 0.0F;
	m.m_Data[14] = 0.0F;
	m.m_Data[15] = 1.0F;
}

bool LookRotationToQuaternion(const Vector3f& viewVec, const Vector3f& upVec, Quaternionf* res)
{
	Matrix3x3f m;
	if (!LookRotationToMatrix(viewVec, upVec, &m))
		return false;
	MatrixToQuaternion(m, *res);
	return true;
}

float AngularDistance(const Quaternionf& lhs, const Quaternionf& rhs)
{
	float dot = Dot(lhs, rhs);
	if (dot < 0.0f)
		dot = -dot;
	return acos((std::min)(1.0F, dot)) * 2.0F;

}

Quaternionf FromToQuaternionSafe(const Vector3f& lhs, const Vector3f& rhs)
{
	float lhsMag = Magnitude(lhs);
	float rhsMag = Magnitude(rhs);
	if (lhsMag < Vector3f::epsilon || rhsMag < Vector3f::epsilon)
		return Quaternionf::identity();
	else
		return FromToQuaternion(lhs / lhsMag, rhs / rhsMag);
}

Quaternionf FromToQuaternion(const Vector3f& from, const Vector3f& to)
{
	Matrix3x3f m;
	m.SetFromToRotation(from, to);
	Quaternionf q;
	MatrixToQuaternion(m, q);
	return q;
}

void MatrixToQuaternion(const Matrix3x3f& kRot, Quaternionf& q)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternionf Calculus and Fast Animation".
#if _DEBUG
	float det = kRot.GetDeterminant();
	DebugAssertIf(!CompareApproximately(det, 1.0F, .005f));
#endif
	float fTrace = kRot.Get(0, 0) + kRot.Get(1, 1) + kRot.Get(2, 2);
	float fRoot;

	if (fTrace > 0.0f)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0f);  // 2w
		q.w = 0.5f*fRoot;
		fRoot = 0.5f / fRoot;  // 1/(4w)
		q.x = (kRot.Get(2, 1) - kRot.Get(1, 2))*fRoot;
		q.y = (kRot.Get(0, 2) - kRot.Get(2, 0))*fRoot;
		q.z = (kRot.Get(1, 0) - kRot.Get(0, 1))*fRoot;
	}
	else
	{
		// |w| <= 1/2
		int s_iNext[3] = { 1, 2, 0 };
		int i = 0;
		if (kRot.Get(1, 1) > kRot.Get(0, 0))
			i = 1;
		if (kRot.Get(2, 2) > kRot.Get(i, i))
			i = 2;
		int j = s_iNext[i];
		int k = s_iNext[j];

		fRoot = sqrt(kRot.Get(i, i) - kRot.Get(j, j) - kRot.Get(k, k) + 1.0f);
		float* apkQuat[3] = { &q.x, &q.y, &q.z };
		DebugAssertIf(fRoot < Vector3f::epsilon);
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f / fRoot;
		q.w = (kRot.Get(k, j) - kRot.Get(j, k)) * fRoot;
		*apkQuat[j] = (kRot.Get(j, i) + kRot.Get(i, j))*fRoot;
		*apkQuat[k] = (kRot.Get(k, i) + kRot.Get(i, k))*fRoot;
	}
	q = Normalize(q);
}

void MatrixToQuaternion(const Matrix4x4f& m, Quaternionf& q)
{
	Matrix3x3f mat(
		m.Get(0, 0), m.Get(0, 1), m.Get(0, 2),
		m.Get(1, 0), m.Get(1, 1), m.Get(1, 2),
		m.Get(2, 0), m.Get(2, 1), m.Get(2, 2));

	MatrixToQuaternion(mat, q);
}
