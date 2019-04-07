#ifndef MP_TOKEN_HH
#define MP_TOKEN_HH

namespace mp {
    enum class token {
        END_OF_FILE,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        COLON,
        KWD_DEFINE,
        PARAM,
        SYMBOL,
        TEXT_BLOCK
    };
}

#endif // MP_TOKEN_HH
