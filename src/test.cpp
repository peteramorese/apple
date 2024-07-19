#include "lemon/ArgParser.h"

using namespace LMN;

int main(int argc, char** argv) {
    LMN::ArgParser parser(argc, argv);

    Arg<ArgType::Indicator> test = parser.addDef<ArgType::Indicator>().flag('t').key("test").description("This is a test");
    Arg<ArgType::Indicator> beep = parser.addDef<ArgType::Indicator>().flag('b').key("beep").description("beep borp");
    Arg<ArgType::Value, int> val = parser.addDef<ArgType::Value, int>().flag('d').key("dhoom").description("an integerrr").defaultValue(4);
    Arg<ArgType::List, int> ll = parser.addDef<ArgType::List, int>().flag('l').key("my-list").description("a listerino :D").defaultList({1, 2, 3, 4, 5});
    Arg<ArgType::Value, int> val2 = parser.addDef<ArgType::Value, int>().key("goo").description("require meeee").required();

    parser.enableHelp();

    INFO("Value: " << val.value());
    INFO("has indicator? " << (bool)test);
    INFO("list: ");
    for (auto e : ll.list()) {
        PRINT(e);
    }
    return 0;
}