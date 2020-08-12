#include <cstdio>
#include <iostream>

#include <gnuplotxx.h>

using namespace gnuplotxx;

int main(int argc, char *argv[]) {
  auto plot = Plot("Plot");
  auto series = plot.createSeries("x^2");

  for (double x = 0; x < 5; x += 0.1)
    series.append(x, x * x);

  plot.update();

  std::cout << "Press ENTER to exit..." << std::endl;
  std::getchar();

  return 0;
}
