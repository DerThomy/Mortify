#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>

#include "Mortify/Events/Event.h"
#include "Mortify/Window.h"

namespace Mortify {

	class PerspectiveCamera
	{
	public:


	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Pitch;
		float m_Yaw;
		float m_Roll;
	};

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }

		inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation) { m_Rotation = rotation; UpdateViewMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void UpdateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

	/*
	class Camera
	{
	public:
		enum CameraType {
			ORTHO,
			FREE
		};
		enum CameraDirection {
			UP,
			DOWN,
			FORWARD,
			BACK,
			LEFT,
			RIGHT
		};

	public:
		Camera();

		~Camera();

		void Reset();

		void Update();

		void ChangePitch(float degrees);

		void ChangeHeading(float degrees);

		void SetMode(CameraType cam_mode);

		void SetPosition(glm::vec3 pos);

		void SetLookAt(glm::vec3 pos);

		void SetFOV(double fov);

		void SetViewport(int loc_x, int loc_y, int width, int height);

		void SetClipping(double near_clip_distance, double far_clip_distance);

		void OnEvent(Event& e);

		inline CameraType GetMode() const { return m_CameraMode; }
		inline int GetViewportX() const { return m_ViewportX; }
		inline int GetViewportY() const { return m_ViewportY; }
		inline glm::mat4 GetVP() const { return m_VP; }

	private:
		void Move(CameraDirection dir);
		void Move2D(int x, int y);

	private:
		CameraType m_CameraMode;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraPositionDelta;
		glm::vec3 m_CameraLookAt;
		glm::vec3 m_CameraDirection;

		glm::vec3 m_CameraUp;
		glm::vec3 m_MousePosition;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_VP;

		int m_ViewportX;
		int m_ViewportY;

		int m_WindowWidth;
		int m_WindowHeight;

		double m_Aspect;
		double m_FOV;
		double m_NearClip;
		double m_FarClip;

		float m_CameraScale;
		float m_CameraHeading;
		float m_CameraPitch;

		float m_MaxPitchRate;
		float m_MaxHeadingRate;
		bool m_MoveCamera;
	};
	*/
}