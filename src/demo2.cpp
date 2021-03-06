#include <cstdio>
#include <iostream>
#include <random>

#include <gnuplotxx.h>

using namespace gnuplotxx;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto plot = Plot("Plot").font("Arial", 10);

  auto series1 = plot.createSeries("Random Walk")
                     .with(PlotStyle::Lines)
                     .color(Colors::SteelBlue);
  auto series2 = plot.createSeries("Random Walk (Smoothed)", series1)
                     .with(PlotStyle::Lines)
                     .lineWidth(5.0)
                     .color(Colors::Red.fade(0.5))
                     .smooth(Smooth::Bezier);

  std::mt19937 gen;
  std::uniform_int_distribution dis(0, 1);
  for (int x = 0, y = 0; x < 1000; ++x, y += (dis(gen)) ? 1 : -1)
    series1.append(x, y);

  plot.update();

  std::cout << "Press ENTER to exit..." << std::endl;
  std::getchar();

  return 0;
}
