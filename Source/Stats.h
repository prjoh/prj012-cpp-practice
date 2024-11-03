/*
 * Stats.h - module for profiling functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#pragma once

#include <Renderer.h>
#include <Core/Utility.h>
#include <Core/PrimitiveTypes.h>


namespace zv
{
  const u32 k_sample_size = 50;

  class Stats : public IImGuiRenderable
  {
    MovingAverage<f32, k_sample_size> m_fps_avg{ 5.0f / k_sample_size };
    MovingAverage<f32, k_sample_size> m_frame_time_ms_avg{ 5.0f / k_sample_size };

  public:
    void update();
    void imgui_update() override;
  };
}
