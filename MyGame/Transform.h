#pragma once
//DebugRenderer.cpp

#include "PlatformInterface.h"
#include <memory>

#include "Runtime/Math/Quaternion.h"

class Transform
{
public:
	typedef std::vector<Transform* >	TransformComList;
	typedef TransformComList::iterator	iterator;

public:
	Transform();
	~Transform();


private:
	Quaternionf                      m_LocalRotation;
	Vector3f                         m_LocalPosition;
	Vector3f                         m_LocalScale;

	mutable Matrix4x4f               m_CachedTransformMatrix;
	mutable UInt8                    m_CachedTransformType;
	mutable UInt8                    m_HasCachedTransformMatrix;
	mutable UInt8                    m_HasChanged;

public:
	UInt8                            m_InternalTransformType;
	UInt8                            m_SupportsTransformChanged;

	TransformComList                 m_Children;
	Transform*          m_Father;
public:
	enum SetParentOption { kWorldPositionStays = 1 << 0, kLocalPositionStays = 1 << 1, kAllowParentingFromPrefab = 1 << 2, kDisableTransformMessage = 1 << 3 };
	bool SetParent(Transform * parent, SetParentOption options = kWorldPositionStays);

	/// Returns a ptr to the father transformcomponent (NULL if no father)
	Transform *GetParent() const { return m_Father; }
	/// Finds given transform
	iterator Find(const Transform* child);

	/// access to the children
	int GetChildrenCount()const { return m_Children.size(); }
	Transform &GetChild(int i) const { DebugAssertIf(i < (int)m_Children.size()); return *m_Children[i]; }
	iterator begin() { return m_Children.begin(); }

	iterator end() { return m_Children.end(); }
	/// Sets the rotation in local space
	void SetLocalRotation(const Quaternionf& rotation);
	/// Sets the Rotation in world space
	void SetRotation(const Quaternionf& rotation);
	/// Sets the local euler angles
	void SetLocalEulerAngles(const Vector3f& eulerAngles);

	/// Sets the position in local space
	/// (If the object has no father, localspace is basically the same as world space)
	void SetLocalPosition(const Vector3f& inTranslation);
	/// Sets the position in world space
	void SetPosition(const Vector3f& position);

	/// Sets the position - a local space offset will be scaled, rotated and subtracted from the position.
	/// Used to set the position For CharacterController and NavMeshAgent that have baseOffset / center.
	/// For retreiving the position with a local offset use: TransformPoint (localOffset)
	void SetPositionWithLocalOffset(const Vector3f& p, const Vector3f& localOffset);

	/// Transforms local space position to world space - while applying an offset which is scaled and rotated accordingly.
	Vector3f TransformPointWithLocalOffset(const Vector3f& p, const Vector3f& localOffset) const;

	/// Same as above but normalizes the quaternion
	void SetLocalRotationSafe(const Quaternionf& rotation);
	void SetRotationSafe(const Quaternionf& rotation);

	/// Sets the scale in local space
	void SetLocalScale(const Vector3f& scale);
	/// Sets the scale from a rotation * scale
	/// The transform can not hold the full scale if it contains skew
	void SetWorldRotationAndScale(const Matrix3x3f& worldRotationAndScale);

	/// Sets the world position and rotation	
	void SetPositionAndRotation(const Vector3f& position, const Quaternionf& rotation);
	void SetLocalPositionAndRotation(const Vector3f& position, const Quaternionf& rotation);


	/// Return matrix that converts a point from World To Local space
	Matrix4x4f GetWorldToLocalMatrix() const;
	/// Return matrix that converts a point from Local To World space
	Matrix4x4f GetLocalToWorldMatrix() const;

	Matrix4x4f GetWorldToLocalMatrixNoScale() const;
	const Matrix4x4f& GetWorldToLocalMatrixNoScale(Matrix4x4f& m) const;
	Matrix4x4f GetLocalToWorldMatrixNoScale() const;
	const Matrix4x4f& GetLocalToWorldMatrixNoScale(Matrix4x4f& m) const;


	enum { kPositionChanged = 1 << 0, kRotationChanged = 1 << 1, kScaleChanged = 1 << 3, kAnimatePhysics = 1 << 4, kParentingChanged = 1 << 5 };
	void SendTransformChanged(int mask);
	void SetDirty() {  }
	void SetCacheDirty();


	/// Gets the rotation from local to world space
	Quaternionf GetRotation() const;
	/// Gets the local rotation
	Quaternionf GetLocalRotation() const { return m_LocalRotation; }
	/// Gets the local euler angles (in the editor it is first ensures that they are in sync with the local rotation quaternion)
	Vector3f GetLocalEulerAngles();

	/// Gets the local position relative to the father
	Vector3f GetLocalPosition() const { return m_LocalPosition; }
	/// Gets the position in world space
	Vector3f GetPosition() const;

	/// Returns the local scale	
	Vector3f GetLocalScale() const { return m_LocalScale; }
	/// Returns the world rotation and scale.
	/// (It is impossible to return a Vector3 because the scale might be skewed)
	Matrix3x3f GetWorldRotationAndScale() const;

	Matrix3x3f GetWorldScale() const;

	Vector3f GetWorldScaleLossy() const;

	/// transforms a point from localspace to worldspace
	Vector3f TransformPoint(const Vector3f& inPoint) const;
	/// Transforms a direction from localspace to worldspace
	/// (Ignores scale)
	Vector3f TransformDirection(const Vector3f& inDirection) const;

	/// Transforms a point from worldspace to localspace
	Vector3f InverseTransformPoint(const Vector3f& inPosition) const;
	/// Transforms a direction from worldspace to localspace
	/// (Ignores scale)
	Vector3f InverseTransformDirection(const Vector3f& inDirection) const;


	// For use only by the animation system	
	void RecalculateTransformType();

	TransformType CalculateTransformMatrix(Matrix4x4f& matrix) const;


	void GetPositionAndRotation(Vector3f& pos, Quaternionf& q)const;
private:
		TransformType CalculateLocalTransformMatrix(Matrix4x4f& matrix) const;
		TransformType CalculateTransformMatrixIterative(Matrix4x4f& matrix) const;

private:

};

