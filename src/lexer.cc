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
    utf8::utf8(unicode_t c) : length(0), data(nullptr) {
        if (c <= 0x7F) {
            length = 1;
            data = new char;
            *data = static_cast<char>(c);
        }
        else if (c <= 0x7FF) {
            length = 2;
            data = new char[2];
            data[0] = c >> 6;
            data[0] |= 0xC0;
            data[1] = c & 0x3F;
            data[1] |= 0x80;
        }
        else if (c <= 0xFFFF) {
            length = 3;
            data = new char[3];
            data[0] = c >> 12;
            data[0] |= 0xE0;
            data[1] = (c & 0xFC0) >> 6;
            data[1] |= 0x80;
            data[2] = c & 0x3F;
            data[2] |= 0x80;
        }
        else {
            length = 4;
            data = new char[4];
            data[0] = c >> 18;
            data[0] |= 0xF0;
            data[1] = (c & 0x3F000) >> 12;
            data[1] |= 0x80;
            data[2] = (c & 0xFC0) >> 6;
            data[2] |= 0x80;
            data[3] = c & 0x3F;
            data[3] |= 0x80;
        }
    }

    utf8::~utf8() {
        delete[] data;
    }

    lexer::lexer(const std::string &inp) : m_File(inp), m_Input(), m_Text(), m_State(STATE_NON_MACRO_TEXT) {
        m_Input.open(inp.c_str());
    }

    unicode_t lexer::next_unicode() {
        unicode_t result = 0;
        int next = m_Input.get();

        if (ASCII_CHAR(next))
            return static_cast<unicode_t>(next);
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
        std::ostringstream ss;
        bool complete = false;
        token result = token::END_OF_FILE;
        unicode_t next = next_unicode();

        if (next == LEFT_BRACKET_CODEPOINT) {
            m_State = STATE_MACRO_HEADING;
            return token::LEFT_BRACKET;
        }
        else if (next == RIGHT_BRACKET_CODEPOINT) {
            return token::RIGHT_BRACKET;
        }
        else if (m_State == STATE_MACRO_HEADING) {

            while (!complete) {

            }
        }
        else {
            if (next == EOF)
                return token::END_OF_FILE;
            for (; next != 0x2329; next = next_unicode()) {
                if (next == EOF) {
                    m_Text = ss.str();
                    return token::TEXT_BLOCK;
                }
                utf8 utf8(next);
                ss.write(utf8.data, utf8.length);
            }

            m_State = STATE_MACRO_HEADING;
            return token::LEFT_BRACKET;
        }

        return token::END_OF_FILE;
    }

    std::string lexer::text() const {
        return m_Text;
    }

    bool isspace(unicode_t c) {
        switch (c) {
            case 0x20:
            case 0x09:
            case 0x0A:
            case 0x0B:
            case 0x0C:
            case 0x0D:
                return true;
            default:
                return false;
        }
    }

    bool isascii(unicode_t c) {
        return c <= 0x7F;
    }

    bool isdelim(unicode_t c) {
        return isspace(c) || c == LEFT_BRACKET_CODEPOINT || c == RIGHT_BRACKET_CODEPOINT;
    }
}
