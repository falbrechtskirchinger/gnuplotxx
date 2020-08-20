#include <cmath>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>

#include <gnuplotxx.h>

using namespace gnuplotxx;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto plot = Plot("Plot").range(Axes::Y2, -1.5, 1.5);
  auto series1 = plot.createSeries("x^2").with(PlotStyle::Lines);
  auto series2 = plot.createSeries("sin(2x)")
                     .with(PlotStyle::Lines)
                     .smooth(Smooth::CSplines)
                     .axes(Axes::X1Y2);

  std::vector<std::pair<double, double>> vxSq, vsin2X;

  for (double x = 0; x < 5; x += 0.1) {
    vxSq.emplace_back(x, x * x);
    vsin2X.emplace_back(x, std::sin(2 * x));
  }

  series1.append(vxSq.begin(), vxSq.end());
  series2.append(vsin2X.begin(), vsin2X.end());

  plot.update();

  std::cout << "Press ENTER to exit..." << std::endl;
  std::getchar();

  return 0;
}
