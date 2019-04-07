#include <cassert>
#include <cstdio>
#include <sstream>
#include "mp/lexer.hh"

#define ASCII_CHAR(C)           (!((C) & 0x80))
#define UTF8_TWO_BYTES(C)       (((C) >> 6) == 3)
#define UTF8_THREE_BYTES(C)     (((C) >> 5) == 7)
#define UTF8_FOUR_BYTES(C)      (((C) >> 4) == 0xF)

#define UTF8_FOLLOWING_BYTE(C)  ((C) >> 7)

#define LEFT_BRACKET_CODEPOINT  0x2329
#define RIGHT_BRACKET_CODEPOINT 0x232A

namespace mp {
    lexer::lexer(const std::string &inp) : m_File(inp), m_Input(), m_Symbol(), m_Lexing_macro(false) {
        m_Input.open(inp.c_str());
    }

    lexer::unicode_t lexer::nextUnicode() {
        lexer::unicode_t result = 0;
        int next = m_Input.get();

        if (ASCII_CHAR(next))
            return static_cast<lexer::unicode_t>(next);
        else if (UTF8_TWO_BYTES(next)) {
            result |= (next & 0x1F) << 6;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= next & 0x3F;

            return result;
        }
        else if (UTF8_THREE_BYTES(next)) {
            result |= (next & 0xF) << 12;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= (next & 0x3F) << 6;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= next & 0x3F;

            return result;
        }
        else if (UTF8_FOUR_BYTES(next)) {
            result |= (next & 7) << 18;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= (next & 0x3F) << 12;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= (next & 0x3F) << 6;

            next = m_Input.get();
            assert(UTF8_FOLLOWING_BYTE(next));
            result |= next & 0x3F;

            return result;
        }

        return 0;
    }

    token lexer::lex() {
        std::stringstream ss;
        bool complete = false;
        token result = token::END_OF_FILE;
        lexer::unicode_t next = nextUnicode();

        if (next == LEFT_BRACKET_CODEPOINT) {
            m_Lexing_macro = true;
            return token::LEFT_BRACKET;
        }
        else if (next == RIGHT_BRACKET_CODEPOINT) {
            m_Lexing_macro = false;
            return token::RIGHT_BRACKET;
        }
        else if (m_Lexing_macro) {
            

            while (!complete) {

            }
        }
        else {
            for (; next != 0x2329; next = nextUnicode()) {
                if (next == EOF)
                    return token::END_OF_FILE;
            }

            m_Lexing_macro = true;
            return token::LEFT_BRACKET;
        }

        return token::END_OF_FILE;
    }

    std::string lexer::symbol() const {
        return m_Symbol;
    }
}
