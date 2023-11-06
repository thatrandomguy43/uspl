#pragma once
#include <vector>
#include <string>
#include <set>
namespace IO {
    class CompileError{
        public:
        std::string filename;
        size_t position;
        std::string error_msg;
        static bool LessThan(const CompileError& lhs, const CompileError& rhs)
        {
            return lhs.position < rhs.position;
        }
    };
    extern std::set<std::string> program_args;
    void AddError(CompileError error);

}