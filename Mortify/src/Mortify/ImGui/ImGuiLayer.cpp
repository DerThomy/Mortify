#include "mtpch.h"

#include "Mortify/ImGui/ImGuiLayer.h"
#include "Mortify/Rendering/RendererAPI.h"

#include <imgui.h>

#ifdef MT_PLATFORM_WINDOWS
    #include <examples/imgui_impl_opengl3.h>
    #include <examples/imgui_impl_win32.h>
#elif defined(MT_PLATFORM_LINUX)
    #include <examples/imgui_impl_opengl3.h>
    #include <examples/imgui_impl_glfw.h>
#endif

#include "Mortify/Core/Application.h"

// Temporary
#include <GLFW/glfw3.h>

namespace Mortify
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	void ImGuiLayer::OnAttach()
	{
		MT_PROFILE_FUNCTION();
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();

        #ifdef MT_PLATFORM_WINDOWS
            ImGui_ImplWin32_Init(app.GetWindow()->GetNativeWindow(), app.GetWindow()->GetContext()->GetContextHandler());
        #elif defined(MT_PLATFORM_LINUX)
            ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow()), true);
        #endif	
        
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::OpenGL: ImGui_ImplOpenGL3_Init("#version 410"); break;
        }
	}

	void ImGuiLayer::OnDetach()
	{
		MT_PROFILE_FUNCTION();
        
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::OpenGL: ImGui_ImplOpenGL3_Shutdown(); break;
        }
        
        #ifdef MT_PLATFORM_WINDOWS
            ImGui_ImplWin32_Shutdown();
        #elif defined(MT_PLATFORM_LINUX)
            
        #endif	
    
		ImGui::DestroyContext();
	}
	
	void ImGuiLayer::Begin()
	{
		MT_PROFILE_FUNCTION();
        
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::OpenGL: ImGui_ImplOpenGL3_NewFrame(); break;
        }
		
        #ifdef MT_PLATFORM_WINDOWS
            ImGui_ImplWin32_NewFrame();
        #elif defined(MT_PLATFORM_LINUX)
            glfwPollEvents();
            ImGui_ImplGlfw_NewFrame();
        #endif	
        
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		MT_PROFILE_FUNCTION();
		
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

		// Render
		ImGui::Render();
        
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::OpenGL: ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); break;
        }
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			app.GetWindow()->GetContext()->MakeContextCurrent();
		}
	}
}