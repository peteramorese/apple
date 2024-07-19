#include "lemon/ArgParser.h"

using namespace LMN;

int main(int argc, char** argv) {
    LMN::ArgParser parser(argc, argv);

    Arg<ArgType::Indicator> test = parser.addDef<ArgType::Indicator>().flag('t').key("test").description("This is a test");
    Arg<ArgType::Indicator> beep = parser.addDef<ArgType::Indicator>().flag('b').key("beep").description("beep borp");
    Arg<ArgType::Value, int> val = parser.addDef<ArgType::Value, int>().flag('d').key("dhoom").description("an integerrr").defaultValue(4);

    parser.enableHelp();

    INFO("Value: " << val.value());
    INFO("has indicator? " << (bool)test);
    return 0;
}