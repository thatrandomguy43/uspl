#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
namespace IO {
    class CompileError{
        public:
        std::string filename;
        size_t position;
        std::string error_msg;
    };
    extern std::set<std::string> program_args;
    void AddError(CompileError error);

}