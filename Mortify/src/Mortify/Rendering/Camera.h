#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mortify/Events/Event.h"

namespace Mortify {
	// Default camera values
	static const float YAW = -90.0f;
	static const float PITCH = 0.0f;
	static const float SPEED = 2.5f;
	static const float SENSITIVITY = 0.1f;
	static const float ZOOM = 45.0f;

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
}