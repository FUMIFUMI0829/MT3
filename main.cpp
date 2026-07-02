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

	Spring spring{};
	spring.anchor             = { 0.0f, 0.0f, 0.0f };
	spring.naturalLength      = 1.0f;
	spring.stiffness          = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 1.2f, 0.0f, 0.0f };
	ball.mass     = 2.0f;
	ball.radius   = 0.05f;
	ball.color    = BLUE;

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
		ImGui::Begin("Spring");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate",    &cameraRotate.x,    0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("anchor",             &spring.anchor.x,             0.01f);
		ImGui::DragFloat ("naturalLength",      &spring.naturalLength,        0.01f);
		ImGui::DragFloat ("stiffness",          &spring.stiffness,            1.0f);
		ImGui::DragFloat ("dampingCoefficient", &spring.dampingCoefficient,   0.1f);
		ImGui::Separator();

		ImGui::DragFloat3("ball.position", &ball.position.x, 0.01f);
		ImGui::DragFloat ("ball.mass",     &ball.mass,        0.1f);
		ImGui::DragFloat ("ball.radius",   &ball.radius,      0.001f);

		if (ImGui::Button(isSimulating ? "Stop" : "Start")) {
			isSimulating = !isSimulating;
			if (isSimulating) {
				ball.velocity     = { 0.0f, 0.0f, 0.0f };
				ball.acceleration = { 0.0f, 0.0f, 0.0f };
			}
		}

		ImGui::End();
#endif

		if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
			isSimulating = !isSimulating;
			if (isSimulating) {
				ball.velocity     = { 0.0f, 0.0f, 0.0f };
				ball.acceleration = { 0.0f, 0.0f, 0.0f };
			}
		}

		if (isSimulating) {
			Vector3 diff   = ball.position - spring.anchor;
			float length   = Length(diff);
			if (length != 0.0f) {
				Vector3 direction    = Normalize(diff);
				Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
				Vector3 displacement = ball.position - restPosition;
				Vector3 restoringForce = -spring.stiffness * displacement;
				Vector3 dampingForce   = -spring.dampingCoefficient * ball.velocity;
				Vector3 force          = restoringForce + dampingForce;
				ball.acceleration      = force / ball.mass;
			}
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity     * deltaTime;
		}

		Segment seg{ spring.anchor, ball.position - spring.anchor };
		Sphere anchorSphere{ spring.anchor, 0.02f };
		Sphere ballSphere{ ball.position, ball.radius };

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// 線分
		DrawSegment(seg, viewProjectionMatrix, viewportMatrix, WHITE);

		// アンカー
		DrawSphere(anchorSphere, viewProjectionMatrix, viewportMatrix, RED);

		// ボール
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
