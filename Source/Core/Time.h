/*
 * Time.h - module containing timing functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#pragma once

#include <Core/PrimitiveTypes.h>

namespace zv
{
  namespace Time
  {
    // Clock management interface
    namespace Clock
    {
      // construction; must be called at the beginning and end of the program
      void create();
      void destroy();

      void reset();
      void tick();
    }

    // Public interface for timing information
    f32 elapsed_time_s();
    f64 elapsed_time_s_64();
    f32 delta_time_s();
    f32 delta_time_s_32();
    void set_time_scale(f64 time_scale);
  }
}
