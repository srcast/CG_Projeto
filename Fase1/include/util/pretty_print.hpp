#pragma once

#include <fmt/color.h>

#define pretty_print(format_string, ...) do {                                  \
    fmt::print(                                                                \
        fmt::emphasis::bold | fg(fmt::terminal_color::white),                  \
        "{}: ",                                                                \
        prog_name                                                              \
    );                                                                         \
    fmt::print(format_string __VA_OPT__(,) __VA_ARGS__);                       \
} while (0)

#define pretty_print_err(format_string, ...) do {                              \
    fmt::print(                                                                \
        stderr,                                                                \
        fmt::emphasis::bold | fg(fmt::terminal_color::white),                  \
        "{}: ",                                                                \
        prog_name                                                              \
    );                                                                         \
    fmt::print(                                                                \
        stderr,                                                                \
        fmt::emphasis::bold | fg(fmt::terminal_color::red),                    \
        "error: "                                                              \
    );                                                                         \
    fmt::print(stderr, format_string __VA_OPT__(,)  __VA_ARGS__);              \
} while (0)
