#include "Spline.h"
#include <windows.h>
#include <sysinfoapi.h>

void Spline::Initialize() {
	startCount = GetTickCount64();
}

Vector3 Spline::Update(std::vector<Vector3>& points, float timeRate) {

	//経過時間の計算
	nowCount = GetTickCount64();
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

	timeRate = elapsedTime / maxTime;

	//timeRateが1.0f以上になったら、次の区間に進む
	if (timeRate >= 1.0f) {
		if (startIndex < points.size() - 3) {

			startIndex++;

			timeRate -= 1.0f;
			startCount = GetTickCount64();
		}
		else {
			timeRate = 1.0f;
		}
	}

	Vector3 pos = SplinePosition(points, startIndex, timeRate);

	return pos;
}


void Spline::Draw() {

}

Vector3 Spline::SplinePosition(std::vector<Vector3>& points, size_t startIndex, float t) {
	//補間すべき点
	size_t n = points.size() - 2;

	if (startIndex > n) return points[n];
	if (startIndex < 1) return points[1];

	//p0~p3の制御点を取得する
	Vector3 p0_ = points[startIndex - 1];
	Vector3 p1_ = points[startIndex];
	Vector3 p2_ = points[startIndex + 1];
	Vector3 p3_ = points[startIndex + 2];

	//Catmull-Rom の式による補間
	Vector3 position;

	position.x = 0.5f * (p1_.x * 2.0f + (-p0_.x + p2_.x) *
		t + (p0_.x * 2 - p1_.x * 5 + p2_.x * 4 - p3_.x) *
		(t * t) + (-p0_.x + p1_.x * 3 - p2_.x * 3 + p3_.x) *
		(t * t * t));

	position.y = 0.5f * (p1_.y * 2.0f + (-p0_.y + p2_.y) *
		t + (p0_.y * 2 - p1_.y * 5 + p2_.y * 4 - p3_.y) *
		(t * t) + (-p0_.y + p1_.y * 3 - p2_.y * 3 + p3_.y) *
		(t * t * t));

	position.z = 0.5f * (p1_.z * 2.0f + (-p0_.z + p2_.z) *
		t + (p0_.z * 2 - p1_.z * 5 + p2_.z * 4 - p3_.z) *
		(t * t) + (-p0_.z + p1_.z * 3 - p2_.z * 3 + p3_.z) *
		(t * t * t));

	return position;
}