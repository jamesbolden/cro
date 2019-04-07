#ifndef MP_LEXER_HH
#define MP_LEXER_HH

#include <string>
#include <fstream>
#include "mp/common.hh"
#include "mp/token.hh"

namespace mp {
    struct utf8 {
        utf8(unicode_t);
        ~utf8();

        unsigned length;
        char *data;
    };

    class lexer {
        unicode_t next_unicode();
    public:
        lexer(const std::string&);

        token lex();
        void set_state(state_t);
        std::string text() const;

    private:
        std::string m_File;
        std::ifstream m_Input;
        std::string m_Text;
        state_t m_State;
    };

    bool isspace(unicode_t);
    bool isascii(unicode_t);
    bool isdelim(unicode_t);
}

#endif // MP_LEXER_HH
