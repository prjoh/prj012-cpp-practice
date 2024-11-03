/*
 * Stats.cpp - module for profiling functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#include <Stats.h>
#include <Core/Time.h>

#include <ThirdParty/sdl-imgui/imgui_impl_sdl.h>


void zv::Stats::update()
{
  m_frame_time_ms_avg.update(Time::elapsed_time_s(), Time::delta_time_s() * 1000.0f);
  m_fps_avg.update(Time::elapsed_time_s(), 1.0f / Time::delta_time_s());
}

void zv::Stats::imgui_update()
{
  ImGui::Begin("Engine Stats");
  ImGui::Text("FPS: %.1f", m_fps_avg.get_average());
  ImGui::Text("Frame Time: %.6f ms", m_frame_time_ms_avg.get_average());
  ImGui::End();
}
