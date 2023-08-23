#include <vector>
#include <string>

namespace IO {

    extern std::vector<std::string> program_args;
    void GrabCLIArguments(int argc, const char** argv);

}