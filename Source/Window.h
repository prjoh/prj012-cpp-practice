#pragma once

#include <string>

#include <Core/PrimitiveTypes.h>
#include <Core/PlatformContext.h>

#if OS_WINDOWS
#include <windows.h>
#elif OS_MAC
// TODO : ???
#endif

struct SDL_Window;

namespace zv
{
#if OS_WINDOWS
  typedef HWND NativeWindowHandle;
#elif OS_MAC
  typedef NSWindow NativeWindowHandle;
#else
# error "Other OS currently not supported."
#endif

  class Window
  {
    friend class Renderer;

  public:
    struct CreateParams {
      std::string title;
      s32 width;
      s32 height;
      bool enable_fullscreen{ false };
    };
    bool create(const CreateParams& params);
    void destroy();

  private:
    NativeWindowHandle get_native_window_handle() const;

  private:
    SDL_Window* m_ptr_sdl_window{ nullptr };
  };
}
