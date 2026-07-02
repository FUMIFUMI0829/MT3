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

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};
	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f,  0.0f},
	};
	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};


	unsigned int jointColors[3] = { RED, GREEN, BLUE };

	Vector3 a{ 0.2f, 1.0f, 0.0f };
	Vector3 b{ 2.4f, 3.1f, 1.2f };
	Vector3 rotate{ 0.4f, 1.43f, -0.8f };

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

		ImGui::Separator();
		ImGui::DragFloat3("Shoulder Translate", &translates[0].x, 0.01f);
		ImGui::DragFloat3("Shoulder Rotate", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("Shoulder Scale", &scales[0].x, 0.01f);

		ImGui::Separator();
		ImGui::DragFloat3("Elbow Translate", &translates[1].x, 0.01f);
		ImGui::DragFloat3("Elbow Rotate", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("Elbow Scale", &scales[1].x, 0.01f);

		ImGui::Separator();
		ImGui::DragFloat3("Hand Translate", &translates[2].x, 0.01f);
		ImGui::DragFloat3("Hand Rotate", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("Hand Scale", &scales[2].x, 0.01f);

		ImGui::End();
#endif

		// 演算子オーバーロードの動作確認
		Vector3 c = a + b;
		Vector3 d = a - b;
		Vector3 e = a * 2.4f;
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateMatrix  = rotateXMatrix * rotateYMatrix * rotateZMatrix;

#ifdef USE_IMGUI
		ImGui::Begin("OperatorOverload");
		ImGui::Text("c:%f, %f, %f", c.x, c.y, c.z);
		ImGui::Text("d:%f, %f, %f", d.x, d.y, d.z);
		ImGui::Text("e:%f, %f, %f", e.x, e.y, e.z);
		ImGui::Text(
			"matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
			rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
			rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
		ImGui::End();
#endif

		Matrix4x4 worldMatrices[3];
		worldMatrices[0] = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		worldMatrices[1] = Multiply(MakeAffineMatrix(scales[1], rotates[1], translates[1]), worldMatrices[0]);
		worldMatrices[2] = Multiply(MakeAffineMatrix(scales[2], rotates[2], translates[2]), worldMatrices[1]);

		Vector3 worldPos[3];
		Sphere sphere[3];
		for (int i = 0; i < 3; ++i) {
			worldPos[i] = { worldMatrices[i].m[3][0], worldMatrices[i].m[3][1], worldMatrices[i].m[3][2] };
			sphere[i] = { worldPos[i], 0.05f };
		}

		Segment seg01{ worldPos[0], Subtract(worldPos[1], worldPos[0]) };
		Segment seg12{ worldPos[1], Subtract(worldPos[2], worldPos[1]) };
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		for (int i = 0; i < 3; ++i) {
			DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, jointColors[i]);
		}
		DrawSegment(seg01, viewProjectionMatrix, viewportMatrix, WHITE);
		DrawSegment(seg12, viewProjectionMatrix, viewportMatrix, WHITE);

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