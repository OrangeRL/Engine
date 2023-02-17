#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "MathFunc.h"

class ViewProjection
{
public:
	Matrix4 matView;
	Vector3 eye;	//���_���W
	Vector3 target={0,0,0};	//�����_���W
	Vector3 up;		//������x�N�g��

public:
	void Initialize();
	void UpdateView();
};

