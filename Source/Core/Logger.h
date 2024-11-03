/*
 * Logger.h - contains all logging functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#pragma once

#include <string>
#include <optional>

#include <Config.h>
#include <Core/Format.h>
#include <Core/PrimitiveTypes.h>

namespace zv
{
  //---------------------------------------------------------------------------------------------------------------------
  // Constants
  //---------------------------------------------------------------------------------------------------------------------

  // display flags
  const unsigned char k_logflag_write_to_log_file =		1 << 0;
  const unsigned char k_logflag_write_to_debugger =		1 << 1;
  const unsigned char k_logflag_write_to_console =		1 << 2;

  //---------------------------------------------------------------------------------------------------------------------
  // ErrorMessenger
  //---------------------------------------------------------------------------------------------------------------------

  namespace internal
  {
    // This class is used by the debug macros and shouldn't be accessed externally.
    class ErrorMessenger
    {
      bool m_enabled;

    public:
      ErrorMessenger();
      void show(const std::string& error_message, std::optional<FormatArgs> args, bool is_fatal, const char* func_name, const char* src_file, u32 line_num);
    };
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  // Public logger interface
  //------------------------------------------------------------------------------------------------------------------------------------
  namespace Logger
  {
    // construction; must be called at the beginning and end of the program
    // void create(const char* loggingConfigFilename);
    bool create(const char* base_path);
    void destroy();
    
    // logging functions
    void log(const std::string& tag, const std::string& message, std::optional<FormatArgs> args, const char* func_name, const char* src_file, u32 line_num);
    void set_tag_config(const std::string& tag, unsigned char flags, zv::FormatColor color);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
// Logging macros
//------------------------------------------------------------------------------------------------------------------------------------

#if ZV_LOG_ENABLED

// Fatal Errors are fatal and are always presented to the user.
#define ZV_FATAL(format, ...) \
	do \
	{ \
		static zv::internal::ErrorMessenger* ptr_error_messenger = new zv::internal::ErrorMessenger(); \
		ptr_error_messenger->show(format, zv::make_format_args(__VA_ARGS__), true, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\

// Errors are bad and potentially fatal.  They are presented as a dialog with Abort, Retry, and Ignore.  Abort will
// break into the debugger, retry will continue the game, and ignore will continue the game and ignore every subsequent 
// call to this specific error.  They are ignored completely in release mode.
#define ZV_ERROR(format, ...) \
	do \
	{ \
		static zv::internal::ErrorMessenger* ptr_error_messenger = new zv::internal::ErrorMessenger(); \
		ptr_error_messenger->show(format, zv::make_format_args(__VA_ARGS__), false, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\

// Warnings are recoverable.  They are just logs with the "WARNING" tag that displays calling information.  The flags
// are initially set to WARNINGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden normally.
#define ZV_WARNING(format, ...) \
	do \
	{ \
		zv::Logger::log("WARNING", format, zv::make_format_args(__VA_ARGS__), __FUNCTION__, __FILE__, __LINE__); \
	}\
	while (0)\

// This is just a convenient macro for logging if you don't feel like dealing with tags.  It calls Log() with a tag
// of "INFO".  The flags are initially set to LOGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden 
// normally.
#define ZV_INFO(format, ...) \
	do \
	{ \
		zv::Logger::log("INFO", format, zv::make_format_args(__VA_ARGS__), NULL, NULL, 0); \
	} \
	while (0) \

// This macro is used for logging and should be the preferred method of "printf debugging".  You can use any tag 
// string you want, just make sure to enabled the ones you want somewhere in your initialization.
#define ZV_LOG(tag, format, ...) \
	do \
	{ \
		zv::Logger::log(tag, format, zv::make_format_args(__VA_ARGS__), NULL, NULL, 0); \
	} \
	while (0) \

// This macro replaces GCC_ASSERT().
#define ZV_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
		  static zv::internal::ErrorMessenger* ptr_error_messenger = new zv::internal::ErrorMessenger(); \
			ptr_error_messenger->show(#expr, std::nullopt, false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	} \
	while (0) \

#else

#define ZV_FATAL(...) (void)(0)
#define ZV_ERROR(...) (void)(0)
#define ZV_WARNING(...) (void)(0)
#define ZV_INFO(...) (void)(0)
#define ZV_LOG(...) (void)(0)
#define ZV_ASSERT(...) (void)(0)

#endif // ZV_LOG_ENABLED
