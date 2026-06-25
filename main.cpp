#include <Novice.h>
#include "Vector2.h"
#include "MT3.h"
#include <imgui.h>
#include <cmath>
#include <algorithm>

const char kWindowTitle[] = "LE2B_11_コウサカ_タカフミ";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	/*Vector3 rotate{ 0.f,0.f,0.f };
	OBB obb{
		.center{-1.f,0.f,0.f},
		.orientations{{1.f,0.f,0.f},
					  {0.f,1.f,0.f},
					  {0.f,0.f,1.f}},
		.size{0.5f,0.5f,0.5f}
	};*/

	/*Sphere sphere{
		.center{0.f,0.f,0.f},
		.radius{0.5f}
	};*/

	/*Segment segment{
		.origin{-0.8f, -0.3f, 0.0f},
		.diff{0.5f, 0.5f, 0.5f}
	};

	Vector3 rotate1{ 0.0f, 0.0f, 0.0f };
	Vector3 rotate2{ -0.05f, -2.49f, 0.15f };
	OBB obb1{
		.center{0.0f, 0.0f, 0.0f},
		.orientations = {{1.0f, 0.0f, 0.0f},
						 {0.0f, 1.0f, 0.0f},
						 {0.0f, 0.0f, 1.0f}},
		.size{0.83f, 0.26f, 0.24f}
	};

	OBB obb2{
		.center{0.9f, 0.66f, 0.78f},
		.orientations = {{1.0f, 0.0f, 0.0f},
						 {0.0f, 1.0f, 0.0f},
						 {0.0f, 0.0f, 1.0f}},
		.size{0.5f, 0.37f, 0.5f}
	};*/

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	//Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateYMatrix(rotate.z)));

	////回転行列から軸を抽出
	//obb.orientations[0].x = rotateMatrix.m[0][0];
	//obb.orientations[0].y = rotateMatrix.m[0][1];
	//obb.orientations[0].z = rotateMatrix.m[0][2];

	//obb.orientations[1].x = rotateMatrix.m[1][0];
	//obb.orientations[1].y = rotateMatrix.m[1][1];
	//obb.orientations[1].z = rotateMatrix.m[1][2];

	//obb.orientations[2].x = rotateMatrix.m[2][0];
	//obb.orientations[2].y = rotateMatrix.m[2][1];
	//obb.orientations[2].z = rotateMatrix.m[2][2];

	Vector3 controlPoints[3] = {
		{ -0.8f, 0.58f, 1.0f },
		{ 1.76f, 1.0f, -0.3f },
		{ 0.94f, -0.7f, 2.3f },
	};

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);



#ifdef USE_IMGUI
		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.01f);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.01f);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.01f);

		ImGui::End();
#endif

		unsigned int objColor = WHITE;

		/*if (IsCollision(obb1, obb2)) {
			objColor = RED;
		}*/

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], viewProjectionMatrix, viewportMatrix, objColor);

		///
		/// ↑描画処理ここまで
		///

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();

	return 0;
}