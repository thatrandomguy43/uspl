#include <vector>
#include <string>
#include <set>

namespace IO {
    class CompileError{
        public:
        std::string filename;
        size_t position;
        std::string error_msg;
    };
    extern std::set<std::string> program_args;
    extern std::vector<CompileError> error_list;
    extern std::string current_file;
    std::set<std::string> GrabCLIArguments(int argc, const char** argv);
    std::string GetFileContents(std::string filepath);
    void AddError(CompileError error);

}