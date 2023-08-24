#include <vector>
#include <string>


namespace IO {
    class CompileError{
        public:
        std::string filename;
        size_t position;
        int error_code;
    };
    extern std::vector<std::string> program_args;
    extern std::vector<CompileError> error_list;
    void GrabCLIArguments(int argc, const char** argv);
    void AddError(CompileError);

}