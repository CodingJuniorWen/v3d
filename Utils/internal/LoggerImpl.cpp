#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace Utils {
namespace {

spdlog::level::level_enum toSpdLevel(LogLevel level) {
  switch (level) {
  case LogLevel::Trace:
    return spdlog::level::trace;
  case LogLevel::Debug:
    return spdlog::level::debug;
  case LogLevel::Info:
    return spdlog::level::info;
  case LogLevel::Warn:
    return spdlog::level::warn;
  case LogLevel::Error:
    return spdlog::level::err;
  case LogLevel::Critical:
    return spdlog::level::critical;
  case LogLevel::Off:
    return spdlog::level::off;
  }
  return spdlog::level::info;
}

} // namespace

void Logger::init(LogLevel level, const std::string &logFile) {
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

  if (!logFile.empty()) {
    sinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true));
  }

  auto logger =
      std::make_shared<spdlog::logger>("Utils", sinks.begin(), sinks.end());
  logger->set_level(toSpdLevel(level));
  logger->flush_on(spdlog::level::warn);

  spdlog::set_default_logger(std::move(logger));
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
}

void Logger::shutdown() { spdlog::shutdown(); }

void Logger::setLevel(LogLevel level) {
  spdlog::set_level(toSpdLevel(level));
}

void Logger::flush() { spdlog::default_logger()->flush(); }

void Logger::trace(const std::string &message) { spdlog::trace(message); }

void Logger::debug(const std::string &message) { spdlog::debug(message); }

void Logger::info(const std::string &message) { spdlog::info(message); }

void Logger::warn(const std::string &message) { spdlog::warn(message); }

void Logger::error(const std::string &message) { spdlog::error(message); }

void Logger::critical(const std::string &message) {
  spdlog::critical(message);
}

} // namespace Utils
