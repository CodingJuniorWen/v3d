#pragma once

#include "Export.h"
#include <string>

namespace Utils {

enum class LogLevel {
  Trace = 0,
  Debug,
  Info,
  Warn,
  Error,
  Critical,
  Off
};

/**
 * Thin wrapper around spdlog. Public headers do not expose spdlog;
 * call sites depend only on Utils.
 */
class UTILS_EXPORT Logger {
public:
  /// Initialize default logger (colored stdout). Optional file sink when
  /// @p logFile is non-empty.
  static void init(LogLevel level = LogLevel::Info,
                   const std::string &logFile = {});

  static void shutdown();
  static void setLevel(LogLevel level);
  static void flush();

  static void trace(const std::string &message);
  static void debug(const std::string &message);
  static void info(const std::string &message);
  static void warn(const std::string &message);
  static void error(const std::string &message);
  static void critical(const std::string &message);

private:
  Logger() = default;
};

} // namespace Utils

#define UTILS_LOG_TRACE(msg) ::Utils::Logger::trace(msg)
#define UTILS_LOG_DEBUG(msg) ::Utils::Logger::debug(msg)
#define UTILS_LOG_INFO(msg) ::Utils::Logger::info(msg)
#define UTILS_LOG_WARN(msg) ::Utils::Logger::warn(msg)
#define UTILS_LOG_ERROR(msg) ::Utils::Logger::error(msg)
#define UTILS_LOG_CRITICAL(msg) ::Utils::Logger::critical(msg)
