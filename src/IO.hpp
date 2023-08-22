#pragma once

#include <vector>
#include <string>

namespace IO {

    std::vector<std::string> program_args;
    void GrabCLIArguments(int argc, const char** argv);

}