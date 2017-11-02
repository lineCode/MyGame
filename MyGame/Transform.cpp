
#include "Transform.h"

inline float InverseSafe(float f)
{
	if (Abs(f) > Vector3f::epsilon)
		return 1.0F / f;
	else
		return 0.0F;

}

inline Vector3f InverseSafe(const Vector3f& v)
{
	return Vector3f(InverseSafe(v.x), InverseSafe(v.y), InverseSafe(v.z));
}

static TransformType DetectActualNegativeScale(int type, const Transform* transform)
{
	type &= ~kOddNegativeScaleTransform;

	// Calculate if we need to flip the back facing when rendering
	// We need to use backface rendering if one or three scale axes are negative (odd count)
	// In this case we enable kOddNegativeScaleTransform flag

	Transform* cur = (Transform *)transform;
	while (cur)
	{
		TransformType parentType = (TransformType)cur->m_InternalTransformType;
		// kOddNegativeScaleTransform is XOR against parent (odd+odd=even, odd+even=odd, even+even=even), other bits are OR
		type = ((type | parentType) & ~kOddNegativeScaleTransform) | ((type ^ parentType) & kOddNegativeScaleTransform);
		cur = cur->GetParent();
	}

	return (TransformType)type;
}
static TransformType UpdateTransformType(TransformType type, const Transform* transform)
{
	if ((type & kOddNegativeScaleTransform) != 0)
		type = DetectActualNegativeScale(type, transform);

	// kNonUniformScaleTransform 'overwrites' kUniformScaleTransform
	if ((type & kNonUniformScaleTransform) != 0)
		type &= ~kUniformScaleTransform;

	DebugAssertIf((type & (kNonUniformScaleTransform | kUniformScaleTransform)) != (kNonUniformScaleTransform | kUniformScaleTransform));
	return type;
}

Transform::Transform()
{
	m_InternalTransformType = kNoScaleTransform;
}


Transform::~Transform()
{
}

bool Transform::SetParent(Transform * newFather, SetParentOption options /*= kWorldPositionStays*/)
{
	// Make sure that the new father is not a child of this transform.
	Transform* cur;
	for (cur = newFather; cur != NULL; cur = cur->m_Father)
	{
		if (this == cur)
			return false;
	}

	if ((options & kAllowParentingFromPrefab) == 0)
	{

	}
	Vector3f worldPosition = GetPosition();
	Quaternionf worldRotation = GetRotation();
	Matrix3x3f worldScale = GetWorldRotationAndScale();

	// If it already has an father, remove this from fathers children
	Transform* father = GetParent();
	if (father != NULL)
	{
		// Find this in fathers children list
		iterator i = father->Find(this);
		DebugAssertIf(i == father->end());
		father->m_Children.erase(i);
		father->SetDirty();
	}

	if (newFather)
	{
		newFather->m_Children.push_back(this);
		newFather->SetDirty();
	}
	m_Father = newFather;
	if (!(options & kDisableTransformMessage))
	{
		if (options & kWorldPositionStays)
		{
			// Restore old position so they stay at the same position in worldspace
			SetRotationSafe(worldRotation);
			SetPosition(worldPosition);
			SetWorldRotationAndScale(worldScale);
			SendTransformChanged(kParentingChanged);
		}
		else
			SendTransformChanged(kPositionChanged | kRotationChanged | kScaleChanged | kParentingChanged);
	}
	SetDirty();
	SetCacheDirty();

	return true;
}

Transform::iterator Transform::Find(const Transform* child)
{
	iterator it, itEnd = end();
	for (it = begin(); it != itEnd; ++it)
	{
		if (*it == child)
			return it;
	}
	return itEnd;
}

void Transform::SetLocalRotation(const Quaternionf& q)
{
	m_LocalRotation = q;
	SetDirty();
	SendTransformChanged(kRotationChanged);
}

void Transform::SetLocalRotationSafe(const Quaternionf& q)
{
	SetLocalRotation(NormalizeSafe(q));
}

void Transform::SetRotationSafe(const Quaternionf& q)
{
	Transform* father = GetParent();
	if (father != NULL)
		SetLocalRotation(NormalizeSafe(Inverse(father->GetRotation()) * q));
	else
		SetLocalRotation(NormalizeSafe(q));
}

void Transform::SetLocalScale(const Vector3f& scale)
{
	m_LocalScale = scale;
	RecalculateTransformType();
	SetDirty();
	SendTransformChanged(kScaleChanged | kRotationChanged | kPositionChanged);
}

void Transform::SetWorldRotationAndScale(const Matrix3x3f& scale)
{
	m_LocalScale = Vector3f::one;

	Matrix3x3f inverseRS = GetWorldRotationAndScale();
	inverseRS.Invert();

	inverseRS = inverseRS * scale;

	m_LocalScale.x = inverseRS.Get(0, 0);
	m_LocalScale.y = inverseRS.Get(1, 1);
	m_LocalScale.z = inverseRS.Get(2, 2);

	RecalculateTransformType();
	SetDirty();
	SendTransformChanged(kScaleChanged | kRotationChanged | kPositionChanged);
}

void Transform::SetPositionAndRotation(const Vector3f& position, const Quaternionf& rotation)
{

}

void Transform::SetLocalPositionAndRotation(const Vector3f& p, const Quaternionf& q)
{
	m_LocalPosition = p;
	m_LocalRotation = q;
	SetDirty();
	SendTransformChanged(kPositionChanged | kRotationChanged);
}

Matrix4x4f Transform::GetWorldToLocalMatrix() const
{
	Matrix4x4f m, temp;
	m.SetTRInverse(m_LocalPosition, m_LocalRotation);
	if (m_InternalTransformType != kNoScaleTransform)
	{
		Matrix4x4f scale;
		scale.SetScale(InverseSafe(m_LocalScale));
		MultiplyMatrices4x4(&scale, &m, &temp);
		CopyMatrix(temp.GetPtr(), m.GetPtr());
	}

	Transform* father = GetParent();
	if (father != NULL)
	{
		Matrix4x4f parentMat = father->GetWorldToLocalMatrix();
		MultiplyMatrices4x4(&m, &parentMat, &temp);
		CopyMatrix(temp.GetPtr(), m.GetPtr());
	}

	return m;
}

Matrix4x4f Transform::GetLocalToWorldMatrix() const
{
	Matrix4x4f m;
	CalculateTransformMatrix(m);
	return m;
}

Matrix4x4f Transform::GetWorldToLocalMatrixNoScale() const
{
	Matrix4x4f m;
	GetWorldToLocalMatrixNoScale(m);
	return m;
}

const Matrix4x4f& Transform::GetWorldToLocalMatrixNoScale(Matrix4x4f& m) const
{
	Vector3f pos;
	Quaternionf rot;
	GetPositionAndRotation(pos, rot);
	m.SetTRInverse(pos, rot);
	return m;
}

Matrix4x4f Transform::GetLocalToWorldMatrixNoScale() const
{
	Matrix4x4f m;
	GetLocalToWorldMatrixNoScale(m);
	return m;
}

const Matrix4x4f& Transform::GetLocalToWorldMatrixNoScale(Matrix4x4f& m) const
{
	Quaternionf rot; Vector3f pos;
	GetPositionAndRotation(pos, rot);
	m.SetTR(pos, rot);
	return m;
}

void Transform::SetRotation(const Quaternionf& q)
{
	Transform* father = GetParent();
	if (father != NULL)
		SetLocalRotation(Inverse(father->GetRotation()) * q);
	else
		SetLocalRotation(q);
}

void Transform::SetLocalEulerAngles(const Vector3f& eulerAngles)
{
	SetLocalRotationSafe(EulerToQuaternion(eulerAngles * Deg2Rad(1)));
}

void Transform::SetLocalPosition(const Vector3f& inTranslation)
{
	m_LocalPosition = inTranslation;
	SetDirty();
	SendTransformChanged(kPositionChanged);
}

void Transform::SetPosition(const Vector3f& p)
{
	Vector3f newPosition = p;
	Transform* father = GetParent();
	if (father != NULL)
		newPosition = father->InverseTransformPoint(newPosition);

	SetLocalPosition(newPosition);
}

void Transform::SetPositionWithLocalOffset(const Vector3f& p, const Vector3f& localOffset)
{
	Vector3f newPosition = p - TransformPoint(localOffset) + GetPosition();
	SetPosition(newPosition);
}

Vector3f Transform::TransformPointWithLocalOffset(const Vector3f& p, const Vector3f& localOffset) const
{
	return p - TransformPoint(localOffset) + GetPosition();
}

void Transform::SendTransformChanged(int mask)
{

}

void Transform::SetCacheDirty()
{
	m_HasCachedTransformMatrix = false;
	m_HasChanged = true;

	TransformComList::iterator end = m_Children.end();
	for (TransformComList::iterator i = m_Children.begin(); i != end; ++i)
		(*i)->SetCacheDirty();
}

Quaternionf Transform::GetRotation() const
{
	Quaternionf worldRot = m_LocalRotation;
	Transform* cur = GetParent();
	while (cur)
	{
		worldRot = cur->m_LocalRotation * worldRot;
		cur = cur->GetParent();
	}

	return worldRot;
}

Vector3f Transform::GetLocalEulerAngles()
{
	Quaternionf rotation = NormalizeSafe(m_LocalRotation);
	return QuaternionToEuler(rotation) * Rad2Deg(1);
}

Vector3f Transform::GetPosition() const
{
	Vector3f worldPos = m_LocalPosition;
	Transform* cur = GetParent();
	while (cur)
	{
		worldPos.Scale(cur->m_LocalScale);
		worldPos = RotateVectorByQuat(cur->m_LocalRotation, worldPos);
		worldPos += cur->m_LocalPosition;

		cur = cur->GetParent();
	}

	return worldPos;
}

Matrix3x3f Transform::GetWorldRotationAndScale() const
{
	Matrix3x3f scale;
	scale.SetScale(m_LocalScale);

	Matrix3x3f rotation;
	QuaternionToMatrix(m_LocalRotation, rotation);

	Transform* parent = GetParent();
	if (parent)
	{
		///@TODO optimize: Special case multiplication
		Matrix3x3f parentTransform = parent->GetWorldRotationAndScale();
		return parentTransform * rotation * scale;
	}
	else
	{
		return rotation * scale;
	}
}

Matrix3x3f Transform::GetWorldScale() const
{
	Matrix3x3f invRotation;
	QuaternionToMatrix(Inverse(GetRotation()), invRotation);
	Matrix3x3f scaleAndRotation = GetWorldRotationAndScale();
	return invRotation * scaleAndRotation;
}

Vector3f Transform::GetWorldScaleLossy() const
{
	Matrix3x3f rot = GetWorldScale();
	return Vector3f(rot.Get(0, 0), rot.Get(1, 1), rot.Get(2, 2));
}

Vector3f Transform::TransformPoint(const Vector3f& inPoint) const
{
	Vector3f worldPos = inPoint;

	const Transform* cur = this;
	while (cur)
	{
		worldPos.Scale(cur->m_LocalScale);
		worldPos = RotateVectorByQuat(cur->m_LocalRotation, worldPos);
		worldPos += cur->m_LocalPosition;

		cur = cur->GetParent();
	}
	return worldPos;
}

Vector3f Transform::InverseTransformPoint(const Vector3f& inPosition) const
{
	Vector3f newPosition, localPosition;
	Transform* father = GetParent();
	if (father)
		localPosition = father->InverseTransformPoint(inPosition);
	else
		localPosition = inPosition;

	localPosition -= m_LocalPosition;
	newPosition = RotateVectorByQuat(Inverse(m_LocalRotation), localPosition);
	if (m_InternalTransformType != kNoScaleTransform)
		newPosition.Scale(InverseSafe(m_LocalScale));

	return newPosition;
}

Vector3f Transform::TransformDirection(const Vector3f& inDirection) const
{
	return RotateVectorByQuat(GetRotation(), inDirection);
}

Vector3f Transform::InverseTransformDirection(const Vector3f& inDirection) const
{
	return RotateVectorByQuat(Inverse(GetRotation()), inDirection);
}

void Transform::RecalculateTransformType()
{
	// #pragma message ("Compare approximately is bad due to epsilon changing with the size of the value")
	if (CompareApproximately(m_LocalScale.x, m_LocalScale.y, 0.0001F) && CompareApproximately(m_LocalScale.y, m_LocalScale.z, 0.0001F))
	{
		if (CompareApproximately(m_LocalScale.x, 1.0F, 0.0001F))
		{
			m_InternalTransformType = kNoScaleTransform;
		}
		else
		{
			m_InternalTransformType = kUniformScaleTransform;
			if (m_LocalScale.x < 0.0F)
			{
				m_InternalTransformType = kOddNegativeScaleTransform | kNonUniformScaleTransform;
			}
		}
	}
	else
	{
		m_InternalTransformType = kNonUniformScaleTransform;

		int hasOddNegativeScale = m_LocalScale.x * m_LocalScale.y * m_LocalScale.z < 0.0F ? 1 : 0;
		m_InternalTransformType |= (TransformType)(hasOddNegativeScale * kOddNegativeScaleTransform);
	}
}

TransformType Transform::CalculateTransformMatrix(Matrix4x4f& transform) const
{
	//@TODO: Does this give any performance gain??
	if (m_HasCachedTransformMatrix)
	{
		CopyMatrix(m_CachedTransformMatrix.GetPtr(), transform.GetPtr());
		return (TransformType)m_CachedTransformType;
	}

	const Transform* transforms[32];
	int transformCount = 1;
	TransformType type = (TransformType)0;
	Matrix4x4f temp;

	{
		// collect all transform that need CachedTransformMatrix update
		transforms[0] = this;
		Transform* parent = NULL;
		for (parent = GetParent(); parent != NULL && !parent->m_HasCachedTransformMatrix; parent = parent->GetParent())
		{
			transforms[transformCount++] = parent;
			// reached maximum of transforms that we can calculate - fallback to old method
			if (transformCount == 31)
			{
				parent = parent->GetParent();
				if (parent)
				{
					type = parent->CalculateTransformMatrixIterative(temp);
					DebugAssertIf(parent->m_HasCachedTransformMatrix);
				}
				break;
			}
		}

		// storing parent of last transform (can be null), the transform itself won't be updated
		transforms[transformCount] = parent;
		DebugAssertIf(transformCount <= 31);
	}

	// iterate transforms from lowest parent
	for (int i = transformCount - 1; i >= 0; --i)
	{
		const Transform* t = transforms[i];
		const Transform* parent = transforms[i + 1];
		if (parent)
		{
			DebugAssertIf(parent->m_HasCachedTransformMatrix);
			// Build the local transform into temp
			type |= t->CalculateLocalTransformMatrix(temp);
			type |= (TransformType)parent->m_CachedTransformType;
			MultiplyMatrices4x4(&parent->m_CachedTransformMatrix, &temp, &t->m_CachedTransformMatrix);
		}
		else
		{
			// Build the local transform into m_CachedTransformMatrix
			type |= t->CalculateLocalTransformMatrix(t->m_CachedTransformMatrix);
		}
		// store cached transform
		t->m_CachedTransformType = UpdateTransformType(type, t);
		t->m_HasCachedTransformMatrix = true;
	}

	DebugAssertIf(m_HasCachedTransformMatrix);
	CopyMatrix(m_CachedTransformMatrix.GetPtr(), transform.GetPtr());
	return (TransformType)m_CachedTransformType;
}


void Transform::GetPositionAndRotation(Vector3f& pos, Quaternionf& q) const
{
	Vector3f worldPos = m_LocalPosition;
	Quaternionf worldRot = m_LocalRotation;
	Transform* cur = GetParent();
	while (cur)
	{
		worldPos.Scale(cur->m_LocalScale);
		worldPos = RotateVectorByQuat(cur->m_LocalRotation, worldPos);
		worldPos += cur->m_LocalPosition;

		worldRot = cur->m_LocalRotation * worldRot;

		cur = cur->GetParent();
	}

	pos = worldPos;
	q = worldRot;
}

TransformType Transform::CalculateLocalTransformMatrix(Matrix4x4f& matrix) const
{
	TransformType type = (TransformType)m_InternalTransformType;
	if (type == kNoScaleTransform)
		matrix.SetTR(m_LocalPosition, m_LocalRotation);
	else
		matrix.SetTRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
	return type;
}


// This method doesn't cache all transforms - just the last one, but can calculate
// more than 32 transforms. CalculateTransformMatrix caches all results
TransformType Transform::CalculateTransformMatrixIterative(Matrix4x4f& transform) const
{
	if (m_HasCachedTransformMatrix)
	{
		CopyMatrix(m_CachedTransformMatrix.GetPtr(), transform.GetPtr());
		return (TransformType)m_CachedTransformType;
	}

	// Build the local transform
	TransformType type = CalculateLocalTransformMatrix(transform);

	Transform* parent = GetParent();
	Matrix4x4f temp;
	while (parent != NULL)
	{
		if (parent->m_HasCachedTransformMatrix)
		{
			type |= (TransformType)parent->m_CachedTransformType;
			MultiplyMatrices4x4(&parent->m_CachedTransformMatrix, &transform, &temp);
			// no need to iterate further - we got world transform
			parent = NULL;
		}
		else
		{
			Matrix4x4f parentTransform;
			type |= parent->CalculateLocalTransformMatrix(parentTransform);
			MultiplyMatrices4x4(&parentTransform, &transform, &temp);
			parent = parent->GetParent();
		}
		CopyMatrix(temp.GetPtr(), transform.GetPtr());
	}

	CopyMatrix(transform.GetPtr(), m_CachedTransformMatrix.GetPtr());
	m_CachedTransformType = UpdateTransformType(type, this);
	m_HasCachedTransformMatrix = true;
	return type;
}
