#include "generator/primitives/module.hpp"
#include "util/pretty_print.hpp"

#include <brief_int.hpp>
#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <cstring>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <span>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

using namespace brief_int;
using namespace std::string_view_literals;

auto display_help() -> void;
auto parse_u32(char const* s) -> u32;
auto parse_float(char const* s) -> float;
auto check_num_args(usize expected, usize actual) -> void;

auto constexpr prog_name = "generator"sv;

auto const cli_actions = std::unordered_map<
    std::string_view,
    auto (*)(std::span<char const*>) -> void
> {
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
            float const radius = parse_float(args[0]);
            u32 const num_slices = parse_u32(args[1]);
            u32 const num_stacks = parse_u32(args[2]);
            auto output_file = fmt::output_file(args[3]);
            generate_sphere(radius, num_slices, num_stacks, output_file);
        }
    },
    {
        "box",
        [](std::span<char const*> const args) {
            check_num_args(3, args.size());
            u32 const num_units = parse_u32(args[0]);
            u32 const grid_len = parse_u32(args[1]);
            auto output_file = fmt::output_file(args[2]);
            generate_box(num_units, grid_len, output_file);
        }
    },
    {
        "cone",
        [](std::span<char const*> const args) {
            check_num_args(5, args.size());
            float const radius = parse_float(args[0]);
            float const height = parse_float(args[1]);
            u32 const num_slices = parse_u32(args[2]);
            u32 const num_stacks = parse_u32(args[3]);
            auto output_file = fmt::output_file(args[4]);
            generate_cone(radius, height, num_slices, num_stacks, output_file);
        },
    },
    {
        "plane",
        [](std::span<char const*> const args) {
            check_num_args(3, args.size());
            u32 const len = parse_u32(args[0]);
            u32 const num_divs = parse_u32(args[1]);
            auto output_file = fmt::output_file(args[2]);
            generate_plane(len, num_divs, output_file);
        }
    },
};

auto display_help() -> void {
    using namespace fmt::literals;
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
        "        box <num_units> <grid_len>\n"
        "            Generate a box with <num_units> units, where each side\n"
        "            is divided in a <grid_len>x<grid_len> grid.\n"
        "\n"
        "        cone <radius> <height> <num_slices> <num_stacks>\n"
        "            Generate a cone with radius <radius>, height <height>,\n"
        "            <num_slices> slices and <num_stacks> stacks.\n"
        "\n"
        "        plane <len> <num_divs>\n"
        "            Generate a plane with <len> units in length, and\n"
        "            <num_divs> divisions along each axis.\n",
        "prog"_a = ::prog_name
    );
}

auto parse_u32(char const* const s) -> u32 {
    u32 uninit;
    char const* const end = s + std::strlen(s);
    auto const [parse_end, err] = std::from_chars(s, end, uninit);
    if (parse_end != end || err != std::errc{}) {
        throw std::invalid_argument {
            fmt::format(
                "failed parsing '{}' into u32",
                std::string_view{s, end}
            )
        };
    }
    return uninit;
}

auto parse_float(char const* const s) -> float {
    float uninit;
    char const* const end = s + std::strlen(s);
    auto const [parse_end, err] = std::from_chars(s, end, uninit);
    if (parse_end != end || err != std::errc{}) {
        throw std::invalid_argument {
            fmt::format(
                "failed parsing '{}' into float",
                std::string_view{s, end}
            )
        };
    }
    return uninit;
}

auto check_num_args(usize const expected, usize const actual) -> void {
    if (expected != actual) {
        throw std::invalid_argument {
            fmt::format("expected {} arguments, but got {}", expected, actual)
        };
    }
}

auto main(int argc, char* argv[]) -> int {
    if (argc < 2) {
        pretty_print_err("no command provided.\n");
        return EXIT_FAILURE;
    }

    auto const cmd = std::string_view{argv[1]};
    auto const maybe_action = ::cli_actions.find(cmd);
    if (maybe_action == ::cli_actions.end()) {
        pretty_print_err("unrecognized command '{}'.\n", cmd);
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
        using namespace fmt::literals;
        int const local_errno = errno;
        pretty_print_err(
            "failed {primitive} generation with error '{err}'",
            "primitive"_a = cmd,
            "err"_a = e.what()
        );
        if (local_errno != 0) {
            fmt::print(stderr, ": '{}'", std::strerror(local_errno));
        }
        fmt::print(stderr, ".\n");
        return EXIT_FAILURE;
    }
}
