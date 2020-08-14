#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>

#include <chaiscript/chaiscript.hpp>

#include <gnuplotxx-chaiscript.h>
#include <gnuplotxx.h>

using namespace chaiscript;
using namespace gnuplotxx;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " SCRIPT_FILE" << std::endl;
    return -1;
  }

  ChaiScript chai;

  // Add gnuplotxx::library() ChaiScript module
  chai.add(library());

  std::mt19937 gen;
  chai.add(fun([&gen](int min, int max) {
             std::uniform_int_distribution dis(min, max);
             return dis(gen);
           }),
           "rand");
  chai.add(fun([&gen](double min, double max) {
             std::uniform_real_distribution dis(min, max);
             return dis(gen);
           }),
           "rand");
  chai.add(fun([](double x) { return std::sin(x); }), "sin");

  auto plot = Plot("Plot");
  chai.add_global(var(plot), "plot");

  chai.eval_file(argv[1]);

  plot.update();

  std::cout << "Press ENTER to exit..." << std::endl;
  std::getchar();

  return 0;
}
