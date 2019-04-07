#include <string>
#include "mp/lexer.hh"

int main(int argc, char **argv) {
    mp::lexer lexer(std::string(argv[1]));

    while (lexer.lex() != token::END_OF_FILE) { }

    return 0;
}
