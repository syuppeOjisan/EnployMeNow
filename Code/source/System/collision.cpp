#include	"System/collision.h"

using namespace DirectX::SimpleMath;

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width) {

	// �����ȃx�N�g�������߂�
	Vector3 zaxis(0, 0, 1);
	Vector3 line = endpos - startpos;

	Vector3 ans;
	ans = line.Cross(zaxis);					// �O��
	ans.Normalize();							// ���K��

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = 0.0f;

	// �����̃x�N�g�������߂�
	double ABx = segment.endpoint.x - segment.startpoint.x;
	double ABy = segment.endpoint.y - segment.startpoint.y;
	double ABz = segment.endpoint.z - segment.startpoint.z;

	// �����̎n�_�Ɠ_�����ԃx�N�g�������߂�
	double APx = point.x - segment.startpoint.x;
	double APy = point.y - segment.startpoint.y;
	double APz = point.z - segment.startpoint.z;

	// ����AB�̒��������߂�
	double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

	// AB�x�N�g����AP�x�N�g���̓��ς��v�Z����i�ˉe�������������߂�j
	double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

	// tt���v�Z
	double tt = ABdotAP / AB2;

	// �����̑�
	intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
	intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
	intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

	t = static_cast<float>(tt);
	// �����̑��̒���
	distance = (intersectionpoint - point).Length();
	return distance;
}

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = calcPointLineDist(p, segment, intersectionpoint, t);

	// ��_�������̊O�ɂ���i�n�_�ɋ߂��j
	if (t < 0.0f) {

		intersectionpoint = segment.startpoint;		// �J�n�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_�������̊O�ɂ���i�I�_�ɋ߂��j
	if (t > 1.0f) {

		intersectionpoint = segment.endpoint;		// �I�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_��������ɂ���i�O�������P�j
	return distance;
}

// ���������������i�l�p�`�j�Ƌ��̓����蔻����s��
bool CollisionSphereOrientedQuad(
	Vector3 StartPos,						// �J�n�ʒu
	Vector3 EndPos,							// �I���ʒu
	float width,							// ��
	float radius,							// ���a
	Vector3 CirclePos)						// ���S���W
{

	Vector3 v[4];							// �l�p�`���_
	Vector3 intersectionpoint{};			// ��_

	// �����ɕ��s�ȂS�p�`�̍��W�����߂�
	CalcQuadOrientedLine(StartPos, EndPos, v, width);

	// �����i�S�p�`�̐����j
	Segment s[4];
	s[0].startpoint = v[0];
	s[0].endpoint = v[1];

	s[1].startpoint = v[2];
	s[1].endpoint = v[3];

	s[2].startpoint = v[0];
	s[2].endpoint = v[2];

	s[3].startpoint = v[1];
	s[3].endpoint = v[3];


	float t;

	// �l�p�`���͂ސ����Ɠ_�̋����𒲂ׂ�
	for (int idx = 0; idx < 4; idx++) {
		// �~�̒��S�Ɛ����̋��������߂�
		float distance = calcPointSegmentDist(CirclePos, s[idx], intersectionpoint, t);

		if (distance < radius) {

			return true;
		}
	}

	return false;

}

// �܂������ɗ����Ă�~���Ƌ��̓����蔻��
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
{
	// XZ���ʂł̃`�F�b�N
	{
		// XZ���ʂł̒��S�ԋ��������߂�
		Vector3 p1center = sphere.center;
		Vector3 p2center = cylinder.bottom;

		p1center.y = 0.0f;
		p2center.y = 0.0f;

		float length = (p1center - p2center).Length();

		// �d�Ȃ肪���邩�H
		if (sphere.radius + cylinder.radius < length) {
			return false;
		}
	}

	// XY���ʂł̃`�F�b�N
	{
		// XY���ʂōl����
		Vector3 p1center = sphere.center;				// ���̒��S
		Vector3 p2bottomcenter = cylinder.bottom;		// �~���̒�
		Vector3 p2topcenter = cylinder.top;				// �~���̏��

		p1center.z = 0.0f;

		p2bottomcenter.z = 0.0f;
		p2topcenter.z = 0.0f;

		// ����
		Segment seg;
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// �J�n�ʒu
			seg.endpoint,							// �I���ʒu
			cylinder.radius * 2.0f,					// ��
			sphere.radius,							// ���a
			p1center);								// ���S���W

		if (sts) {
			return true;
		}
	}

	// YZ���ʂł̃`�F�b�N
	{
		// YZ���ʂōl����
		Vector3 p1center = sphere.center;							// ���̒��S
		Vector3 p2bottomcenter = cylinder.bottom;					// �~���̒�
		Vector3 p2topcenter = cylinder.top;							// �~���̏��

		p1center.z = 0.0f;

		p2bottomcenter.x = 0.0f;
		p2topcenter.x = 0.0f;

		Segment seg;			// ����
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// �J�n�ʒu
			seg.endpoint,							// �I���ʒu
			cylinder.radius * 2.0f,					// ��
			sphere.radius,							// ���a
			p1center);								// ���S���W

		if (sts) {
			return true;
		}
	}

	return false;
}

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2)
{

	float length = (p1.center - p2.center).Length();

	if (p1.radius + p2.radius > length) {
		return false;
	}

	return true;
}

// AABB
bool CollisionAABB(AABB p1, AABB p2){

	// X���W
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y���W
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z���W
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}

bool CollisionOBB(const OBB& obbA, const OBB& obbB)
{
	//������
	Vector3 vecSeparate;

	// 2�̃I�u�W�F�N�g�����񂾃x�N�g�����v�Z
	Vector3 vecDistance;
	vecDistance = obbB.worldcenter - obbA.worldcenter;

	bool sts;	// �߂�l

	// OBB-A�����X�g
	const Vector3* OBB_A_Axis[3] = 
	{
			&obbA.axisX,
			&obbA.axisY,
			&obbA.axisZ
	};

	// OBB-B�����X�g
	const Vector3* OBB_B_Axis[3] = 
	{
			&obbB.axisX,
			&obbB.axisY,
			&obbB.axisZ
	};

	// OBB-A�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {

		Vector3 vecSeparate = *OBB_A_Axis[i];

		sts = CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// OBB-B�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {
		Vector3 vecSeparate = *OBB_B_Axis[i];

		sts = CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// �O�ς𕪗����Ƃ��Čv�Z
	for (int p1 = 0; p1 < 3; p1++) {
		for (int p2 = 0; p2 < 3; p2++) {

			Vector3 obbAaxis = *OBB_A_Axis[p1];
			Vector3 obbBaxis = *OBB_B_Axis[p2];

			Vector3	crossseparate;			// �O�σx�N�g��
			crossseparate = obbAaxis.Cross(obbBaxis);

			sts = CompareLength(
				obbA,				// OBB-A
				obbB,				// OBB-B
				crossseparate,		// ������
				vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
			if (sts == false) {
				return false;
			}
		}
	}

	return true;
}


AABB MakeAABB(Vector3 centerposition,float width ,float height,float depth) {

	AABB aabb{};

	width = width;
	height = height;
	depth = depth;

	width = fabs(width);
	height = fabs(height);
	depth = fabs(depth);

	aabb.min.x = centerposition.x - width / 2.0f;
	aabb.min.y = centerposition.y - height / 2.0f;
	aabb.min.z = centerposition.z - depth / 2.0f;

	aabb.max.x = centerposition.x + width / 2.0f;
	aabb.max.y = centerposition.y + height / 2.0f;
	aabb.max.z = centerposition.z + depth / 2.0f;

	return aabb;
}

OBB MakeOBB(const std::vector<DEFORM_VERTEX>& vertices)
{
	aiVector3D max;
	aiVector3D min;
	OBB obb;

	// �ő�l�ƍŏ��l�����߂�
	min = max = vertices[0].Position;
	for (auto& v : vertices) {
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	obb.min = Vector3(min.x, min.y, min.z);
	obb.max = Vector3(max.x, max.y, max.z);

	// ���S�����߂�
	obb.worldcenter = obb.center = (obb.max + obb.min) * 0.5f;

	// ����ݒ�
	obb.axisX = Vector3(1, 0, 0);
	obb.axisY = Vector3(0, 1, 0);
	obb.axisZ = Vector3(0, 0, 1);

	// ������ݒ�
	DirectX::SimpleMath::Vector3 len;
	len = obb.max - obb.min;

	obb.lengthx = fabs(len.x);
	obb.lengthy = fabs(len.y);
	obb.lengthz = fabs(len.z);

	return obb;
}

// ���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
void UpdateOBB(Matrix worldmtx,Vector3 scale, OBB& obbIn) 
{
	// �o�͗p
	OBB obbOut;

	// ���s�ړ�
	obbOut.worldcenter = Vector3::Transform(obbIn.center, worldmtx);

	// �ړ������J�b�g
	worldmtx._41 = 0.0f;
	worldmtx._42 = 0.0f;
	worldmtx._43 = 0.0f;

	// ����]
	obbOut.axisX = DirectX::SimpleMath::Vector3::Transform(obbIn.axisX, worldmtx);
	obbOut.axisY = DirectX::SimpleMath::Vector3::Transform(obbIn.axisY, worldmtx);
	obbOut.axisZ = DirectX::SimpleMath::Vector3::Transform(obbIn.axisZ, worldmtx);

	// ���𐳋K��
	obbOut.axisX.Normalize();
	obbOut.axisY.Normalize();
	obbOut.axisZ.Normalize();

	// �g�k���l��
	obbOut.lengthx *= scale.x;
	obbOut.lengthy *= scale.y;
	obbOut.lengthz *= scale.z;

	obbIn = obbOut;
}

// OBB�̏d�Ȃ�𔻒肷��
bool CompareLength(
	const OBB& ObbA,		// OBB-A
	const OBB& ObbB,		// OBB-B
	const Vector3& vecSeparate,		// ������
	const Vector3& vecDistance)		// ���S���W�����񂾃x�N�g��
{
	float fDistance{};

	// �������Ɏˉe�������S�Ԃ����ԃx�N�g��
	fDistance = vecDistance.Dot(vecSeparate);	// ���ς��v�Z
	fDistance = fabsf(fDistance);				// �v���X�ɂ���

	//��������Ƀ{�b�N�XA���ˉe�����e�̒���
	float fShadowA = 0;

	//��������Ƀ{�b�N�X�a���ˉe�����e�̒���
	float fShadowB = 0;

	//�{�b�N�XA�́h�e�h���Z�o
	float fShadowAx{};
	float fShadowAy{};
	float fShadowAz{};

	// X���𕪗����Ɏˉe
	fShadowAx = vecSeparate.Dot(ObbA.axisX);
	fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowAy = vecSeparate.Dot(ObbA.axisY);
	fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowAz = vecSeparate.Dot(ObbA.axisZ);
	fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowA = fShadowAx + fShadowAy + fShadowAz;

	//�{�b�N�XB�́h�e�h���Z�o
	float fShadowBx{};
	float fShadowBy{};
	float fShadowBz{};

	// X���𕪗����Ɏˉe
	fShadowBx = vecSeparate.Dot(ObbB.axisX);
	fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowBy = vecSeparate.Dot(ObbB.axisY);
	fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowBz = vecSeparate.Dot(ObbB.axisZ);
	fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowB = fShadowBx + fShadowBy + fShadowBz;

	if (fDistance > fShadowA + fShadowB) {
		return false;
	}

	return true;
}