#include <Novice.h>
#include "Vector2.h"
#include "MT3.h"
#include <imgui.h>
#include <cmath>
#include <algorithm>
#include <numbers>

const char kWindowTitle[] = "LE2B_11_コウサカ_タカフミ";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	const float deltaTime = 1.0f / 60.0f;
	float angularVelocity = 3.14f;
	float radius = 0.8f;
	Vector3 center = { 0.0f, 0.0f, 0.0f };
	float angle = 0.0f;
	bool isSimulating = false;

	Ball ball{};
	ball.position = { center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius, center.z };
	ball.radius = 0.05f;
	ball.color = BLUE;

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
		ImGui::Begin("CircularMotion");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat("angularVelocity", &angularVelocity, 0.01f);
		ImGui::DragFloat("radius", &radius, 0.01f);
		ImGui::DragFloat3("center", &center.x, 0.01f);
		ImGui::Text("angle: %.3f", angle);

		if (ImGui::Button(isSimulating ? "Stop" : "Start")) {
			isSimulating = !isSimulating;
		}

		ImGui::End();
#endif

		if (isSimulating) {
			angle += angularVelocity * deltaTime;

			ball.position.x = center.x + std::cos(angle) * radius;
			ball.position.y = center.y + std::sin(angle) * radius;
			ball.position.z = center.z;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		Sphere ballSphere{ ball.position, ball.radius };
		DrawSphere(ballSphere, viewProjectionMatrix, viewportMatrix, ball.color);

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
