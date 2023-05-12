#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "MathFunc.h"
#include "WinApp.h"
class ViewProjection
{
public:
	Matrix4 matView;
	Vector3 eye;	//���_���W
	Vector3 target={0,0,0};	//�����_���W
	Vector3 up;		//������x�N�g��
	XMMATRIX matProjection = {};
	WinApp* winApp = nullptr;
public:
	void Initialize(WinApp* winApp);
	void UpdateView();
	void FOV(WinApp* winApp);
};

