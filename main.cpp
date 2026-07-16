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

	Plane plane;
	plane.normal   = Normalize({ -0.2f, 0.9f, -0.3f });
	plane.distance = 0.0f;

	Ball ball{};
	ball.position = { 0.8f, 1.2f, 0.3f };
	ball.mass     = 2.0f;
	ball.radius   = 0.05f;
	ball.color    = WHITE;

	float restitution = 0.8f; // 反発係数

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
		ImGui::Begin("BallOnPlane");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate",    &cameraRotate.x,    0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("plane.normal",   &plane.normal.x,   0.01f);
		ImGui::DragFloat ("plane.distance", &plane.distance,   0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("ball.position", &ball.position.x, 0.01f);
		ImGui::DragFloat ("restitution",   &restitution,     0.01f, 0.0f, 1.0f);

		if (ImGui::Button(isSimulating ? "Stop" : "Start")) {
			isSimulating = !isSimulating;
			if (isSimulating) {
				ball.velocity     = { 0.0f, 0.0f, 0.0f };
				ball.acceleration = { 0.0f, 0.0f, 0.0f };
			}
		}

		ImGui::End();
#endif

		if (isSimulating) {
			ball.acceleration = { 0.0f, -9.8f, 0.0f };
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity     * deltaTime;

			if (IsCollision(Sphere{ ball.position, ball.radius }, plane)) {
				Vector3 reflectedVelocity  = Reflect(ball.velocity, plane.normal);
				Vector3 normalComponent    = Project(reflectedVelocity, plane.normal);
				Vector3 tangentComponent   = reflectedVelocity - normalComponent;
				ball.velocity              = normalComponent * restitution + tangentComponent;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);

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
