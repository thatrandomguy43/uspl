

#include "IO.hpp"

#include <iostream>
#include <fstream>
#include <variant>
#include <sstream>

using namespace std;


vector<string> IO::program_args{};
vector<IO::CompileError> IO::error_list{};

void IO::GrabCLIArguments(int argc, const char** argv)
{
    for (int argument_index = 0; argument_index < argc; argument_index++)
    {
        program_args.push_back(argv[argument_index]);
    }
    return;
}



int main(int argc, const char** argv){
    IO::GrabCLIArguments(argc, argv);

    if (IO::program_args.size() < 2){
        cout << "specify a damn file😳" << endl;
        return 1;
    }
    fstream test_in;
    test_in.open(IO::program_args[1], ios::in);
    if (test_in.fail())
    {
        cout << "could not load file " <<  IO::program_args[1] << endl;
        return 1;
    }
    stringstream input_buffer;
    string input_storage{};

    input_buffer << test_in.rdbuf();
    input_storage = input_buffer.str();

    test_in.close();

    fstream primary_out;

    primary_out.open("testout.ll", ios::out);
    primary_out << input_storage;
    primary_out.close();
    return 0;
}