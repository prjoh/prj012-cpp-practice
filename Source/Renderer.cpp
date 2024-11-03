#include <Core/PlatformContext.h>

#include <Renderer.h>
#include <Core/Logger.h>

#include <ThirdParty/DiligentCore/Primitives/interface/DebugOutput.h>

#include <ThirdParty/DiligentCore/Platforms/Win32/interface/Win32NativeWindow.h>
#include <ThirdParty/DiligentCore/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <ThirdParty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>

#include <ThirdParty/DiligentCore/Graphics/GraphicsAccessories/interface/ColorConversion.h>

#include <ThirdParty/DiligentTools/Imgui/interface/ImGuiImplDiligent.hpp>
#include <ThirdParty/DiligentTools/Imgui/interface/ImGuiDiligentRenderer.hpp>
#include <ThirdParty/sdl-imgui/imgui_impl_sdl.h>


static void diligent_log_callback(Diligent::DEBUG_MESSAGE_SEVERITY severity, const char* message, const char* function, const char* file, int line)
{
  // Convert priority to string
  const char* priority_str = nullptr;
  switch (severity) {
    case Diligent::DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_INFO:        priority_str = "INFO";  break;
    case Diligent::DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_WARNING:     priority_str = "WARN";  break;
    case Diligent::DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_ERROR:       priority_str = "ERROR"; break;
    case Diligent::DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR: priority_str = "FATAL"; break;
    default: priority_str = "UNKNOWN"; break;
  }

  ZV_LOG("EXTERN", "**Diligent** [{}] {}", priority_str, message);
}

zv::Renderer::Renderer()
{
  m_imgui_renderables.reserve(1);
}

zv::Renderer::~Renderer()
{
  if (m_ptr_engine_factory)
  {
    m_ptr_engine_factory->SetMessageCallback(nullptr);
  }
}

bool zv::Renderer::create(const CreateParams& params)
{
  using namespace Diligent;

#if OS_WINDOWS
  Win32NativeWindow window{params.ptr_window->get_native_window_handle()};
#else
# error "Other platforms currently not supported."
#endif

  SwapChainDesc swap_chain_desc;

  FullScreenModeDesc fsm_desc;
  fsm_desc.Fullscreen = params.enable_fullscreen;
  fsm_desc.RefreshRateNumerator = params.enable_vsynch ? 60 : 0;
  fsm_desc.RefreshRateDenominator = 1;

  switch (params.device_type)
  {
    case eRenderDeviceType::RENDER_DEVICE_TYPE_D3D11:
    {
#if ENGINE_DLL
      // Load the dll and import GetEngineFactoryD3D11() function
      auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#endif
      auto* ptr_factory_d3d11 = GetEngineFactoryD3D11();
      m_ptr_engine_factory = ptr_factory_d3d11;

      m_ptr_engine_factory->SetMessageCallback(diligent_log_callback);

      EngineD3D11CreateInfo engine_ci;
      ptr_factory_d3d11->CreateDeviceAndContextsD3D11(engine_ci, &m_ptr_device, &m_ptr_immediate_context);
      ptr_factory_d3d11->CreateSwapChainD3D11(m_ptr_device, m_ptr_immediate_context, swap_chain_desc, fsm_desc, window, &m_ptr_swap_chain);

      break;
    }
    case eRenderDeviceType::RENDER_DEVICE_TYPE_D3D12:
    {
#if ENGINE_DLL
      // Load the dll and import GetEngineFactoryD3D12() function
      auto *GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#endif
      auto *ptr_factory_d3d12 = GetEngineFactoryD3D12();
      m_ptr_engine_factory = ptr_factory_d3d12;

      m_ptr_engine_factory->SetMessageCallback(diligent_log_callback);

      EngineD3D12CreateInfo engine_ci;
      ptr_factory_d3d12->CreateDeviceAndContextsD3D12(engine_ci, &m_ptr_device, &m_ptr_immediate_context);
      ptr_factory_d3d12->CreateSwapChainD3D12(m_ptr_device, m_ptr_immediate_context, swap_chain_desc, fsm_desc, window, &m_ptr_swap_chain);

      break;
    }
    default:
    {
      ZV_ERROR("Unknown / unsupported device type.");
      break;
    }
  }

  if (m_ptr_device == nullptr || m_ptr_immediate_context == nullptr || m_ptr_swap_chain == nullptr)
  {
    return false;
  }

  m_convert_ps_output_to_gamma = (swap_chain_desc.ColorBufferFormat == eTextureFormat::TEX_FORMAT_RGBA8_UNORM ||
                                  swap_chain_desc.ColorBufferFormat == eTextureFormat::TEX_FORMAT_BGRA8_UNORM);

  m_wireframe_supported = m_ptr_device->GetDeviceInfo().Features.WireframeFill;

  // create_pipeline_state();

  // // Load textured cube
  // m_cube_vertex_buffer = CreateVertexBuffer(m_ptr_device, VERTEX_COMPONENT_FLAG_POS_UV);
  // m_cube_index_buffer  = CreateIndexBuffer(m_ptr_device);
  // m_texture_srv       = LoadTexture(m_ptr_device, "Assets/Textures/grid.png")->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
  // // Set cube texture SRV in the SRB
  // m_ptr_srb->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(m_texture_srv);

  // create_instance_buffer();

  m_vsynch_enabled = params.enable_vsynch;
  m_clear_color = params.clear_color;
  m_imgui_available = params.init_imgui;

  if (m_imgui_available)
  {
    m_imgui_show = true;

    if (!init_imgui(swap_chain_desc, params.ptr_window))
    {
      ZV_ERROR("Failed to initialize imgui.");
      return false;
    }
  }

  return true;
}

bool zv::Renderer::init_imgui(const SwapChainDesc& swap_chain_desc, const Window* ptr_window)
{
  ImGui::CreateContext();
  ImGuiIO& io    = ImGui::GetIO();
  io.IniFilename = nullptr;

  // TODO
  // ImGui::StyleColorsDark();

  if (!ImGui_ImplSDL2_InitForD3D(ptr_window->m_ptr_sdl_window))
  {
    return false;
  }

  m_ptr_imgui_renderer = std::make_unique<ImGuiDiligentRenderer>(ImGuiDiligentCreateInfo{m_ptr_device, swap_chain_desc});

  return true;
}

void zv::Renderer::destroy()
{
  if (m_ptr_immediate_context)
  {
    m_ptr_immediate_context->Flush();
  }

  m_ptr_device = nullptr;
  m_ptr_immediate_context = nullptr;
  m_ptr_swap_chain = nullptr;
  m_ptr_imgui_renderer = nullptr;

  if (m_imgui_available)
  {
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();
  }
}

void zv::Renderer::register_imgui_renderable(IImGuiRenderable *ptr_imgui_renderable)
{
  m_imgui_renderables.emplace_back(ptr_imgui_renderable);
}

void zv::Renderer::update()
{
  using namespace Diligent;

  if (m_ptr_imgui_renderer)
  {
    const auto& sc_desc = m_ptr_swap_chain->GetDesc();

    ImGui_ImplSDL2_NewFrame();
  	m_ptr_imgui_renderer->NewFrame(sc_desc.Width, sc_desc.Height, sc_desc.PreTransform);
  	ImGui::NewFrame();

    for (IImGuiRenderable* ptr_renderable : m_imgui_renderables)
    {
      ptr_renderable->imgui_update();
    }

    // Update imgui
    //   ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    //   if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    //   {
    //     if (ImGui::SliderInt("Grid Size", &m_grid_size, 1, 32))
    //     {
    //       // populate_instance_buffer();
    //     }
    //   }
    //   ImGui::End();
  }

  // // Set cube view matrix
  // Matrix44 View = Matrix44::RotationX(-0.6f) * Matrix44::Translation(0.f, 0.f, 4.0f);

  // // Get pretransform matrix that rotates the scene according the surface orientation
  // auto SrfPreTransform = get_surface_pretransform_matrix(float3{0, 0, 1});

  // // Get projection matrix adjusted to the current screen orientation
  // auto Proj = get_adjusted_projection_matrix(PI_F / 4.0f, 0.1f, 100.f);

  // // Compute view-projection matrix
  // m_view_proj_matrix = View * SrfPreTransform * Proj;

  // // Global rotation matrix
  // m_rotation_matrix = Matrix44::RotationY(Time::get().elapsed_time_s() * 1.0f) * Matrix44::RotationX(-Time::get().elapsed_time_s() * 0.25f);

  // Apply rotation
  // Matrix44 CubeModelTransform = Matrix44::Identity();
  Matrix44 CubeModelTransform = Matrix44::RotationY(/*Time::get().elapsed_time_s() * */1.0f) * Matrix44::RotationX(-PI_F * 0.1f);

  // Camera is at (0, 0, -5) looking along the Z axis
  Matrix44 View = Matrix44::Translation(0.f, -1.0f, 5.0f);

  // Get pretransform matrix that rotates the scene according the surface orientation
  auto SrfPreTransform = get_surface_pretransform_matrix(float3{0, 0, 1});

  // Get projection matrix adjusted to the current screen orientation
  auto Proj = get_adjusted_projection_matrix(PI_F / 4.0f, 0.1f, 100.f);

  // Compute world-view-projection matrix
  m_world_view_proj_matrix = CubeModelTransform * View * SrfPreTransform * Proj;

  ///////////////////////////
  // Pre Render
  ///////////////////////////
  // ZE_ASSERT(m_ptr_immediate_context && m_ptr_swap_chain);

  // m_ptr_immediate_context->ClearStats();

  // Set render targets before issuing any draw command.
  // Note that Present() unbinds the back buffer if it is set as render target.
  ITextureView* ptr_rtv = m_ptr_swap_chain->GetCurrentBackBufferRTV();
  ITextureView* ptr_dsv = m_ptr_swap_chain->GetDepthBufferDSV();
  m_ptr_immediate_context->SetRenderTargets(1, &ptr_rtv, ptr_dsv, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // Clear the back buffer
  Vector4 clear_color = m_clear_color;
  if (m_convert_ps_output_to_gamma)
  {
      // If manual gamma correction is required, we need to clear the render target with sRGB color
      clear_color = LinearToSRGB(clear_color);
  }
  m_ptr_immediate_context->ClearRenderTarget(ptr_rtv, clear_color.Data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
  m_ptr_immediate_context->ClearDepthStencil(ptr_dsv, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  ///////////////////////////
  // Render
  ///////////////////////////
  // {
  //   // Map the buffer and write current world-view-projection matrix
  //   MapHelper<Matrix44> cb_constants(m_ptr_immediate_context, ptr_material->vertex_shader_constants(), MAP_WRITE, MAP_FLAG_DISCARD);
  //   cb_constants[0] = m_world_view_proj_matrix;
  // }

  // // Bind vertex and index buffers
  // const u64 offsets[] = {0};
  // IBuffer* ptr_buffs[] = {ptr_mesh->vertex_buffer()};
  // m_ptr_immediate_context->SetVertexBuffers(0, _countof(ptr_buffs), ptr_buffs, offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
  // m_ptr_immediate_context->SetIndexBuffer(ptr_mesh->index_buffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // // Set the pipeline state in the immediate context
  // m_ptr_immediate_context->SetPipelineState(ptr_material->pipeline_state());
  // // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
  // // makes sure that resources are transitioned to required states.
  // m_ptr_immediate_context->CommitShaderResources(ptr_material->shader_resource_binding(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // DrawIndexedAttribs draw_attrs;       // This is an indexed draw call
  // draw_attrs.IndexType    = VT_UINT32; // Index type
  // draw_attrs.NumIndices   = 36;
  // // Verify the state of vertex and index buffers
  // draw_attrs.Flags = DRAW_FLAG_VERIFY_ALL;
  // m_ptr_immediate_context->DrawIndexed(draw_attrs);

  ///////////////////////////
  // Post Render
  ///////////////////////////
  if (m_ptr_imgui_renderer)
  {
    if (m_imgui_show)
    {
      //m_ptr_imgui->Render(m_ptr_immediate_context);
       ImGui::Render();
       m_ptr_imgui_renderer->RenderDrawData(m_ptr_immediate_context, ImGui::GetDrawData());
    }
    else
    {
      //m_ptr_imgui->EndFrame();
       ImGui::EndFrame();
    }
  }

  m_ptr_swap_chain->Present(m_vsynch_enabled ? 1 : 0);
}

zv::Matrix44 zv::Renderer::get_adjusted_projection_matrix(f32 fov, f32 near_plane, f32 far_plane) const
{
  using namespace Diligent;

  const auto& SCDesc = m_ptr_swap_chain->GetDesc();

  float AspectRatio = static_cast<float>(SCDesc.Width) / static_cast<float>(SCDesc.Height);
  float XScale, YScale;
  if (SCDesc.PreTransform == SURFACE_TRANSFORM_ROTATE_90 ||
      SCDesc.PreTransform == SURFACE_TRANSFORM_ROTATE_270 ||
      SCDesc.PreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90 ||
      SCDesc.PreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270)
  {
      // When the screen is rotated, vertical FOV becomes horizontal FOV
      XScale = 1.f / std::tan(fov / 2.f);
      // Aspect ratio is inversed
      YScale = XScale * AspectRatio;
  }
  else
  {
      YScale = 1.f / std::tan(fov / 2.f);
      XScale = YScale / AspectRatio;
  }

  Matrix44 Proj;
  Proj._11 = XScale;
  Proj._22 = YScale;
  Proj.SetNearFarClipPlanes(near_plane, far_plane, m_ptr_device->GetDeviceInfo().NDC.MinZ == -1);
  return Proj;
}

zv::Matrix44 zv::Renderer::get_surface_pretransform_matrix(const zv::Vector3 &camera_view_axis) const
{
  using namespace Diligent;

  const auto& SCDesc = m_ptr_swap_chain->GetDesc();
  switch (SCDesc.PreTransform)
  {
    case SURFACE_TRANSFORM_ROTATE_90:
        // The image content is rotated 90 degrees clockwise.
        return Matrix44::RotationArbitrary(camera_view_axis, -PI_F / 2.f);

    case SURFACE_TRANSFORM_ROTATE_180:
        // The image content is rotated 180 degrees clockwise.
        return Matrix44::RotationArbitrary(camera_view_axis, -PI_F);

    case SURFACE_TRANSFORM_ROTATE_270:
        // The image content is rotated 270 degrees clockwise.
        return Matrix44::RotationArbitrary(camera_view_axis, -PI_F * 3.f / 2.f);

    case SURFACE_TRANSFORM_OPTIMAL:
        UNEXPECTED("SURFACE_TRANSFORM_OPTIMAL is only valid as parameter during swap chain initialization.");
        return Matrix44::Identity();

    case SURFACE_TRANSFORM_HORIZONTAL_MIRROR:
    case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90:
    case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180:
    case SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270:
        UNEXPECTED("Mirror transforms are not supported");
        return Matrix44::Identity();

    default:
        return Matrix44::Identity();
  }
}
