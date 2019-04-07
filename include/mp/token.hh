#ifndef MP_TOKEN_HH
#define MP_TOKEN_HH

namespace mp {
    enum class token {
        END_OF_FILE,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        KWD_DEFINE,
        PARAM,
        SYMBOL
    };
}

#endif // MP_TOKEN_HH
