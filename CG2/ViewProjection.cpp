#include "ViewProjection.h"

void ViewProjection::Initialize(WinApp* winApp) {
	//eye = { 0 , 0 , -50 };	//���_���W
	//target = { 0 , 0 , 0 };	//�����_���W
	eye = { 0 , 20 , -1000 };	//���_���W
	target = { 0 , 20 , 0 };	//�����_���W
	up = { 0 , 1 , 0 };		//������x�N�g��
	FOV(winApp);
	UpdateView();
}

void ViewProjection::UpdateView() {
	matView = MathFunc::Utility::CreatMatView(eye, target, up);
}

void ViewProjection::FOV(WinApp* winApp) {
	//�������e�ϊ��s��̌v�Z
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);
}