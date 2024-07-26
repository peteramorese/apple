#include "lemon/ArgParser.h"
#include "lemon/Random.h"

using namespace lemon;

int main(int argc, char** argv) {
    lemon::ArgParser parser(argc, argv);

    Arg<ArgT::Check> test = parser.addDef<ArgT::Check>().flag('t').key("test").description("This is a test");
    Arg<ArgT::Check> beep = parser.addDef<ArgT::Check>().flag('b').key("beep").description("beep borp");
    Arg<ArgT::Value, int> val = parser.addDef<ArgT::Value, int>().flag('d').key("dhoom").description("an integerrr").defaultValue(2).options({1, 2, 3, 55});
    Arg<ArgT::List, int> ll = parser.addDef<ArgT::List, int>().flag('l').key("my-list").description("a listerino :D").defaultList({1, 2, 3, 4, 5});
    Arg<ArgT::Value, int> val2 = parser.addDef<ArgT::Value, int>().key("goo").description("uno");
    Arg<ArgT::Value, int> val3 = parser.addDef<ArgT::Value, int>().key("boo").description("dos");

    parser.enableHelp();

    INFO("Value: " << val.value());
    INFO("has indicator? " << (bool)test);
    INFO("list: ");
    for (auto e : ll.list()) {
        PRINT(e);
    }

    DEBUG("rng: " << RNG::nrandd());
    return 0;
}
