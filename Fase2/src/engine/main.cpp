#include "engine/module.hpp"

#include <brief_int.hpp>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>
#include <string_view>

namespace engine {
auto display_help() -> void;
} // namespace engine

auto main(int argc, char* argv[]) -> int {
    using namespace brief_int::literals;
    namespace config = engine::config;

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

    if (argc > 2) {
        spdlog::error("too many options provided.");
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }

    if (argc == 1) {
        spdlog::info("no XML file was provided, rendering a default world.");
        engine::render::get().run();
    }

    auto const cmd = std::string_view{argv[1]};
    if (cmd == "-h" or cmd == "--help") {
        engine::display_help();
        return EXIT_SUCCESS;
    } else if (not cmd.starts_with("--input=")) {
        spdlog::error("unrecognized command '{}'.", cmd);
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }
    char const* const input_filename = cmd.data() + cmd.find('=') + 1_uz;

    errno = 0;
    auto world = engine::parse::xml::parse_world(input_filename);

    if (world.has_error()) {
        int const local_errno = errno;
        spdlog::error(
            "failed '{xml_filename}' world generation with error '{err}'",
            fmt::arg("xml_filename", input_filename),
            fmt::arg("err", world.error())
        );
        if (local_errno != 0) {
            spdlog::error(
                "errno set with value '{}'", std::strerror(local_errno)
            );
        }
        spdlog::critical("aborting.");
        return EXIT_FAILURE;
    }

    spdlog::info("successfully parsed world '{}'.", input_filename);
    engine::render::get().set_world(*world).run();
}

namespace engine {

auto display_help() -> void {
    fmt::print(
        "Usage:\n"
        "    {prog} (-h | --help)\n"
        "        Display this message.\n"
        "\n"
        "    {prog} --input=<input_file>\n"
        "        Render the world described in the XML file named\n"
        "        <input_file>.\n",
        fmt::arg("prog", config::PROG_NAME)
    );
}

} // namespace engine
