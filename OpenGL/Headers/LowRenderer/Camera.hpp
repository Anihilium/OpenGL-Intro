#pragma once
#include <Maths/Vec3.hpp>
#include <Maths/Mat4.hpp>
using namespace Core::Maths;

namespace LowRenderer
{
	struct CameraInputs
	{
		float deltaX = 0.f;
		float deltaY = 0.f;
		bool moveForward = true;
		bool moveBackward = true;
		bool moveRight = true;
		bool moveLeft = true;
		bool moveUp = true;
		bool moveDown = true;
	};

	class Camera
	{
	public:
		float Aspect = 1.f;

		CameraInputs CamInputs;
		Mat4 ViewProjectionMatrix;
		Vec3 Position;
		Vec3 Front;

	private:
		float mFar = 100.f;
		float mNear = 0.1f;
		float mFOV = 90.f;
		float mSpeed = 5.f;
		float mSensitivity = 0.2f;
		float mPitch;
		float mYaw;

		Vec3 mUp;
		Vec3 mRight;
		Vec3 mWorldUp;

	public:
		Camera(const Vec3& position = Vec3(0.f, 0.f, 0.f), const Vec3& up = Vec3(0.f, 1.f, 0.f), const float yaw = -90.f, const float pitch = 0.f, const int height = 1, const int width = 1);
		void Update(float deltaTime);
	
	private:
		void UpdateVectors();
		Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
		Mat4 GetViewMatrix();
		Mat4 GetProjectionMatrix();
	};
}