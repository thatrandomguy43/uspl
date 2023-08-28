#include <vector>
#include <string>


namespace IO {
    class CompileError{
        public:
        std::string filename;
        size_t position;
        std::string error_msg;
    };
    extern std::vector<std::string> program_args;
    extern std::vector<CompileError> error_list;
    std::vector<std::string> GrabCLIArguments(int argc, const char** argv);
    std::string GetFileContents(std::string filepath);
    
    void AddError(CompileError error);

}