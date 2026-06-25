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