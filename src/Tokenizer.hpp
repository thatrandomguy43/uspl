#include <string>
#include <optional>
#include <vector>
#include <variant>

enum TokenType
{
    identifier,

    literal_bool,
    literal_integer,
    literal_float,
    literal_char,
    literal_string,

    operator_assignment,
    keyword_var,
    keyword_const,
    keyword_function,
    keyword_class,
    keyword_namespace,

    keyword_do,
    keyword_end,
    keyword_if,
    keyword_else,
    keyword_while,
    keyword_for,
    keyword_in,
    keyword_break,

    operator_addition,
    operator_subtraction,
    operator_multiplication,
    operator_division,
    operator_modulo,

    operator_logical_or,
    operator_logical_and,
    operator_logical_not,

    operator_equality,
    operator_inequality,
    operator_lessthan,
    operator_greaterthan,
    operator_less_or_equal,
    operator_greater_or_equal,


    operator_bitwise_or,
    operator_bitwise_and,
    operator_bitwise_not,
    operator_bitwise_xor,
    operator_shift_left,
    operator_shift_right,

    operator_address,
    operator_pointer,
    operator_reference,
    operator_member_access,
    operator_scope_resolution,
    keyword_new,
    keyword_delete,
    keyword_this,


    //these can't really be handled like operators, because they have stuff inside them and are somewhat ambiguous
    open_parentheses,
    close_parentheses,
    open_bracket,
    close_bracket,
    open_brace,
    close_brace,

    seperator,

    reserved_hash,
    reserved_exclamation,
    reserved_questionmark,
    reserved_colon,
    reserved_semicolon,

    error_token

    //ones i can think of rn, some of these will take a while to be added
};
class Token
{
    public:
    std::variant<std::nullopt_t, bool, char, std::string, uint64_t, double> contents;
    size_t length;
    TokenType type;
};

class SourceFile
{
    public:
    const std::string name;
    const std::string text;

    private:
    size_t position;

    Token TestForToken();
    Token ProcessTextLiteral();
    Token ProcessNumberLiteral();
    std::pair<std::string, size_t> EscapeText();

    public:
    SourceFile( const std::string& filename, const std::string& filetext) : name(filename) ,text(filetext), position(0) {}
    bool operator<(const SourceFile& other) const
    {
        return this->name < other.name;
    }
    std::vector<Token> TokenizeText();
};