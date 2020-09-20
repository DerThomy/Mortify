#pragma once

#include "Mortify/Core/Window.h"
#include "Mortify/Rendering/RenderContext.h"
#include "LinuxOS.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mortify
{
	class LinuxWindow : public Window
	{
	private:
		friend class LinuxGLRenderContext;

	public:
		LinuxWindow(WindowID id, const WindowConfig& config, const EventCallbackFn& callback = nullptr);
		virtual ~LinuxWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Props.Width; }
		inline unsigned int GetHeight() const override { return m_Props.Height; }
		inline float GetAspectRatio() const override { return static_cast<float>(m_Props.Width) / static_cast<float>(m_Props.Height); }
		inline WindowID GetID() const override { return m_Props.ID; }
		inline const WindowLimits& GetLimits() const { return m_Limits; }
        inline const std::string& GetTitle() const override { return m_Props.Title; }

		inline bool IsResizeable() const { return m_Props.Resizeable; }
		inline bool KeepsAspectRatio() const { return m_Props.KeepAspect; }
		inline bool IsBorderless() const { return m_Props.Borderless; }
		inline bool IsAlwaysOnTop() const { return m_Props.AlwaysOnTop; }
		inline bool AutoIconifys() const { return m_Props.AutoIconify; }
		inline bool ScalesToMonitor() const { return m_Props.ScaleToMonitor; }
		inline bool IsMaximized() const override { return glfwGetWindowAttrib(m_WindowHandle, GLFW_MAXIMIZED); }
		inline bool IsMinimized() const override { return glfwGetWindowAttrib(m_WindowHandle, GLFW_ICONIFIED); }
		inline bool IsFullscreen() const override { return m_Props.Fullscreen; }
		inline bool VSyncEnabled() const override { return m_Props.VSync; }
		bool IsKeyPressed(KeyCode code) const override;
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_MouseButtons.at(button); };

		inline WindowMode GetWindowMode() const override;
		void SetWindowMode(WindowMode mode) override;
		
		inline const Ref<RenderContext>& GetContext() const override { return m_RenderContext; }
		inline void SetContext(const Ref<RenderContext>& context) override { m_RenderContext = context; };
		
		inline void* GetNativeWindow() const override { return static_cast<void*>(m_WindowHandle); }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Props.EventCallback = callback; }
		void SetVSync(bool enabled) override;

		void SetFlags(uint8_t flags, bool state) override;
		void Close() override;
		inline void LimitWindowSize(WindowLimits limits) override { m_Props.Limits = limits; };

	private:
		void Init();
		void Shutdown() override;
        void ClipSize();

	private:

		static LinuxWindow* s_MainWindow;

	private:
		Ref<RenderContext> m_RenderContext;
		Ref<LinuxOS> m_OS;
        GLFWwindow* m_WindowHandle;

		WindowProps m_Props;
		WindowLimits m_Limits;

		struct SavedInfo
		{
			int XPos, YPos;
			int Width, Height;
		};
		SavedInfo m_SavedInfo{};
		
		std::unordered_map<KeyCode, bool> m_Keys;
		std::unordered_map<MouseCode, bool> m_MouseButtons;
	};
}