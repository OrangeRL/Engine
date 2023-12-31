#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathFunc.h"
#include "Input.h"

class ReilCamera
{
public:
	void Initialize(Vector3 pos, Vector3 rot, WinApp* winApp);

	void Update(Input* input,float wave);

	ViewProjection GetViewProjection() { return viewProjection_; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

	Vector3 GetRotation() { return rotation; }

private:
	const float PI = 3.14159f;
	Input* input = nullptr;

	//ワールド行列
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//回転
	Vector3 rotation = { 0,0.5f,0 };
	//回転の速度
	Vector3 rotSpeeed = { 0.005f,0.005f,0.005f };
	
	const float MAX_FLAME = 100.0f;
	float flameLeft = 0;
	float flameRight = 0;
	float flameUp = 0;
	float flameDown = 0;

	float a = 0;
};

