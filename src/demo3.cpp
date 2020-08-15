#include <cmath>
#include <cstdio>
#include <iostream>

#include <gnuplotxx.h>

using namespace gnuplotxx;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto plot = Plot("Plot").range(Axes::Y2, -1.5, 1.5);
  auto series1 = plot.createSeries("x^2").plotStyle(PlotStyle::Lines);
  auto series2 = plot.createSeries("sin(5x)")
                     .plotStyle(PlotStyle::Lines)
                     .smooth(Smooth::CSplines)
                     .axes(Axes::X1Y2);

  for (double x = 0; x < 5; x += 0.1) {
    series1.append(x, x * x);
    series2.append(x, std::sin(2 * x));
  }

  plot.update();

  std::cout << "Press ENTER to exit..." << std::endl;
  std::getchar();

  return 0;
}
