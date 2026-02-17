#include <cctype>
#include <cstdint>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <optional>

enum class TokenKind {
    Identifier,
    Number,
    Plus, Minus, Mul, Pow, Div, Modulo,  // +, -, *, **, /, %
    PlusAssign, MinusAssign, MulAssign, PowAssign, DivAssign, ModuloAssign, // +=, -=, *=, **=, /=, %=,
    GreaterThan, LesserThan, GreaterThanEqual, LesserThanEqual, Equal, // >, <, >=, <=, ==
    Ampersand, Caret, // &, ^
    At, Hashtag, // @, #
    ExclamationMark, QuestionMark, // !, ?
    Assign, // =
    LeftArrow, // <-
    RightArrow, // ->
    FatArrow, // =>
    // Punctuation
    Comma, Dot, DoubleDot, TripleDot, SemiColon, Colon, DoubleColon,

    SingleQuote, // '
    DoubleQuote, // " "
    LParen, RParen,  // ( )
    LBracket, RBracket, // [ ]
    LBrace, RBrace, // { }

    EOFToken,
    Unknown,
    Newline,
    ErrorToken,
    /// Keywords
    // Declaration Keywords
    FnKw, StructKw, UnionKw, EnumKw, TraitKw, VarKw, ValKw,
    // Control Flow Keywords
    IfKw, ElseKw, IsKw, ThenKw,
    WhileKw, ForKw, InKw, DoKw, LoopKw,
    ReturnKw, DeferKw, CatchKw,
    // Boolean Keywords
    TrueKw, FalseKw, NullKw,
    // Boolean Operators
    AndKw, OrKw, NotKw, XorKw, NorKw,
    // Imports
    ImportKw, FromKw, AsKw, AllKw,
    /// Literals
    StringLiteral, NumberLiteral,


};

// Represents a single unit of code
struct Token {
    TokenKind kind;
    std::string_view lexeme; // View into the original source code
    uint32_t line;
};

class Lexer {
    std::string_view source;
    size_t start = 0;
    size_t current = 0;
    uint32_t line = 1;

    bool is_at_end() const { return current >= source.length(); }
    char advance(uint32_t advance_by=1) { return source[current + advance_by]; }
    char peek() const { return is_at_end() ? '\0' : source[current]; }
    char peek_forwards(char peek_by=1) { return is_at_end() ? '\0' : source[current + peek_by]; }

    Token make_token(TokenKind kind) {
        return Token{kind, source.substr(start, current - start), line};
    }
    Token error_token(std::string_view message) {
        return Token{TokenKind::ErrorToken, message, line};
    }

public:
    Lexer(std::string_view source) : source(source) {}

    Token scanToken() {
        // Skip whitespace
        while (!is_at_end()) {
            char c = peek();
            if (c == ' ' || c == '\r' || c == '\t') {
                advance();
            } else if (c == '\n') {
                line++;
                advance();
            } else {
                break;
            }
        }
        // This is for the lexer to handle views into the text/code
        size_t start = current;
        if (is_at_end()) return make_token(TokenKind::EOFToken);

        // Keywords
        if (std::isalpha(current)) {

        /// =====Identifers=====
            if (isalpha(current)) {
                while (isalnum(peek())) advance();
                return make_token(TokenKind::Identifier);
            }

        /// =====Declarations=====
            // struct
            if (current == 's' && peek_forwards() == 't' && peek_forwards(2) == 'r' && peek_forwards(3) == 'u' && peek_forwards(4) == 'c' && peek_forwards(5) == 't') {
                advance(6);
                return make_token(TokenKind::StructKw);
            }
            // enum
            if (current == 'e' && peek_forwards() == 'n' && peek_forwards(2) == 'u' && peek_forwards(3) == 'm') {
                advance(4);
                return make_token(TokenKind::EnumKw);
            }
            // union
            if (current == 'u' && peek_forwards() == 'n' && peek_forwards(2) == 'i' && peek_forwards(3) == 'o' && peek_forwards(4) == 'n') {
                advance(5);
                return make_token(TokenKind::UnionKw);
            }
            // var
            if (current == 'v' && peek_forwards() == 'a' && peek_forwards(2) == 'r') {
                advance(3);
                return make_token(TokenKind::VarKw);
            }
            // val
            if (current == 'v' && peek_forwards() == 'a' && peek_forwards(2) == 'l') {
                advance(3);
                return make_token(TokenKind::ValKw);
            }
            // fn
            if (current == 'f' && peek_forwards() == 'n') {
                advance(2);
                return make_token(TokenKind::FnKw);
            }
            /// Todo: implement trait or capability or interface

        /// =====Control Flow=====
            // if
            if (current == 'i' && peek_forwards() == 'f') {
                advance(2);
                return make_token(TokenKind::IfKw);
            }
            // else
            if (current == 'e' && peek_forwards() == 'l' && peek_forwards(2) == 's' && peek_forwards(3) == 'e') {
                advance(4);
                return make_token(TokenKind::ElseKw);
            }
            // is
            if (current == 'i' && peek_forwards() == 's') {
                advance(2);
                return make_token(TokenKind::IsKw);
            }
            // then
            if (current == 't' && peek_forwards() == 'h' && peek_forwards(2) == 'e' && peek_forwards(3) == 'n') {
                advance(4);
                return make_token(TokenKind::ThenKw);
            }
            // do
            if (current == 'd' && peek_forwards() == 'o') {
                advance(2);
                return make_token(TokenKind::DoKw);
            }
            // while
            if (current == 'w' && peek_forwards() == 'h' && peek_forwards(2) == 'i' && peek_forwards(3) == 'l' && peek_forwards(4) == 'e') {
                advance(5);
                return make_token(TokenKind::WhileKw);
            }
            // for
            if (current == 'f' && peek_forwards() == 'o' && peek_forwards(2) == 'r') {
                advance(3);
                return make_token(TokenKind::ForKw);
            }
            // in
            if (current == 'i' && peek_forwards() == 'n') {
                advance(2);
                return make_token(TokenKind::InKw);
            }
            // loop
            if (current == 'l' && peek_forwards() == 'o' && peek_forwards(2) == 'o' && peek_forwards(3) == 'p') {
                advance(4);
                return make_token(TokenKind::LoopKw);
            }
            // return
            if (current == 'r' && peek_forwards() == 'e' && peek_forwards(2) == 't' && peek_forwards(3) == 'u' && peek_forwards(4) == 'r' && peek_forwards(5) == 'n') {
                advance(6);
                return make_token(TokenKind::ReturnKw);
            }
            // defer
            if (current == 'd' && peek_forwards() == 'e' && peek_forwards(2) == 'f' && peek_forwards(3) == 'e' && peek_forwards(4) == 'r') {
                advance(5);
                return make_token(TokenKind::DeferKw);
            }
            // catch
            if (current == 'c' && peek_forwards() == 'a' && peek_forwards(2) == 't' && peek_forwards(3) == 'c' && peek_forwards(4) == 'h') {
                advance(5);
                return make_token(TokenKind::CatchKw);
            }

        /// =====Values=====
            // null
            if (current == 'n' && peek_forwards() == 'u' && peek_forwards(2) == 'l' && peek_forwards(3) == 'l') {
                advance(4);
                return make_token(TokenKind::NullKw);
            }
            // true
            if (current == 't' && peek_forwards() == 'r' && peek_forwards(2) == 'u' && peek_forwards(3) == 'e') {
                advance(4);
                return make_token(TokenKind::TrueKw);
            }
            // false
            if (current == 'f' && peek_forwards() == 'a' && peek_forwards(2) == 'l' && peek_forwards(3) == 's' && peek_forwards(4) == 'e') {
                advance(5);
                return make_token(TokenKind::FalseKw);
            }

        /// =====Multi Character Symbols and Punctuation=====
            // .. (exclusive range)
            if (current == '.' && peek_forwards() == '.') {
                advance(2);
                return make_token(TokenKind::DoubleDot);
            }
            // ... (inclusive range)
            if (current == '.' && peek_forwards() == '.' && peek_forwards(2) == '.') {
                advance(3);
                return make_token(TokenKind::TripleDot);
            }
            // :: (scope resolution)
            if (current == ':' && peek_forwards() == ':') {
                advance(2);
                return make_token(TokenKind::DoubleColon);
            }
            // <- (left arrow)
            if (current == '<' && peek_forwards() == '-') {
                advance(2);
                return make_token(TokenKind::LeftArrow);
            }
            // ->
            if (current == '-' && peek_forwards() == '>') {
                advance(2);
                return make_token(TokenKind::RightArrow);
            }
            // => (fat arrow)
            if (current == '>' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::FatArrow);
            }
            // ** (pow)
            if (current == '*' && peek_forwards() == '*') {
                advance(2);
                return make_token(TokenKind::Pow);
            }
            // >= (greater than or equal)
            if (current == '>' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::GreaterThanEqual);
            }
            // <= (less than or equal)
            if (current == '<' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::LesserThanEqual);
            }
            // == (equal)
            if (current == '=' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::Equal);
            }
            // +=
            if (current == '+' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::PlusAssign);
            }
            // -=
            if (current == '-' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::MinusAssign);
            }
            // *=
            if (current == '*' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::MulAssign);
            }
            // /=
            if (current == '/' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::DivAssign);
            }
            // %=
            if (current == '%' && peek_forwards() == '=') {
                advance(2);
                return make_token(TokenKind::ModuloAssign);
            }
            // **=
            if (current == '*' && peek_forwards() == '*') {
                advance(2);
                return make_token(TokenKind::PowAssign);
            }
            // \n
            if (current == '\n') {
                advance();
                return make_token(TokenKind::Newline);
            }

        /// =====String Literals=====
            if (current == '\"') {
                advance();
                while (peek() != '\"' && !is_at_end()) {
                    if (peek() == '\n') line++;
                    advance();
                }
                if (is_at_end()) return error_token("Unterminated string.");
                advance();
                return make_token(TokenKind::StringLiteral);
            }

        }


        char c = advance();

        // Single-character tokens
        switch (c) {
            // Delimiters
            case '{': advance(); return make_token(TokenKind::LBrace);
            case '}': advance(); return make_token(TokenKind::RBrace);
            case '(': advance(); return make_token(TokenKind::LParen);
            case ')': advance(); return make_token(TokenKind::RParen);
            case '[': advance(); return make_token(TokenKind::LBracket);
            case ']': advance(); return make_token(TokenKind::RBracket);
            case '\"': advance(); return make_token(TokenKind::DoubleQuote);
            case '\'': advance(); return make_token(TokenKind::SingleQuote);
            // Punctuation
            case '!': advance(); return make_token(TokenKind::ExclamationMark);
            case '?': advance(); return make_token(TokenKind::QuestionMark);
            case '&': advance(); return make_token(TokenKind::Ampersand);
            case '^': advance(); return make_token(TokenKind::Caret);
            case ',': advance(); return make_token(TokenKind::Comma);
            case '.': advance(); return make_token(TokenKind::Dot);
            case ':': advance(); return make_token(TokenKind::Colon);
            case ';': advance(); return make_token(TokenKind::SemiColon);

            case '/': advance(); return make_token(TokenKind::Div);
            case '*': advance(); return make_token(TokenKind::Mul);
            case '+': advance(); return make_token(TokenKind::Plus);
            case '-': advance(); return make_token(TokenKind::Minus);
            case '%': advance(); return make_token(TokenKind::Modulo);
            // Comparison
            case '>': advance(); return make_token(TokenKind::GreaterThan);
            case '<': advance(); return make_token(TokenKind::LesserThan);
            // Miscellaneous
            case '@': advance();return make_token(TokenKind::At);
            case '#': advance();return make_token(TokenKind::Hashtag);
            case '=': advance();return make_token(TokenKind::Assign);

        }
        // Numeric literals
        if (isdigit(c)) {
            while (isdigit(peek())) advance();
            return make_token(TokenKind::Number);
        }


        return make_token(TokenKind::Unknown);
    }
};
