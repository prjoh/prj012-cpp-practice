#include <Application.h>

#include <Config.h>
#include <Renderer.h>
#include <Window.h>
#include <Stats.h>
#include <Core/Logger.h>
#include <Core/Time.h>

#include <ThirdParty/SDL2/include/SDL.h>
#include <ThirdParty/sdl-imgui/imgui_impl_sdl.h>


zv::Application::Application()
  : m_ptr_window(std::make_unique<Window>())
  , m_ptr_renderer(std::make_unique<Renderer>())
  , m_ptr_stats(std::make_unique<Stats>())
  , m_event()
{
}

zv::Application::~Application()
{
}

s32 zv::Application::run()
{
  Time::Clock::create();
  if (!Logger::create(get_base_path()))
  {
    return 1;
  }

  Window::CreateParams window_params;
  window_params.title = PROJECT_TITLE;
  window_params.width = 1200;
  window_params.height = 800;
  // window_params.enable_fullscreen = true;

  if (!m_ptr_window->create(window_params))
  {
    ZV_ERROR("Failed to create window for '{}'.", PROJECT_TITLE);
    return 1;
  }

  Renderer::CreateParams renderer_params;
  renderer_params.ptr_window = m_ptr_window.get();
  renderer_params.device_type = eRenderDeviceType::RENDER_DEVICE_TYPE_D3D12;
  // renderer_params.enable_fullscreen = true;
  renderer_params.enable_vsynch = false;
  renderer_params.init_imgui = true;
  renderer_params.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

  if (!m_ptr_renderer->create(renderer_params))
  {
    ZV_ERROR("Failed to create renderer for '{}'.", PROJECT_TITLE);
    return 1;
  }

  m_ptr_renderer->register_imgui_renderable(m_ptr_stats.get());

  while (!m_quit)
  {
    while (SDL_PollEvent(&m_event) != 0)
    {
      ImGui_ImplSDL2_ProcessEvent(&m_event);
 
      if (m_event.type == SDL_QUIT)
      {
        m_quit = true;
      }

      if (m_event.type == SDL_KEYDOWN)
      {
        if (m_event.key.keysym.sym == SDLK_ESCAPE)
        {
          m_quit = true;
        }
      }
    }

    Time::Clock::tick();

    m_ptr_stats->update();

    m_ptr_renderer->update();
  }

  m_ptr_renderer->destroy();
  m_ptr_window->destroy();

  Logger::destroy();
  Time::Clock::destroy();

  return 0;
}
