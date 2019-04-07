#include <string>
#include "mp/lexer.hh"

int main(int argc, char **argv) {
    mp::lexer lexer(argv[1]);

    while (lexer.lex() != mp::token::END_OF_FILE) { }

    return 0;
}
