#include "ViewProjection.h"

void ViewProjection::Initialize(WinApp* winApp) {
	//eye = { 0 , 0 , -50 };	//視点座標
	//target = { 0 , 0 , 0 };	//注視点座標
	eye = { 0 , 0 , -50 };	//視点座標
	target = { 0 , 0 , 0 };	//注視点座標
	up = { 0 , 1 , 0 };		//上方向ベクトル
	FOV(winApp);
	UpdateView();
}

void ViewProjection::UpdateView() {
	matView = MathFunc::Utility::CreatMatView(eye, target, up);
}

void ViewProjection::FOV(WinApp* winApp) {
	//透視投影変換行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);
}