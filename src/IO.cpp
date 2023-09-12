#include "Tokenizer.hpp"
#include "ASTBuilder.hpp"
#include "IO.hpp"
#include <format>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace IO;


set<string> IO::program_args;
map<string,vector<IO::CompileError>> error_list;

void PrintErrors(const set<SourceFile>& files)
{
    for (auto file : files)
    {
        size_t line_num = 1;
        size_t char_num = 1;
        size_t index = 0;
        for (auto error : error_list[file.name])
        {
            while (index != error.position) {
                index++;
                char_num++;
                if (file.text[index] == '\n')
                {
                    line_num++;
                    char_num = 0;
                }
            }
            cout << format("Error at ({}, {}) in {}: {}", line_num, char_num, error.filename, error.error_msg) << endl;
        }
    }
    return;
}

void IO::AddError(CompileError error)
{
    error_list[error.filename].push_back(error);
    return;
}

set<string> GrabCLIArguments(int argc, const char** argv)
{
    set<string> args_vec;
    for (int16_t argument_index = 1; argument_index < argc; argument_index++)
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
    auto start_time = chrono::high_resolution_clock::now();

    program_args = GrabCLIArguments(argc, argv);

    if (program_args.size() == 0){
        cout << "specify a damn file" << endl;
        return 0xDEE5D1CC;
    }

    vector<SourceFile> file_contents;

    for (auto arg : program_args)
    {
        if (arg[0] != '-')
        {
            file_contents.push_back( SourceFile{arg, GetFileContents(arg)});
        }
    }
    map<string, vector<Token>> token_streams;

    for (auto file : file_contents)
    {
        token_streams[file.name] = file.TokenizeText();
    }

    map<string, AST::TranslationUnit> file_trees;
    ASTBuilder builder;
    for (auto stream : token_streams)
    {
        file_trees.try_emplace(stream.first ,builder.BuildFile(stream.second, stream.first));
    }
    

    auto end_time = chrono::high_resolution_clock::now();
    auto time_elapsed = end_time - start_time;
    cout << "Time elapsed: " << chrono::duration_cast<chrono::milliseconds>(time_elapsed) << endl;
    return 0;
}