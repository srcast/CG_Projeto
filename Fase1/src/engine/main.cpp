#include "engine/module.hpp"
#include "util/pretty_print.hpp"

#include <brief_int.hpp>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fmt/core.h>
#include <string_view>

namespace engine {
    auto display_help() -> void;
} // namespace engine

auto main(int argc, char* argv[]) -> int {
    using namespace brief_int::literals;
    using engine::config::prog_name;

    if (argc > 2) {
        pretty_print_err("too many options provided.\n");
        return EXIT_FAILURE;
    }

    if (argc == 1) {
        // No XML file was provided, render a default world.
        engine::render::launch().run();
    }

    auto const cmd = std::string_view{argv[1]};
    if (cmd == "-h" || cmd == "--help") {
        engine::display_help();
        return EXIT_SUCCESS;
    } else if (not cmd.starts_with("--input=")) {
        pretty_print_err("unrecognized command '{}'.\n", cmd);
        return EXIT_FAILURE;
    }
    char const* const input_filename = cmd.data() + cmd.find('=') + 1_uz;

    errno = 0;
    auto world = engine::parse::xml::parse_world(input_filename);

    if (world.has_error()) {
        int const local_errno = errno;
        pretty_print_err(
            "failed '{}' world generation with error '{}'",
            input_filename,
            world.error()
        );
        if (local_errno != 0) {
            fmt::print(stderr, ": '{}'", std::strerror(local_errno));
        }
        fmt::print(stderr, ".\n");
        return EXIT_FAILURE;
    }

    pretty_print("successfully parsed world '{}'.\n", input_filename);
    std::fflush(stdout);
    engine::render::launch().set_world(*world).run();
}

namespace engine {

auto display_help() -> void {
    using namespace fmt::literals;
    fmt::print(
        "Usage:\n"
        "    {prog} (-h | --help)\n"
        "        Display this message.\n"
        "\n"
        "    {prog} --input=<input_file>\n"
        "        Render the world described in the XML file named\n"
        "        <input_file>.\n",
        "prog"_a = config::prog_name
    );
}

} // namespace engine
