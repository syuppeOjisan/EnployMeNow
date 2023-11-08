#include	"System/collision.h"

using namespace DirectX::SimpleMath;

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width) {

	// 垂直なベクトルを求める
	Vector3 zaxis(0, 0, 1);
	Vector3 line = endpos - startpos;

	Vector3 ans;
	ans = line.Cross(zaxis);					// 外積
	ans.Normalize();							// 正規化

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}

// 直線と点の距離を求める
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = 0.0f;

	// 線分のベクトルを求める
	double ABx = segment.endpoint.x - segment.startpoint.x;
	double ABy = segment.endpoint.y - segment.startpoint.y;
	double ABz = segment.endpoint.z - segment.startpoint.z;

	// 線分の始点と点を結ぶベクトルを求める
	double APx = point.x - segment.startpoint.x;
	double APy = point.y - segment.startpoint.y;
	double APz = point.z - segment.startpoint.z;

	// 線分ABの長さを求める
	double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

	// ABベクトルとAPベクトルの内積を計算する（射影した長さを求める）
	double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

	// ttを計算
	double tt = ABdotAP / AB2;

	// 垂線の足
	intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
	intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
	intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

	t = static_cast<float>(tt);
	// 垂線の足の長さ
	distance = (intersectionpoint - point).Length();
	return distance;
}

// 線分と直線の長さを求める
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = calcPointLineDist(p, segment, intersectionpoint, t);

	// 交点が線分の外にある（始点に近い）
	if (t < 0.0f) {

		intersectionpoint = segment.startpoint;		// 開始点が交点

		float l = (p - intersectionpoint).Length();	// 交点との距離を求める

		return l;
	}

	// 交点が線分の外にある（終点に近い）
	if (t > 1.0f) {

		intersectionpoint = segment.endpoint;		// 終点が交点

		float l = (p - intersectionpoint).Length();	// 交点との距離を求める

		return l;
	}

	// 交点が線分上にある（０＜ｔ＜１）
	return distance;
}

// 幅をもった線分（四角形）と球の当たり判定を行う
bool CollisionSphereOrientedQuad(
	Vector3 StartPos,						// 開始位置
	Vector3 EndPos,							// 終了位置
	float width,							// 幅
	float radius,							// 半径
	Vector3 CirclePos)						// 中心座標
{

	Vector3 v[4];							// 四角形頂点
	Vector3 intersectionpoint{};			// 交点

	// 線分に平行な４角形の座標を求める
	CalcQuadOrientedLine(StartPos, EndPos, v, width);

	// 線分（４角形の線分）
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

	// 四角形を囲む線分と点の距離を調べる
	for (int idx = 0; idx < 4; idx++) {
		// 円の中心と線分の距離を求める
		float distance = calcPointSegmentDist(CirclePos, s[idx], intersectionpoint, t);

		if (distance < radius) {

			return true;
		}
	}

	return false;

}

// まっすぐに立ってる円柱と球の当たり判定
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
{
	// XZ平面でのチェック
	{
		// XZ平面での中心間距離を求める
		Vector3 p1center = sphere.center;
		Vector3 p2center = cylinder.bottom;

		p1center.y = 0.0f;
		p2center.y = 0.0f;

		float length = (p1center - p2center).Length();

		// 重なりがあるか？
		if (sphere.radius + cylinder.radius < length) {
			return false;
		}
	}

	// XY平面でのチェック
	{
		// XY平面で考える
		Vector3 p1center = sphere.center;				// 球の中心
		Vector3 p2bottomcenter = cylinder.bottom;		// 円柱の底
		Vector3 p2topcenter = cylinder.top;				// 円柱の上面

		p1center.z = 0.0f;

		p2bottomcenter.z = 0.0f;
		p2topcenter.z = 0.0f;

		// 線分
		Segment seg;
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// 開始位置
			seg.endpoint,							// 終了位置
			cylinder.radius * 2.0f,					// 幅
			sphere.radius,							// 半径
			p1center);								// 中心座標

		if (sts) {
			return true;
		}
	}

	// YZ平面でのチェック
	{
		// YZ平面で考える
		Vector3 p1center = sphere.center;							// 球の中心
		Vector3 p2bottomcenter = cylinder.bottom;					// 円柱の底
		Vector3 p2topcenter = cylinder.top;							// 円柱の上面

		p1center.z = 0.0f;

		p2bottomcenter.x = 0.0f;
		p2topcenter.x = 0.0f;

		Segment seg;			// 線分
		seg.startpoint = p2bottomcenter;
		seg.endpoint = p2topcenter;

		bool sts = CollisionSphereOrientedQuad(
			seg.startpoint,							// 開始位置
			seg.endpoint,							// 終了位置
			cylinder.radius * 2.0f,					// 幅
			sphere.radius,							// 半径
			p1center);								// 中心座標

		if (sts) {
			return true;
		}
	}

	return false;
}

// 球
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

	// X座標
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y座標
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z座標
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}


AABB SetAABB(Vector3 centerposition,float width ,float height,float depth) {

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

