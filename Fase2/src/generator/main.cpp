#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "generator/module.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "util/parse_number.hpp"
#include "util/try.hpp"

#include <brief_int.hpp>
#include <cerrno>
#include <concepts>
#include <cstdlib>
#include <cstring>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <span>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

using namespace brief_int;
using namespace std::string_view_literals;

namespace generator {

extern const std::unordered_map<
    std::string_view,
    auto (*)(std::span<char const*>) -> void
>  cli_actions;

template <::util::number N, std::invocable F>
auto try_parse_number(
    std::string_view s,
    F&& get_err_msg
) -> N
    requires std::constructible_from<
        std::invalid_argument,
        std::invoke_result_t<F>
    >;

auto try_parse_u32(std::string_view s) -> u32;
auto try_parse_float(std::string_view s) -> float;
auto check_num_args(usize expected, usize actual) -> void;
auto display_help() -> void;

} // namespace generator

auto main(int argc, char* argv[]) -> int {
    using generator::cli_actions;
    namespace config = generator::config;

    auto log_prefix = fmt::format(
        fmt::emphasis::bold | fg(fmt::terminal_color::white),
        "{}",
        config::PROG_NAME
    );
    log_prefix += " [%^%l%$] <%!>: %v";

    spdlog::set_default_logger(spdlog::stderr_color_mt("stderr"));
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::err);
    spdlog::set_pattern(std::move(log_prefix));

    if (argc < 2) {
        spdlog::error("no command provided.");
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }

    auto const cmd = std::string_view{argv[1]};
    auto const maybe_action = cli_actions.find(cmd);
    if (maybe_action == cli_actions.end()) {
        spdlog::error("unrecognized command '{}'.", cmd);
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }

    try {
        auto const args = std::span {
            const_cast<char const**>(argv + 2),
            static_cast<std::size_t>(argc - 2)
        };
        errno = 0;
        maybe_action->second(args);
    } catch (std::exception const& e) {
        int const local_errno = errno;
        spdlog::error(
            "failed {primitive} generation with error '{err}'.",
            fmt::arg("primitive", cmd),
            fmt::arg("err", e.what())
        );
        if (local_errno != 0) {
            spdlog::error(
                "errno set with value '{}'", std::strerror(local_errno)
            );
        }
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }
}

namespace generator {

std::unordered_map<
    std::string_view,
    auto (*)(std::span<char const*>) -> void
> const cli_actions = {
    {
        "-h",
        [](std::span<char const*>) {
            display_help();
        }
    },
    {
        "--help",
        [](std::span<char const*>) {
            display_help();
        }
    },
    {
        "sphere",
        [](std::span<char const*> const args) {
            check_num_args(4, args.size());
            auto const radius = try_parse_float(args[0]);
            auto const num_slices = try_parse_u32(args[1]);
            auto const num_stacks = try_parse_u32(args[2]);
            auto output_file = fmt::output_file(args[3]);
            auto const result
                = generate_sphere(radius, num_slices, num_stacks, output_file);
            if (result.has_error()) {
                throw std::runtime_error(fmt::format("{}", result.error()));
            }
        }
    },
    {
        "box",
        [](std::span<char const*> const args) {
            check_num_args(3, args.size());
            auto const side_len = try_parse_float(args[0]);
            auto const num_divs = try_parse_u32(args[1]);
            auto output_file = fmt::output_file(args[2]);
            auto const result = generate_box(side_len, num_divs, output_file);
            if (result.has_error()) {
                throw std::runtime_error(fmt::format("{}", result.error()));
            }
        }
    },
    {
        "cone",
        [](std::span<char const*> const args) {
            check_num_args(5, args.size());
            auto const radius = try_parse_float(args[0]);
            auto const height = try_parse_float(args[1]);
            auto const num_slices = try_parse_u32(args[2]);
            auto const num_stacks = try_parse_u32(args[3]);
            auto output_file = fmt::output_file(args[4]);
            auto const result = generate_cone(
                radius, height, num_slices, num_stacks, output_file
            );
            if (result.has_error()) {
                throw std::runtime_error(fmt::format("{}", result.error()));
            }
        },
    },
    {
        "plane",
        [](std::span<char const*> const args) {
            check_num_args(3, args.size());
            auto const side_len = try_parse_float(args[0]);
            auto const num_divs = try_parse_u32(args[1]);
            auto output_file = fmt::output_file(args[2]);
            auto const result = generate_plane(side_len, num_divs, output_file);
            if (result.has_error()) {
                throw std::runtime_error(fmt::format("{}", result.error()));
            }
        }
    },
};

template <::util::number N, std::invocable F>
auto try_parse_number(
    std::string_view const s,
    F&& get_err_msg
) -> N
    requires std::constructible_from<
        std::invalid_argument,
        std::invoke_result_t<F>
    >
{
    return TRY_OPTION_OR(
        ::util::parse_number<N>(s),
        throw std::invalid_argument{std::forward<F>(get_err_msg)()}
    );
}

auto try_parse_u32(std::string_view const s) -> u32 {
    return try_parse_number<u32>(
        s,
        [s] { return fmt::format("failed parsing '{}' into u32", s); }
    );
}

auto try_parse_float(std::string_view const s) -> float {
    return try_parse_number<float>(
        s,
        [s] { return fmt::format("failed parsing '{}' into float", s); }
    );
}

auto check_num_args(usize const expected, usize const actual) -> void {
    if (expected != actual) {
        throw std::invalid_argument {
            fmt::format("expected {} arguments, but got {}", expected, actual)
        };
    }
}

auto display_help() -> void {
    fmt::print(
        "Usage:\n"
        "    {prog} (-h | --help)\n"
        "        Display this message.\n"
        "\n"
        "\n"
        "    {prog} (sphere | box | cone | plane) <args>... <output_file>\n"
        "        Draw the specified primitive and store the resulting\n"
        "        vertices in a file named <output_file>.\n"
        "\n"
        "        sphere <radius> <num_slices> <num_stacks>\n"
        "            Generate a sphere with radius <radius>, <num_slices>\n"
        "            slices and <num_stacks> stacks.\n"
        "\n"
        "        box <side_len> <num_divs>\n"
        "            Generate a box where each side has <len> units in length\n"
        "            and <num_divs> divisions along each axis.\n"
        "\n"
        "        cone <radius> <height> <num_slices> <num_stacks>\n"
        "            Generate a cone with radius <radius>, height <height>,\n"
        "            <num_slices> slices and <num_stacks> stacks.\n"
        "\n"
        "        plane <side_len> <num_divs>\n"
        "            Generate a plane with <len> units in length and\n"
        "            <num_divs> divisions along each axis.\n",
        fmt::arg("prog", config::PROG_NAME)
    );
}

} // namespace generator
