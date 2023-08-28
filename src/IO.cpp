
#include "Tokenizer.hpp"
#include "IO.hpp"

#include <iostream>
#include <fstream>
#include <optional>
#include <variant>
#include <sstream>
#include <map>

using namespace std;
using namespace IO;


vector<string> IO::program_args{};
vector<IO::CompileError> IO::error_list{};


void IO::AddError(CompileError error)
{
    error_list.push_back(error);
    return;
}

vector<string> IO::GrabCLIArguments(int argc, const char** argv)
{
    vector<string> args_vec;
    for (int argument_index = 1; argument_index < argc; argument_index++)
    {
        args_vec.push_back(argv[argument_index]);
    }
    return args_vec;
}

string IO::GetFileContents(string filename)
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

int main(int argc, const char** argv){
    program_args = GrabCLIArguments(argc, argv);

    if (program_args.size() == 0){
        cout << "specify a damn file" << endl;
        return 0xDEE5D1CC;
    }

    map<string, string> files_contents;

    for (auto arg : program_args)
    {
        if (arg[0] != '-')
        {
            files_contents[arg] = GetFileContents(arg);
        }
    }

    map<string, vector<Tokenizer::Token>> token_streams;

    for (auto file : files_contents)
    {
        token_streams[file.first] = Tokenizer::TokenizeText(file.second);
    }



      
    




    return 0;
}