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

	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor          = { 0.0f, 1.0f, 0.0f };
	conicalPendulum.length          = 0.8f;
	conicalPendulum.halfApexAngle   = 0.7f;
	conicalPendulum.angle           = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;

	Ball ball{};
	ball.radius = 0.05f;
	ball.color  = BLUE;

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
		ImGui::Begin("ConicalPendulum");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate",    &cameraRotate.x,    0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("anchor",        &conicalPendulum.anchor.x,        0.01f);
		ImGui::DragFloat ("length",        &conicalPendulum.length,           0.01f);
		ImGui::DragFloat ("halfApexAngle", &conicalPendulum.halfApexAngle,    0.01f);
		ImGui::Text("angle: %.3f", conicalPendulum.angle);
		ImGui::Text("angularVelocity: %.3f", conicalPendulum.angularVelocity);

		if (ImGui::Button(isSimulating ? "Stop" : "Start")) {
			isSimulating = !isSimulating;
			if (isSimulating) {
				conicalPendulum.angle = 0.0f;
			}
		}

		ImGui::End();
#endif

		if (isSimulating) {
			conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle          += conicalPendulum.angularVelocity * deltaTime;
		}

		float circleRadius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		float dropHeight   = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		ball.position.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * circleRadius;
		ball.position.y = conicalPendulum.anchor.y - dropHeight;
		ball.position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * circleRadius;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		Segment pendulumRod{ conicalPendulum.anchor, ball.position - conicalPendulum.anchor };
		DrawSegment(pendulumRod, viewProjectionMatrix, viewportMatrix, WHITE);

		Sphere anchorSphere{ conicalPendulum.anchor, 0.02f };

		Sphere bobSphere{ ball.position, ball.radius };
		DrawSphere(bobSphere, viewProjectionMatrix, viewportMatrix, ball.color);

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
