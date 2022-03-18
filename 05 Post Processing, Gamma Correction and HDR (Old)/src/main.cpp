#include <memory>

#include "Config.hpp"
#include "Application.hpp"

#include "State.hpp"

State state;

int main(int argc, char **argv) {
    std::unique_ptr<Config> config = std::make_unique<Config>();

    Application app(*config);
    app.Run();
    return 0;
}