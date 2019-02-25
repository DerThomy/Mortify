#include "mtpch.h"

#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Mortify/Application.h"
#include "Mortify/KeyCodes.h"

// Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Mortify
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temporary. To be replaced by Mortify Key Codes
		io.KeyMap[ImGuiKey_Tab] = MT_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = MT_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = MT_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = MT_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = MT_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = MT_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = MT_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = MT_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = MT_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = MT_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = MT_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = MT_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = MT_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = MT_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = MT_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = MT_KEY_A;
		io.KeyMap[ImGuiKey_C] = MT_KEY_C;
		io.KeyMap[ImGuiKey_V] = MT_KEY_V;
		io.KeyMap[ImGuiKey_X] = MT_KEY_X;
		io.KeyMap[ImGuiKey_Y] = MT_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = MT_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410 core");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonClickedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonClickedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonClickedEvent(MouseButtonClickedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[MT_KEY_LEFT_CONTROL] || io.KeysDown[MT_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[MT_KEY_LEFT_SHIFT] || io.KeysDown[MT_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[MT_KEY_LEFT_ALT] || io.KeysDown[MT_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[MT_KEY_LEFT_SUPER] || io.KeysDown[MT_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.getKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}