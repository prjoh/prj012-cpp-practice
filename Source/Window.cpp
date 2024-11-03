#include <Window.h>
#include <Core/Logger.h>

#include <ThirdParty/SDL2/include/SDL.h>
#include <ThirdParty/SDL2/include/SDL_syswm.h>


static void sdl_log_callback(void* user_data, int category, SDL_LogPriority priority, const char* message)
{
  // Convert priority to string
  const char* priority_str = nullptr;
  switch (priority) {
    case SDL_LOG_PRIORITY_VERBOSE: priority_str = "VERBOSE"; break;
    case SDL_LOG_PRIORITY_DEBUG:   priority_str = "DEBUG";   break;
    case SDL_LOG_PRIORITY_INFO:    priority_str = "INFO";    break;
    case SDL_LOG_PRIORITY_WARN:    priority_str = "WARN";    break;
    case SDL_LOG_PRIORITY_ERROR:   priority_str = "ERROR";   break;
    case SDL_LOG_PRIORITY_CRITICAL:priority_str = "CRITICAL";break;
    default: priority_str = "UNKNOWN"; break;
  }

  ZV_LOG("EXTERN", "**SDL** [{}] {}", priority_str, message);
}

bool zv::Window::create(const CreateParams &params)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    return 1;
  }

  SDL_LogSetOutputFunction(sdl_log_callback, this);

  u32 flags = params.enable_fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;

  m_ptr_sdl_window = SDL_CreateWindow(
    params.title.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    params.width,
    params.height,
    flags
  );

  if (m_ptr_sdl_window == nullptr)
  {
    ZV_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());

    SDL_Quit();

    return false;
  }

  return true;
}

void zv::Window::destroy()
{
  SDL_DestroyWindow(m_ptr_sdl_window);
  SDL_Quit();
}

zv::NativeWindowHandle zv::Window::get_native_window_handle() const
{
  SDL_SysWMinfo wm_info;
  SDL_VERSION(&wm_info.version);
  SDL_GetWindowWMInfo(m_ptr_sdl_window, &wm_info);
  NativeWindowHandle handle;
#if OS_WINDOWS
  return wm_info.info.win.window;
#elif OS_MAC
  return wm_info.info.cocoa.window;
#else
# error "Other OS currently not supported."
#endif
}
