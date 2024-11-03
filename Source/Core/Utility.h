/*
 * Utility.h - collection of core utility functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#pragma once

#include <array>
#include <algorithm>
#include <numeric>

#include <Core/PrimitiveTypes.h>

namespace zv
{
  // non-copyable utiliy base class
  class NonCopyable
  {
  protected:
	  NonCopyable() = default;
    
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
  };

  // non-movable utiliy base class
  class NonMovable : public NonCopyable 
  {
  protected:
    NonMovable() = default;
    
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
  };

  // check whether a debugger is attached
  bool is_debugger_present();

  // Helper class for calculating moving averages
  template<typename T, u32 SAMPLE_SIZE>
  class MovingAverage
  {
    f32 m_current_sample_start_time;
    u32 m_current_sample_count;
    T m_current_sample_accumulator;
    T m_sample_rate_s;
    std::array<T, SAMPLE_SIZE> m_samples;
    u32 m_next_sample_index;
    T m_samples_avg;

  public:
    explicit MovingAverage(const f32 sample_rate_s = 1.f)
      : m_current_sample_start_time(0.0f)
      , m_current_sample_count(0)
      , m_current_sample_accumulator(0)
      , m_sample_rate_s(sample_rate_s)
      , m_next_sample_index(0)
      , m_samples_avg(0)
    {
        m_samples.fill(0.0);
    }

    void reset()
    {
      *this = MovingAverage<T, SAMPLE_SIZE>(m_sample_rate_s);
    }

    void update(f32 current_time, T new_value)
    {
      if (m_samples.size() == 0)
      {
        return;
      }

      if (m_current_sample_start_time == 0)
      {
        m_current_sample_start_time = current_time;
      }

      ++m_current_sample_count;
      m_current_sample_accumulator += new_value;

      if (current_time - m_current_sample_start_time > m_sample_rate_s)
      {
        // Limit to a minimum of 5 frames per second
        // const T accumulator_avg = std::max(m_current_sample_accumulator / m_current_sample_count, 5.0f);

        m_samples[m_next_sample_index] = m_current_sample_accumulator / m_current_sample_count;

        m_next_sample_index = (m_next_sample_index + 1) % static_cast<u32>(m_samples.size());

        // calculate the average
        m_samples_avg = std::accumulate(m_samples.begin(), m_samples.end(), 0.0) / m_samples.size();

        // reset the accumulator and counter
        m_current_sample_accumulator = m_current_sample_count = 0;
        m_current_sample_start_time = current_time;
      }
    }

    [[nodiscard]] T get_average() const { return m_samples_avg; }
  };
}
