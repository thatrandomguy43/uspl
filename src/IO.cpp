#include "Tokenizer.hpp"
#include "IO.hpp"

#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace IO;


set<string> IO::program_args;
map<string,vector<IO::CompileError>> IO::error_list;

void PrintErrors(const map<string,string>& files)
{   
    
}

void IO::AddError(CompileError error)
{
    error_list[error.filename].push_back(error);
    return;
}

set<string> GrabCLIArguments(int argc, const char** argv)
{
    set<string> args_vec;
    for (int argument_index = 1; argument_index < argc; argument_index++)
    {
        args_vec.insert(argv[argument_index]);
    }
    return args_vec;
}

string GetFileContents(string filename)
{
    fstream file_in;
    file_in.open(filename, ios::in);
    if (file_in.fail())
    {
        AddError({"Arguments", 0, "Could not open file " + filename});
        return "";
    }
    stringstream input_buffer;
    string input_storage;

    input_buffer << file_in.rdbuf();
    input_storage = input_buffer.str();

    file_in.close();

    return input_storage;
}

int main(int argc, const char** argv)
{
    program_args = GrabCLIArguments(argc, argv);

    if (program_args.size() == 0){
        cout << "specify a damn file" << endl;
        return 0xDEE5D1CC;
    }

    set<SourceFile> file_contents;

    for (auto arg : program_args)
    {
        if (arg[0] != '-')
        {
            file_contents.insert( SourceFile{arg, GetFileContents(arg)});
        }
    }
    map<string, vector<Token>> token_streams;

    for (auto file : file_contents)
    {
        token_streams[file.name] = file.TokenizeText();
    }

    return 0;
}