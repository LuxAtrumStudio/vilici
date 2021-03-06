#ifndef SPECULA_UTIL_LOG_HPP_
#define SPECULA_UTIL_LOG_HPP_

#include <cstdio>
#include <ctime>

#include "fmt.hpp"

#define fatal(msg, ...)                                                        \
  logger::__log(logger::FATAL, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define error(msg, ...)                                                        \
  logger::__log(logger::ERROR, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define warning(msg, ...)                                                      \
  logger::__log(logger::WARNING, msg, __FILE__, __func__, __LINE__,            \
                ##__VA_ARGS__)
#define info(msg, ...)                                                         \
  logger::__log(logger::INFO, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define success(msg, ...)                                                      \
  logger::__log(logger::SUCCESS, msg, __FILE__, __func__, __LINE__,            \
                ##__VA_ARGS__)
#define version(msg, ...)                                                      \
  logger::__log(logger::VERSION, msg, __FILE__, __func__, __LINE__,            \
                ##__VA_ARGS__)

namespace logger {
enum LogType { FATAL, ERROR, WARNING, INFO, SUCCESS, VERSION, TIMER };
extern FILE *file_;
extern unsigned verbose_;
extern bool color_;

static const char *type_string[7] = {"FATAL  ", "ERROR  ", "WARNING", "INFO   ",
                                     "SUCCESS", "VERSION", "TIMER  "};
static const char *type_color[7] = {"\033[1;31m", "\033[1;31m", "\033[1;33m",
                                    "\033[1;37m", "\033[1;32m", "\033[1;33m",
                                    "\033[1;34m"};

bool init();
template <typename... _ARGS>
void __log(const LogType &type, const char *msg, const char *file,
           const char *func, size_t line, const _ARGS &... args) {
  std::time_t raw_time = time(NULL);
  std::tm *current_tm = std::localtime(&raw_time);
  std::string message =
      fmt::fmt("[{}] <{}:{}:{}> ({}:{}:{}) {}\n", type_string[type],
               current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec,
               file, func, line, fmt::fmt(msg, args...));
  if (verbose_ >= static_cast<unsigned>(type)) {
    if (color_ == true) {
      printf("%s%s\033[0m", type_color[type], message.c_str());
    } else {
      printf("%s", message.c_str());
    }
  }
  fprintf(file_, "%s", message.c_str());
  if (type == FATAL || type == ERROR) {
    fflush(file_);
  }
}
template <typename... _ARGS>
void __log(const LogType &type, const std::string &msg, const char *file,
           const char *func, size_t line, const _ARGS &... args) {
  std::time_t raw_time = time(NULL);
  std::tm *current_tm = std::localtime(&raw_time);
  std::string message =
      fmt::fmt("[{}] <{}:{}:{}> ({}:{}:{}) {}\n", type_string[type],
               current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec,
               file, func, line, fmt::fmt(msg, args...));
  if (verbose_ >= static_cast<unsigned>(type)) {
    if (color_ == true) {
      printf("%s%s\033[0m", type_color[type], message.c_str());
    } else {
      printf("%s", message.c_str());
    }
  }
  fprintf(file_, "%s", message.c_str());
  if (type == FATAL || type == ERROR) {
    fflush(file_);
  }
}
template <typename... _ARGS>
void __log(const char *type, const char *color, const char *msg,
           const char *file, const char *func, size_t line,
           const _ARGS &... args) {
  std::time_t raw_time = time(NULL);
  std::tm *current_tm = std::localtime(&raw_time);
  std::string message =
      fmt::fmt(msg, "[{}] <{}:{}:{}> ({}:{}:{}) {}", type, current_tm->tm_hour,
               current_tm->tm_min, current_tm->tm_sec, file, func, line,
               fmt::fmt(msg, args...));
  if (color_ == true) {
    printf("%s%s\033[0m", color, message.c_str());
  } else {
    printf("%s", message.c_str());
  }
  fprintf(file_, "%s", message.c_str());
}
bool term();

extern FILE *file_;
extern unsigned verbose_;
extern bool color_;
} // namespace logger

#endif // SPECULA_UTIL_LOG_HPP_
