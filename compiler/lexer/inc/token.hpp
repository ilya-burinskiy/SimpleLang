#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>

class Token;
using token_uptr = std::unique_ptr<Token>;


class Token {
private:
    int tag_;
public:
    enum TokenNames {
        REM, INPUT, LET, PRINT, GOTO, IF, END,  // for reserved tokens
        ID, NUM, OP,                            // common tokens names
        BRACE,
        NONE                                    // signals that there are no
                                                // more tokens
    };

    explicit Token(int tag): tag_{tag}
    {
    }

    Token(const Token&) = default;
    Token(Token &&) = default;
    Token& operator=(const Token&) = default;
    virtual ~Token() = default;
    int get_tag(void) const;
    void set_tag(int new_tag);
};

#endif