/*
 * Logger.cpp - contains all logging functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#include <Core/Logger.h>
#include <Core/PlatformContext.h>
#include <Core/Time.h>

#include <map>
#include <list>
#include <chrono>
#include <filesystem>

#if OS_WINDOWS
#include <windows.h>
#elif OS_MAC
#include <cstdio>
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <ThirdParty/fmt/include/fmt/core.h>
#include <ThirdParty/fmt/include/fmt/color.h>
#include <ThirdParty/fmt/include/fmt/chrono.h>


//------------------------------------------------------------------------------------------------------------------------------------
// Some constants
//------------------------------------------------------------------------------------------------------------------------------------

// the log filename
static const char* k_log_filename = "stdout";

// default display flags
#ifdef ZV_DEBUG_MODE
	const unsigned char k_errorflag_default =		(zv::k_logflag_write_to_debugger | zv::k_logflag_write_to_log_file | zv::k_logflag_write_to_console);
	const unsigned char k_warningflag_default =	(zv::k_logflag_write_to_debugger | zv::k_logflag_write_to_log_file | zv::k_logflag_write_to_console);
	const unsigned char k_logflag_default =		  (zv::k_logflag_write_to_debugger | zv::k_logflag_write_to_log_file | zv::k_logflag_write_to_console);
	const unsigned char k_externflag_default =	(zv::k_logflag_write_to_debugger | zv::k_logflag_write_to_log_file | zv::k_logflag_write_to_console);
#else
	const unsigned char k_errorflag_default =		0;
	const unsigned char k_warningflag_default =	0;
	const unsigned char k_logflag_default =		  0;
	const unsigned char k_externflag_default =  0;
#endif

//------------------------------------------------------------------------------------------------------------------------------------
// LogMgr
//------------------------------------------------------------------------------------------------------------------------------------

// singleton
namespace { class LogMgr; }
static LogMgr* s_ptr_log_mgr = NULL;

// LogMgr class
namespace
{
class LogMgr
{
public:
	enum class eErrorDialogResult : u8
	{
		Abort,
		Retry,
		Ignore
	};

  struct Tag
  {
    unsigned char flags;
    zv::FormatColor color = zv::FormatColor::light_gray;
  };

	typedef std::map<std::string, Tag> Tags;
	typedef std::list<zv::internal::ErrorMessenger*> ErrorMessengerList;

	Tags m_tags;
	ErrorMessengerList m_error_messengers;

  std::filesystem::path m_log_path{};
  mutable std::ofstream m_log_file;
	
  // TODO
	// // thread safety
	// CriticalSection m_tag_critical_section;
	// CriticalSection m_messenger_critical_section;

public:
	// construction
	LogMgr();
	~LogMgr();
  
  // void create(const char* logging_config_filename);
  bool create(const char* base_path);

	// logs
	void log(const std::string& tag, const std::string& message, std::optional<zv::FormatArgs> args, const char* func_name, const char* src_file, u32 line_num);
	void set_tag_config(const std::string& tag, unsigned char flags, zv::FormatColor color = zv::FormatColor::light_gray);

	// error messengers
	void add_error_messenger(zv::internal::ErrorMessenger* ptr_messenger);
	LogMgr::eErrorDialogResult error(const std::string& error_message, std::optional<zv::FormatArgs> args, bool is_fatal, const char* func_name, const char* src_file, u32 line_num);

private:
	// log helpers
	void output_final_buffer_to_logs(const std::string& final_buffer, unsigned char flags, zv::FormatColor color);
	void write_to_log_file(const std::string& data) const;
	void get_output_buffer(std::string& out_output_buffer, const std::string& tag, const std::string& message, std::optional<zv::FormatArgs> args, const char* func_name, const char* src_file, u32 line_num);
#if OS_WINDOWS
  void enable_virtual_terminal_processing();
#endif
};
}

LogMgr::LogMgr()
{
	// set up the default log tags
	set_tag_config("ERROR",   k_errorflag_default,   zv::FormatColor::red);
	set_tag_config("WARNING", k_warningflag_default, zv::FormatColor::yellow);
	set_tag_config("INFO",    k_logflag_default,     zv::FormatColor::light_gray);
	set_tag_config("EXTERN",  k_externflag_default,  zv::FormatColor::green);
}

/*
 * Destructor
 */
LogMgr::~LogMgr()
{
	// m_messenger_critical_section.lock();  // TODO
  for (auto it = m_error_messengers.begin(); it != m_error_messengers.end(); ++it)
	{
		zv::internal::ErrorMessenger* ptr_messenger = (*it);
		delete ptr_messenger;
	}
	m_error_messengers.clear();

  if (m_log_file.is_open()) {
    m_log_file.close();
  }

	// m_messenger_critical_section.unlock();  // TODO
}

/*
 * Initializes the logger.
 */
// void LogMgr::create(const char* loggingConfigFilename)
bool LogMgr::create(const char* base_path)
{
  const auto now = std::chrono::system_clock::now();
  const auto time_t = std::chrono::system_clock::to_time_t(now);
  const std::string timestamp = fmt::format("_{:%Y%m%d-%H%M%S}", *std::localtime(&time_t));

  m_log_path = base_path;
  m_log_path.append("Log");
  m_log_path.append(std::string(k_log_filename) + timestamp + ".log");

  std::filesystem::create_directories(m_log_path.parent_path());

  m_log_file = std::ofstream{m_log_path, std::ios::app};
  if (!m_log_file.is_open())
  {
    return false;
  }

#if OS_WINDOWS
  enable_virtual_terminal_processing();
#endif

  return true;

  // TODO
  // if (loggingConfigFilename)
  // {
  //     TiXmlDocument loggingConfigFile(loggingConfigFilename);
  //     if (loggingConfigFile.LoadFile())
  //     {
  //         TiXmlElement* pRoot = loggingConfigFile.RootElement();
  //         if (!pRoot)
  //             return;

  //         // Loop through each child element and load the component
  //         for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
  //         {
  //             unsigned char flags = 0;
  //             std::string tag(pNode->Attribute("tag"));

  //             int debugger = 0;
  //             pNode->Attribute("debugger", &debugger);
  //             if (debugger)
  //                 flags |= zv::k_logflag_write_to_debugger;

  //             int logfile = 0;
  //             pNode->Attribute("file", &logfile);
  //             if (logfile)
  //                 flags |= zv::k_logflag_write_to_log_file;

  //             set_tag_config(tag, flags);
  //         }
  //     }
  // }
}

/*
 * This function builds up the log string and outputs it to various places based on the display flags (m_displayFlags).
 */
void LogMgr::log(const std::string& tag, const std::string& message, std::optional<zv::FormatArgs> args, const char* func_name, const char* src_file, u32 line_num)
{
	// m_tag_critical_section.Lock();  // TODO
	Tags::iterator find_it = m_tags.find(tag);
	if (find_it != m_tags.end())
	{
		// m_tag_critical_section.Unlock();  // TODO
		
		std::string buffer;
		get_output_buffer(buffer, tag, message, args, func_name, src_file, line_num);
		output_final_buffer_to_logs(buffer, find_it->second.flags, find_it->second.color);
	}
	else
	{
		// Critical section is exited in the if statement above, so we need to exit it here if that didn't 
		// get executed.
    // m_tag_critical_section.Unlock();  // TODO
	}
} 

/*
 * Sets one or more display flags
 */
void LogMgr::set_tag_config(const std::string& tag, unsigned char flags, zv::FormatColor color)
{
	// m_tag_critical_section.Lock();  // TODO
	if (flags != 0)
	{
		Tags::iterator find_it = m_tags.find(tag);
		if (find_it == m_tags.end())
    {
			m_tags.insert(std::make_pair(tag, Tag{flags, color}));
    }
		else
    {
			find_it->second = Tag{flags, color};
    }
	}
	else
	{
		m_tags.erase(tag);
	}
	// m_tag_critical_section.Unlock();  // TODO
}

/*
 * Adds an error messenger to the list
 */
void LogMgr::add_error_messenger(zv::internal::ErrorMessenger* ptr_messenger)
{
	// m_messenger_critical_section.Lock();  // TODO
	m_error_messengers.push_back(ptr_messenger);
	// m_messenger_critical_section.Unlock();  // TODO
}

/*
 * Helper for ErrorMessenger.
 */
LogMgr::eErrorDialogResult LogMgr::error(const std::string& error_message, std::optional<zv::FormatArgs> args, bool is_fatal, const char* func_name, const char* src_file, u32 line_num)
{
	std::string tag = ((is_fatal) ? ("FATAL") : ("ERROR"));

	// buffer for our final output string
	std::string buffer;
	get_output_buffer(buffer, tag, error_message, args, func_name, src_file, line_num);

	// write the final buffer to all the various logs
	// m_tag_critical_section.Lock();  // TODO
	Tags::iterator find_it = m_tags.find(tag);
	if (find_it != m_tags.end())
  {
		output_final_buffer_to_logs(buffer, find_it->second.flags, find_it->second.color);
  }
	// m_tag_critical_section.Unlock();  // TODO

  // show the dialog box
#if OS_WINDOWS
  int result = ::MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_DEFBUTTON3);

	// act upon the choice
	switch (result)
	{
		case IDIGNORE : return eErrorDialogResult::Ignore;
		case IDABORT  : __debugbreak(); return eErrorDialogResult::Abort;  // assembly language instruction to break into the debugger
		case IDRETRY :	return eErrorDialogResult::Retry;
		default :       return eErrorDialogResult::Retry;
	}
#elif OS_MAC:
  // TODO: ???
  CFStringRef cfTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
  CFStringRef cfMessage = CFStringCreateWithCString(NULL, message, kCFStringEncodingUTF8);
  
  CFOptionFlags responseFlags;
  CFUserNotificationDisplayAlert(
      0,                                // Timeout (0 means no timeout)
      kCFUserNotificationNoteAlertLevel, // Alert level (info, warning, error)
      NULL, NULL, NULL,                  // Icons (NULL for default)
      cfTitle,                           // Title of the message box
      cfMessage,                         // Message text
      CFSTR("OK"), NULL, NULL,           // Button titles
      &responseFlags                     // User response (button clicked)
  );

  CFRelease(cfTitle);
  CFRelease(cfMessage);

  __builtin_debugtrap();
#else
# error "Other OS currently not supported."
#endif
}

/*
 * This is a helper function that writes the data string to the log file.
 *
 * IMPORTANT: The two places this function is called from wrap the code in the tag critical section (m_pTagCriticalSection), 
 * so that makes this call thread safe.  If you call this from anywhere else, make sure you wrap it in that critical section.
 */
void LogMgr::output_final_buffer_to_logs(const std::string& final_buffer, unsigned char flags, zv::FormatColor color)
{
	// Write the log to each display based on the display flags
	if ((flags & zv::k_logflag_write_to_log_file) > 0)  // log file
  {
		write_to_log_file(final_buffer);
  }
	if ((flags & zv::k_logflag_write_to_debugger) > 0)  // debugger output window
  {
#if OS_WINDOWS
    ::OutputDebugStringA(final_buffer.c_str());
#endif
  }
  if ((flags & zv::k_logflag_write_to_console) > 0) // console output
  {
    fmt::print(fmt::fg(color), "{}", final_buffer);
  }
}

/*
 * This is a helper function that writes the data string to the log file.
 */
void LogMgr::write_to_log_file(const std::string &data) const
{
  if (!m_log_file.is_open())
  {
    return; // can't write to the log file for some reason
  }

  m_log_file << data;
}

/*
 * Fills out_output_buffer with the find error string.
 */
void LogMgr::get_output_buffer(std::string& out_output_buffer, const std::string& tag, const std::string& message, std::optional<zv::FormatArgs> args, const char* func_name, const char* src_file, u32 line_num)
{
  std::chrono::time_point now = std::chrono::system_clock::now();
  auto now_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(now - now_in_seconds);
  const std::string timestamp = fmt::format("{}.{:03}", now_in_seconds, msec.count());

  if (args.has_value())
  {
    const std::string formatted = vformat(message, args.value());
    out_output_buffer = tag.empty() ? formatted : "[" + tag + "][" + timestamp + "] " + formatted;
  }
  else
  {
		out_output_buffer = tag.empty() ? message : "[" + tag + "][" + timestamp + "] " + message;
  }

	if (func_name != NULL)
	{
		out_output_buffer += "\nFunction: ";
		out_output_buffer += func_name;
	}

	if (src_file != NULL)
	{
		out_output_buffer += "\n";
		out_output_buffer += src_file;
	}

	if (line_num != 0)
	{
		out_output_buffer += "\nLine: ";
    char line_numBuffer[11];
    memset(line_numBuffer, 0, sizeof(char));
    out_output_buffer += _itoa(line_num, line_numBuffer, 10);
	}

  out_output_buffer += "\n";
}

#if OS_WINDOWS
void LogMgr::enable_virtual_terminal_processing()
{
  HANDLE hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
  {
      return;
  }

  DWORD dwMode = 0;
  if (!::GetConsoleMode(hOut, &dwMode))
  {
      return;
  }

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  ::SetConsoleMode(hOut, dwMode);
}
#endif

//------------------------------------------------------------------------------------------------------------------------------------
// ErrorMessenger
//------------------------------------------------------------------------------------------------------------------------------------

zv::internal::ErrorMessenger::ErrorMessenger()
{
	s_ptr_log_mgr->add_error_messenger(this);
	m_enabled = true;
}

void zv::internal::ErrorMessenger::show(const std::string &error_message, std::optional<zv::FormatArgs> args, bool is_fatal, const char *func_name, const char *src_file, u32 line_num)
{
  if (m_enabled)
  {
    if (s_ptr_log_mgr->error(error_message, args, is_fatal, func_name, src_file, line_num) == LogMgr::eErrorDialogResult::Ignore)
    {
      m_enabled = false;
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
// Logger (internal)
//------------------------------------------------------------------------------------------------------------------------------------

// void zv::Logger::create(const char* loggingConfigFilename)
bool zv::Logger::create(const char* base_path)
{
  if (s_ptr_log_mgr)
  {
    return false;
  }

  s_ptr_log_mgr = new LogMgr;
  // s_ptr_log_mgr->create(loggingConfigFilename);
  return s_ptr_log_mgr->create(base_path);
}

void zv::Logger::destroy(void)
{
	delete s_ptr_log_mgr;
	s_ptr_log_mgr = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------
// Logger
//------------------------------------------------------------------------------------------------------------------------------------

void zv::Logger::log(const std::string& tag, const std::string& message, std::optional<FormatArgs> args, const char* func_name, const char* src_file, u32 line_num)
{
  ZV_ASSERT(s_ptr_log_mgr);
  s_ptr_log_mgr->log(tag, message, args, func_name, src_file, line_num);
}

void zv::Logger::set_tag_config(const std::string &tag, unsigned char flags, zv::FormatColor color)
{
	ZV_ASSERT(s_ptr_log_mgr);
	s_ptr_log_mgr->set_tag_config(tag, flags, color);
}
