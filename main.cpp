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

	Pendulum pendulum;
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	const float deltaTime = 1.0f / 60.0f;
	bool isSimulating = false;

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
		ImGui::Begin("Pendulum");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("anchor", &pendulum.anchor.x, 0.01f);
		ImGui::DragFloat("length", &pendulum.length, 0.01f);
		ImGui::DragFloat("angle", &pendulum.angle, 0.01f);
		ImGui::Text("angularVelocity: %.3f", pendulum.angularVelocity);

		if (ImGui::Button(isSimulating ? "Stop" : "Start")) {
			isSimulating = !isSimulating;
			if (isSimulating) {
				pendulum.angularVelocity = 0.0f;
				pendulum.angularAcceleration = 0.0f;
			}
		}

		ImGui::End();
#endif

		if (isSimulating) {
			pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;
		}

		// 振り子の先端位置
		Vector3 tipPosition;
		tipPosition.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		tipPosition.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
		tipPosition.z = pendulum.anchor.z;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// アンカーから先端への線分
		Segment pendulumRod{ pendulum.anchor, tipPosition - pendulum.anchor };
		DrawSegment(pendulumRod, viewProjectionMatrix, viewportMatrix, WHITE);

		// アンカー点
		Sphere anchorSphere{ pendulum.anchor, 0.02f };
		DrawSphere(anchorSphere, viewProjectionMatrix, viewportMatrix, WHITE);

		// 振り子の球
		Sphere pendulumBall{ tipPosition, 0.05f };
		DrawSphere(pendulumBall, viewProjectionMatrix, viewportMatrix, WHITE);

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
