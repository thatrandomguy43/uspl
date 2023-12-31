#include "TypeAnalyzer.hpp"
#include "IO.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace IO;


class CodeFile
{
    public:
    string name;
    string source_code;
    vector<Token> tokens;
    AST::TranslationUnit tree;
    string IRFile;
};

set<string> IO::program_args;
unordered_map<string,vector<IO::CompileError>> error_list;
chrono::time_point<chrono::high_resolution_clock> start_time;

void PrintErrors(const vector<CodeFile>& files)
{   
    for (const auto& error : error_list["Arguments"])
    {
        cout << "Error in program arguments: " << error.error_msg << endl;
    }
    for (const auto& file : files)
    {
        size_t line_num = 1;
        size_t char_num = 1;
        size_t index = 0;
        sort<vector<CompileError>::iterator, bool(*)(const CompileError&, const CompileError&)>(error_list[file.name].begin(), error_list[file.name].end(), &CompileError::LessThan);
        for (auto error : error_list[file.name])
        {
            while (index != error.position and index < file.source_code.length()) {
                index++;
                char_num++;
                if (file.source_code[index] == '\n')
                {
                    line_num++;
                    char_num = 0;
                }
            }
            cout << "Error at (" << to_string(line_num) << ", " << to_string(char_num) << ") in " << file.name << ": " << error.error_msg << endl;
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
void PrintTimeElapsed()
{
    auto end_time = chrono::high_resolution_clock::now();
    auto time_elapsed = end_time - start_time;
    cout << "Time elapsed: " << chrono::duration_cast<chrono::milliseconds>(time_elapsed) << endl;
}

int main(int argc, const char** argv)
{
    start_time = chrono::high_resolution_clock::now();
    program_args = GrabCLIArguments(argc, argv);

    vector<CodeFile> files;

    for (auto& arg : program_args)
    {
        if (arg[0] != '-')
        {
            files.push_back( CodeFile{arg, GetFileContents(arg), {}, {}, {}});
        }
    }

    if (files.size() == 0){
        cout << "specify a damn file" << endl;
        return 0xDEE5D1CC;
    }

    if (not error_list.empty())
    {
        PrintErrors(files);
        PrintTimeElapsed();
        return 1;
    }

    Tokenizer tokenizer; //duh
    for (auto& file : files)
    {
        file.tokens = tokenizer.TokenizeText(file.name, file.source_code);
        cout << "File " << file.name << " tokenized.\n";
    }

    if (not error_list.empty())
    {
        PrintErrors(files);
        PrintTimeElapsed();
        return 2;
    }

    AST::Builder builder;
    for (auto& file : files)
    {
        file.tree = builder.BuildFile(file.tokens, &file.name);
        cout << "Built AST of " << file.name << "\n";
    }

    if (not error_list.empty())
    {
        PrintErrors(files);
        PrintTimeElapsed();
        return 3;
    }

    if (not error_list.empty())
    {
        PrintErrors(files);
        PrintTimeElapsed();
        return 4;
    }

    PrintTimeElapsed();
    return 0;
}