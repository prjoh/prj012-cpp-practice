/*
 * Time.cpp - module containing timing functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#include <Core/Time.h>
#include <Core/Logger.h>

#include <ThirdParty/SDL2/include/SDL.h>

//------------------------------------------------------------------------------------------------------------------------------------
// Clock
//------------------------------------------------------------------------------------------------------------------------------------

// singleton
namespace{ class Clock; }
static Clock* s_ptr_clock = nullptr;

namespace
{
class Clock
{
  u64 m_previous_time{ 0 };
  u64 m_current_time{ 0 };
  u64 m_delta_time{ 0 };
  f64 m_time_scale{ 1.0 };
  u64 m_elapsed_time{ 0 };
  u64 m_count_per_second{ 1 };
  u64 m_start_time{ 0 };

public:
  Clock();

  void reset();
  void tick();

  constexpr f32 elapsed_time_s() const { return static_cast<f32>(m_elapsed_time) / m_count_per_second; }
  constexpr f64 elapsed_time_s_64() const { return static_cast<f64>(m_elapsed_time) / m_count_per_second; }

  constexpr f32 delta_time_s() const { return static_cast<f32>(m_delta_time) / m_count_per_second; }
  constexpr f32 delta_time_s_32() const { return static_cast<f64>(m_delta_time) / m_count_per_second; }

  void set_time_scale(f64 time_scale) { m_time_scale = time_scale; }
};

Clock::Clock()
  : m_count_per_second(SDL_GetPerformanceFrequency())
{
}

void Clock::reset()
{
  m_start_time = SDL_GetPerformanceCounter();
  m_current_time = m_start_time;
  m_delta_time = 0;
}

void Clock::tick()
{
  m_previous_time = m_current_time;
  m_current_time = SDL_GetPerformanceCounter();

  m_delta_time = ( m_current_time - m_previous_time ) * m_time_scale;
  m_elapsed_time += m_delta_time;
}
}

//------------------------------------------------------------------------------------------------------------------------------------
// Time interface
//------------------------------------------------------------------------------------------------------------------------------------

void zv::Time::Clock::create()
{
	if (!s_ptr_clock)
  {
		s_ptr_clock = new ::Clock;
    s_ptr_clock->reset();
  }
}

void zv::Time::Clock::destroy()
{
  delete s_ptr_clock;
	s_ptr_clock = NULL;
}

void zv::Time::Clock::reset()
{
  ZV_ASSERT(s_ptr_clock);
  s_ptr_clock->reset();
}

void zv::Time::Clock::tick()
{
  ZV_ASSERT(s_ptr_clock);
  s_ptr_clock->tick();
}

f32 zv::Time::elapsed_time_s()
{ 
  return s_ptr_clock->elapsed_time_s();
}

f64 zv::Time::elapsed_time_s_64()
{
  return s_ptr_clock->elapsed_time_s_64();
}

f32 zv::Time::delta_time_s()
{
  return s_ptr_clock->delta_time_s();
}

f32 zv::Time::delta_time_s_32()
{
  return s_ptr_clock->delta_time_s_32();
}

void zv::Time::set_time_scale(f64 time_scale)
{
  s_ptr_clock->set_time_scale(time_scale);
}
