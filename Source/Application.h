#pragma once

#include <memory>

#include <Core/PrimitiveTypes.h>

#include <ThirdParty/SDL2/include/SDL.h>


namespace zv
{
  class Window;
  class Renderer;
  class Stats;
}

namespace zv
{
  class Application
  {
  public:
    Application();
    ~Application();

  public:
    s32 run();

    static const char* get_base_path() { return SDL_GetBasePath(); }

  private:
    std::unique_ptr<Window> m_ptr_window{ nullptr };
    std::unique_ptr<Renderer> m_ptr_renderer{ nullptr };
    std::unique_ptr<Stats> m_ptr_stats{ nullptr };

    bool m_quit{ false };
    SDL_Event m_event;
  };
}
