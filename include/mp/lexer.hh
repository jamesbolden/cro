#ifndef MP_LEXER_HH
#define MP_LEXER_HH

#include <string>
#include <fstream>
#include "mp/token.hh"

namespace mp {
    class lexer {
        typedef unsigned unicode_t;

        unicode_t nextUnicode();
    public:
        lexer(const std::string&);

        token lex();
        std::string symbol() const;

    private:
        std::string m_File;
        std::ifstream m_Input;
        std::string m_Symbol;
        bool m_Lexing_macro;
    };
}

#endif // MP_LEXER_HH
