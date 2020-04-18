#pragma once

#include "mtpch.h"

#include "Mortify/Core/Core.h"
#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/Input.h"
#include "Mortify/Rendering/RenderContext.h"
#include "Mortify/Core/WindowManager.h"
#include "Mortify/Core/WindowCore.h"

namespace Mortify
{
	// Window Interface for desktop system windows
	class Window
	{
	public:

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual void Shutdown() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;
		virtual WindowID GetID() const = 0;
		virtual const WindowLimits& GetLimits() const = 0;

		virtual bool IsResizeable() const = 0;
		virtual bool KeepsAspectRatio() const = 0;
		virtual bool IsBorderless() const = 0;
		virtual bool IsAlwaysOnTop() const = 0;
		virtual bool AutoIconifys() const = 0;
		virtual bool ScalesToMonitor() const = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsFullscreen() const = 0;
		virtual bool VSyncEnabled() const = 0;
		virtual bool IsKeyPressed(KeyCode code) const = 0;
		virtual bool IsMouseButtonPressed(MouseCode button) const = 0;


		virtual WindowMode GetWindowMode() const = 0;
		virtual void SetWindowMode(WindowMode mode) = 0;
		
		virtual void* GetNativeWindow() const = 0;

		virtual const Ref<RenderContext>& GetContext() const = 0;
		virtual void SetContext(const Ref<RenderContext>& context) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetFlags(uint8_t flags, bool state) = 0;
		virtual void Close() = 0;
		virtual void LimitWindowSize(WindowLimits limits = WindowLimits()) = 0;
		
		static Ref<Window> Create(const WindowConfig& config = WindowConfig(), const EventCallbackFn& callback = nullptr)
		{
			return WindowManager::AddWindow(config, callback);
		}
	};
}