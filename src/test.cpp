#include "lemon/ArgParser.h"

using namespace LMN;

int main(int argc, char** argv) {
    LMN::ArgParser parser(argc, argv);

    Arg<ArgType::Indicator> test = parser.addDef<ArgType::Indicator>().flag('t').key("test").description("This is a test");
    return 0;
}