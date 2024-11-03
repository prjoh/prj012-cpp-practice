#pragma once

#include <Core/PrimitiveTypes.h>
#include <ThirdParty/DiligentCore/Common/interface/RefCntAutoPtr.hpp>


namespace Diligent
{
  class IEngineFactory;
  class IRenderDevice;
  class IDeviceContext;
  class ISwapChain;
  class IPipelineState;
  class IBuffer;
  class IShaderResourceBinding;
  class ITexture;
  class ITextureView;
  class IShaderSourceInputStreamFactory;
  class ImGuiImplDiligent;
  class ImGuiDiligentRenderer;

  enum TEXTURE_VIEW_TYPE : unsigned char;
  enum TEXTURE_FORMAT : u16;
  enum RENDER_DEVICE_TYPE;

  struct SwapChainDesc;
  struct Win32NativeWindow;
  struct EngineD3D11CreateInfo;
  struct EngineD3D12CreateInfo;
  struct ImGuiDiligentCreateInfo;
}

namespace zv
{
  template <typename T>
  using RefCntAutoPtr          = Diligent::RefCntAutoPtr<T>;
  using IEngineFactory         = Diligent::IEngineFactory;
  using IRenderDevice          = Diligent::IRenderDevice;
  using IDeviceContext         = Diligent::IDeviceContext;
  using ISwapChain             = Diligent::ISwapChain;
  using IPipelineState         = Diligent::IPipelineState;
  using IBuffer                = Diligent::IBuffer;
  using IShaderResourceBinding = Diligent::IShaderResourceBinding;
  using ITexture               = Diligent::ITexture;
  using ITextureView           = Diligent::ITextureView;
  using IShaderSourceInputStreamFactory = Diligent::IShaderSourceInputStreamFactory;
  using ImGuiImplDiligent = Diligent::ImGuiImplDiligent;
  using ImGuiDiligentRenderer = Diligent::ImGuiDiligentRenderer;

  using eTextureViewType = Diligent::TEXTURE_VIEW_TYPE;
  using eTextureFormat = Diligent::TEXTURE_FORMAT;
  using eRenderDeviceType = Diligent::RENDER_DEVICE_TYPE;

  using SwapChainDesc = Diligent::SwapChainDesc;
  using Win32NativeWindow = Diligent::Win32NativeWindow;
  using EngineD3D11CreateInfo = Diligent::EngineD3D11CreateInfo;
  using EngineD3D12CreateInfo = Diligent::EngineD3D12CreateInfo;
  using ImGuiDiligentCreateInfo = Diligent::ImGuiDiligentCreateInfo;
}
