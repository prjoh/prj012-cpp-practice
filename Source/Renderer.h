#pragma once

#include <Core/PrimitiveTypes.h>
#include <RendererDecl.h>
#include <MathDefines.h>
#include <Window.h>


namespace zv
{
  class IImGuiRenderable {
  public:
    virtual void imgui_update() = 0;
  };

  class Renderer
  {
  public:
    Renderer();
    ~Renderer();

  public:
    struct CreateParams {
      Window* ptr_window;
      eRenderDeviceType device_type;
      Vector4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };
      bool enable_fullscreen{ false };
      bool enable_vsynch{ true };
      bool init_imgui{ true };
    };
    bool create(const CreateParams& params);
    void destroy();

    void register_imgui_renderable(IImGuiRenderable* ptr_imgui_renderable);

    void update();

  private:
    bool init_imgui(const SwapChainDesc& swap_chain_desc, const Window* ptr_window);

    // Returns projection matrix adjusted to the current screen orientation
    Matrix44 get_adjusted_projection_matrix(f32 fov, f32 near_plane, f32 far_plane) const;
    // Returns pretransform matrix that matches the current screen rotation
    Matrix44 get_surface_pretransform_matrix(const Vector3& camera_view_axis) const;

  private:
    RefCntAutoPtr<IEngineFactory>                  m_ptr_engine_factory{ nullptr };
    RefCntAutoPtr<IShaderSourceInputStreamFactory> m_ptr_shader_source_factory{ nullptr };

    RefCntAutoPtr<ISwapChain>     m_ptr_swap_chain{ nullptr };
    RefCntAutoPtr<IRenderDevice>  m_ptr_device{ nullptr };
    RefCntAutoPtr<IDeviceContext> m_ptr_immediate_context{ nullptr };

    std::unique_ptr<ImGuiDiligentRenderer> m_ptr_imgui_renderer{ nullptr };
    std::vector<IImGuiRenderable*> m_imgui_renderables;

    Vector4 m_clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

    bool m_vsynch_enabled{ false };
    bool m_convert_ps_output_to_gamma{ false };
    bool m_wireframe_supported{ false };
    bool m_imgui_available{ false };
    bool m_imgui_show{ false };

    //std::unique_ptr<ImGuiImplDiligent> m_ptr_imgui;

    // // RefCntAutoPtr<IPipelineState> m_ptr_pso; //

    // // RefCntAutoPtr<IBuffer>                m_cube_vertex_buffer; //
    // // RefCntAutoPtr<IBuffer>                m_cube_index_buffer; //
    // // RefCntAutoPtr<IBuffer>                m_vs_constants; //
    // // RefCntAutoPtr<IBuffer>                m_instance_buffer;

    // // RefCntAutoPtr<IShaderResourceBinding> m_ptr_srb; //

    // // RefCntAutoPtr<ITextureView>           m_texture_srv; //

    Matrix44             m_world_view_proj_matrix;

    Matrix44             m_view_proj_matrix;
    Matrix44             m_rotation_matrix;
    s32                  m_grid_size   = 5;
    // static constexpr s32 k_max_grid_size  = 32;
    // static constexpr s32 k_max_instances = k_max_grid_size * k_max_grid_size * k_max_grid_size;
  };
}
