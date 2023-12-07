#pragma once
#include	<simplemath.h>
#include "Component/animationModel.h"

// 円柱定義
struct BoundingCylinder 
{
	DirectX::SimpleMath::Vector3    bottom;						// 底
	DirectX::SimpleMath::Vector3    top;						// 上
	float	radius;												// 半径
};

// 球定義
struct BoundingSphere
{
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// セグメント（線分）
struct Segment 
{
	DirectX::SimpleMath::Vector3		startpoint;				// 開始点
	DirectX::SimpleMath::Vector3		endpoint;				// 終了点
};

// BOX定義
struct AABB 
{
	DirectX::SimpleMath::Vector3	min;
	DirectX::SimpleMath::Vector3	max;
};

// OBB定義
struct OBB : public AABB
{
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// 中心軸（X軸、Y軸、Z軸）
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOXサイズ
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

// 球
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABBの当たり判定
bool CollisionAABB(AABB p1, AABB p2);

// OBBの当たり判定
bool CollisionOBB(const OBB& obbA,const OBB& obbB);

// make AABB
AABB MakeAABB(DirectX::SimpleMath::Vector3 centerposition, float width, float height, float depth);

// make OBB from vertex
OBB MakeOBB(const std::vector<DEFORM_VERTEX>& vertices);

// OBB更新
void UpdateOBB(DirectX::SimpleMath::Matrix worldmtx, DirectX::SimpleMath::Vector3 scale, OBB& obbIn);

// OBBの重なりを判定する
bool CompareLength(
	const OBB& ObbA,		// OBB-A
	const OBB& ObbB,		// OBB-B
	const DirectX::SimpleMath::Vector3& vecSeparate,		// 分離軸
	const DirectX::SimpleMath::Vector3& vecDistance);		// 中心座標を結んだベクトル


// 直線と点の距離を求める
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width);

// 直線と点の距離を求める
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 線分と直線の長さを求める
float calcPointSegmentDist(
	const DirectX::SimpleMath::Vector3& p,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 幅をもった線分（四角形）と球の当たり判定を行う
bool CollisionSphereOrientedQuad(
	DirectX::SimpleMath::Vector3 StartPos,					// 開始位置
	DirectX::SimpleMath::Vector3 EndPos,					// 終了位置
	float width,											// 幅
	float radius,											// 半径
	DirectX::SimpleMath::Vector3 CirclePos,					// 中心座標
	DirectX::SimpleMath::Vector3& intersectionpoint);		// 交点					

// まっすぐに立ってる円柱と球の当たり判定
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder);