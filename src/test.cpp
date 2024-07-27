#include "lemon/ArgParser.h"
#include "lemon/Random.h"

using namespace lemon;

int main(int argc, char** argv) {
	lemon::ArgParser parser(argc, argv);
    lemon::Arg<lemon::ArgT::Check> verbose = parser.addDef<lemon::ArgT::Check>().flag('v').key("Verbose");
    lemon::Arg<lemon::ArgT::Check> non_convex = parser.addDef<lemon::ArgT::Check>().key("non-conv").description("Solve the non-convex synthesis problem (default to convex)");
    lemon::Arg<lemon::ArgT::Check> adaptive = parser.addDef<lemon::ArgT::Check>().flag('a').description("Use the adaptive subdivision algorithm");
    lemon::Arg<lemon::ArgT::Check> export_matrices = parser.addDef<lemon::ArgT::Check>().flag('e').description("Export the matrices to use external solvers");
    lemon::Arg<lemon::ArgT::Value, std::string> filter = parser.addDef<lemon::ArgT::Value, std::string>().flag('f').key("filter").description("Select which filter to use").options({"diagdeg", "oddsum"});
    lemon::Arg<lemon::ArgT::Value, std::string> solver_id = parser.addDef<lemon::ArgT::Value, std::string>().key("solver").description("Solver ID").defaultValue("SCIP");
    lemon::Arg<lemon::ArgT::Value, std::string> dynamics_type = parser.addDef<lemon::ArgT::Value, std::string>().key("dynamics-type").description("Type of dynamics").options({"to_origin", "random"});
    lemon::Arg<lemon::ArgT::Value, int> dynamics_deg = parser.addDef<lemon::ArgT::Value, int>().key("dynamics-deg").defaultValue(1l).description("Degree of dynamics (e.g. 1 is linear, 2 is quadratic)");
    lemon::Arg<lemon::ArgT::Value, int> barrier_deg = parser.addDef<lemon::ArgT::Value, int>().flag('d').key("deg").description("Barrier degree").required();
    lemon::Arg<lemon::ArgT::Value, int> deg_increase = parser.addDef<lemon::ArgT::Value, int>().flag('i').key("deg-inc").defaultValue(0l).description("Barrier degree increase");
    lemon::Arg<lemon::ArgT::Value, int> subd = parser.addDef<lemon::ArgT::Value, int>().key("subd").flag('s').defaultValue(0l).description("Barrier subdivision");
    lemon::Arg<lemon::ArgT::Value, int> ada_iters = parser.addDef<lemon::ArgT::Value, int>().key("ada-iters").defaultValue(1l).description("Number of adaptive subdivision iterations (ONLY FOR ADAPTIVE)");
    lemon::Arg<lemon::ArgT::Value, int> ada_max_subdiv = parser.addDef<lemon::ArgT::Value, int>().key("ada-max-subdiv").defaultValue(2l).description("Max number of sets divided each iteration (ONLY FOR ADAPTIVE)");
    lemon::Arg<lemon::ArgT::Value, int> time_steps = parser.addDef<lemon::ArgT::Value, int>().key("ts").flag('t').defaultValue(10l).description("Number of time steps");
    parser.enableHelp();

    return 0;
}
