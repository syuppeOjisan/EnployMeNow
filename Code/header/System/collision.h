#pragma once
#include	<simplemath.h>
#include "Component/animationModel.h"

// �~����`
struct BoundingCylinder 
{
	DirectX::SimpleMath::Vector3    bottom;						// ��
	DirectX::SimpleMath::Vector3    top;						// ��
	float	radius;												// ���a
};

// ����`
struct BoundingSphere
{
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// �Z�O�����g�i�����j
struct Segment 
{
	DirectX::SimpleMath::Vector3		startpoint;				// �J�n�_
	DirectX::SimpleMath::Vector3		endpoint;				// �I���_
};

// BOX��`
struct AABB 
{
	DirectX::SimpleMath::Vector3	min;
	DirectX::SimpleMath::Vector3	max;
};

// OBB��`
struct OBB : public AABB
{
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// ���S���iX���AY���AZ���j
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOX�T�C�Y
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB�̓����蔻��
bool CollisionAABB(AABB p1, AABB p2);

// OBB�̓����蔻��
bool CollisionOBB(const OBB& obbA,const OBB& obbB);

// make AABB
AABB MakeAABB(DirectX::SimpleMath::Vector3 centerposition, float width, float height, float depth);

// make OBB from vertex
OBB MakeOBB(const std::vector<DEFORM_VERTEX>& vertices);

// OBB�X�V
void UpdateOBB(DirectX::SimpleMath::Matrix worldmtx, DirectX::SimpleMath::Vector3 scale, OBB& obbIn);

// OBB�̏d�Ȃ�𔻒肷��
bool CompareLength(
	const OBB& ObbA,		// OBB-A
	const OBB& ObbB,		// OBB-B
	const DirectX::SimpleMath::Vector3& vecSeparate,		// ������
	const DirectX::SimpleMath::Vector3& vecDistance);		// ���S���W�����񂾃x�N�g��


// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width);

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const DirectX::SimpleMath::Vector3& p,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// ���������������i�l�p�`�j�Ƌ��̓����蔻����s��
bool CollisionSphereOrientedQuad(
	DirectX::SimpleMath::Vector3 StartPos,					// �J�n�ʒu
	DirectX::SimpleMath::Vector3 EndPos,					// �I���ʒu
	float width,											// ��
	float radius,											// ���a
	DirectX::SimpleMath::Vector3 CirclePos,					// ���S���W
	DirectX::SimpleMath::Vector3& intersectionpoint);		// ��_					

// �܂������ɗ����Ă�~���Ƌ��̓����蔻��
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder);