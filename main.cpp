#include <iostream>
#include <fstream>
#include <iterator>
#include <variant>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

vector<string> program_args{};

namespace IO {
vector<string> GrabCLIArguments(int argc, const char** argv){
    vector<string> args{};
    for (int argument_index = 0; argument_index < argc; argument_index++)
    {
        args.push_back(argv[argument_index]);
    }
    return args;
}
}

int main(int argc, const char** argv){
    program_args = IO::GrabCLIArguments(argc, argv);
    if (program_args.size() < 2){
        cout << "specify a damn file" << endl;
        return 1;
    }
    fstream test_in;
    test_in.open(program_args[1], ios::in);
    if (test_in.fail())
    {
        cout << "could not load file " <<  program_args[1] << endl;
        return 1;
    }
    stringstream input_buffer;
    string input_storage{};

    input_buffer << test_in.rdbuf();
    input_storage = input_buffer.str();

    test_in.close();

    fstream primary_out;

    primary_out.open("out.ll", ios::out);
    primary_out << input_storage;
    primary_out.close();
    return 0;
}