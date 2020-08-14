// Copyright 2020 Florian Albrechtskirchinger
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <chaiscript/dispatchkit/bootstrap_stl.hpp>
#include <chaiscript/dispatchkit/dispatchkit.hpp>
#include <chaiscript/utility/utility.hpp>

#include "gnuplotxx.h"

namespace gnuplotxx {

inline chaiscript::ModulePtr library() {
  using namespace chaiscript;
  auto lib = std::make_shared<Module>();

  utility::add_class<Color>(
      *lib, "Color",
      {constructor<Color()>(), constructor<Color(int)>(),
       constructor<Color(int, int, int)>(),
       constructor<Color(int, int, int, int)>(),
       constructor<Color(double, double, double)>(),
       constructor<Color(double, double, double, double)>(),
       constructor<Color(const Color &)>()},
      {{fun(&Color::red), "red"},
       {fun(&Color::green), "green"},
       {fun(&Color::blue), "blue"},
       {fun(&Color::alpha), "alpha"},
       {fun(&Color::rgba), "rgba"},
       {fun<Color, Color, int>(&Color::fade), "fade"},
       {fun<Color, Color, double>(&Color::fade), "fade"}});

  /* Generated via:
   * gnuplot <<<"show colors"  |& tail -n+2 | \
   *   awk '/^ /{ \
   *     c = ""; \
   *     n = split($1, a, "-"); \
   *     for(i=1; i<=n; ++i) \
   *       c = c sprintf("%s%s", toupper(substr(a[i], 1, 1)), \
   *                             substr(a[i], 2)); \
   *     printf("colors[\"%s\"] = const_var(Colors::%s);\n", c, c); }'
   */

  auto colors = dispatch::Dynamic_Object("Colors");
  colors["White"] = const_var(Colors::White);
  colors["Black"] = const_var(Colors::Black);
  colors["DarkGrey"] = const_var(Colors::DarkGrey);
  colors["Red"] = const_var(Colors::Red);
  colors["WebGreen"] = const_var(Colors::WebGreen);
  colors["WebBlue"] = const_var(Colors::WebBlue);
  colors["DarkMagenta"] = const_var(Colors::DarkMagenta);
  colors["DarkCyan"] = const_var(Colors::DarkCyan);
  colors["DarkOrange"] = const_var(Colors::DarkOrange);
  colors["DarkYellow"] = const_var(Colors::DarkYellow);
  colors["Royalblue"] = const_var(Colors::Royalblue);
  colors["Goldenrod"] = const_var(Colors::Goldenrod);
  colors["DarkSpringGreen"] = const_var(Colors::DarkSpringGreen);
  colors["Purple"] = const_var(Colors::Purple);
  colors["Steelblue"] = const_var(Colors::Steelblue);
  colors["DarkRed"] = const_var(Colors::DarkRed);
  colors["DarkChartreuse"] = const_var(Colors::DarkChartreuse);
  colors["Orchid"] = const_var(Colors::Orchid);
  colors["Aquamarine"] = const_var(Colors::Aquamarine);
  colors["Brown"] = const_var(Colors::Brown);
  colors["Yellow"] = const_var(Colors::Yellow);
  colors["Turquoise"] = const_var(Colors::Turquoise);
  colors["Grey0"] = const_var(Colors::Grey0);
  colors["Grey10"] = const_var(Colors::Grey10);
  colors["Grey20"] = const_var(Colors::Grey20);
  colors["Grey30"] = const_var(Colors::Grey30);
  colors["Grey40"] = const_var(Colors::Grey40);
  colors["Grey50"] = const_var(Colors::Grey50);
  colors["Grey60"] = const_var(Colors::Grey60);
  colors["Grey70"] = const_var(Colors::Grey70);
  colors["Grey"] = const_var(Colors::Grey);
  colors["Grey80"] = const_var(Colors::Grey80);
  colors["Grey90"] = const_var(Colors::Grey90);
  colors["Grey100"] = const_var(Colors::Grey100);
  colors["LightRed"] = const_var(Colors::LightRed);
  colors["LightGreen"] = const_var(Colors::LightGreen);
  colors["LightBlue"] = const_var(Colors::LightBlue);
  colors["LightMagenta"] = const_var(Colors::LightMagenta);
  colors["LightCyan"] = const_var(Colors::LightCyan);
  colors["LightGoldenrod"] = const_var(Colors::LightGoldenrod);
  colors["LightPink"] = const_var(Colors::LightPink);
  colors["LightTurquoise"] = const_var(Colors::LightTurquoise);
  colors["Gold"] = const_var(Colors::Gold);
  colors["Green"] = const_var(Colors::Green);
  colors["DarkGreen"] = const_var(Colors::DarkGreen);
  colors["SpringGreen"] = const_var(Colors::SpringGreen);
  colors["ForestGreen"] = const_var(Colors::ForestGreen);
  colors["SeaGreen"] = const_var(Colors::SeaGreen);
  colors["Blue"] = const_var(Colors::Blue);
  colors["DarkBlue"] = const_var(Colors::DarkBlue);
  colors["MidnightBlue"] = const_var(Colors::MidnightBlue);
  colors["Navy"] = const_var(Colors::Navy);
  colors["MediumBlue"] = const_var(Colors::MediumBlue);
  colors["Skyblue"] = const_var(Colors::Skyblue);
  colors["Cyan"] = const_var(Colors::Cyan);
  colors["Magenta"] = const_var(Colors::Magenta);
  colors["DarkTurquoise"] = const_var(Colors::DarkTurquoise);
  colors["DarkPink"] = const_var(Colors::DarkPink);
  colors["Coral"] = const_var(Colors::Coral);
  colors["LightCoral"] = const_var(Colors::LightCoral);
  colors["OrangeRed"] = const_var(Colors::OrangeRed);
  colors["Salmon"] = const_var(Colors::Salmon);
  colors["DarkSalmon"] = const_var(Colors::DarkSalmon);
  colors["Khaki"] = const_var(Colors::Khaki);
  colors["DarkKhaki"] = const_var(Colors::DarkKhaki);
  colors["DarkGoldenrod"] = const_var(Colors::DarkGoldenrod);
  colors["Beige"] = const_var(Colors::Beige);
  colors["Olive"] = const_var(Colors::Olive);
  colors["Orange"] = const_var(Colors::Orange);
  colors["Violet"] = const_var(Colors::Violet);
  colors["DarkViolet"] = const_var(Colors::DarkViolet);
  colors["Plum"] = const_var(Colors::Plum);
  colors["DarkPlum"] = const_var(Colors::DarkPlum);
  colors["DarkOlivegreen"] = const_var(Colors::DarkOlivegreen);
  colors["Orangered4"] = const_var(Colors::Orangered4);
  colors["Brown4"] = const_var(Colors::Brown4);
  colors["Sienna4"] = const_var(Colors::Sienna4);
  colors["Orchid4"] = const_var(Colors::Orchid4);
  colors["Mediumpurple3"] = const_var(Colors::Mediumpurple3);
  colors["Slateblue1"] = const_var(Colors::Slateblue1);
  colors["Yellow4"] = const_var(Colors::Yellow4);
  colors["Sienna1"] = const_var(Colors::Sienna1);
  colors["Tan1"] = const_var(Colors::Tan1);
  colors["Sandybrown"] = const_var(Colors::Sandybrown);
  colors["LightSalmon"] = const_var(Colors::LightSalmon);
  colors["Pink"] = const_var(Colors::Pink);
  colors["Khaki1"] = const_var(Colors::Khaki1);
  colors["Lemonchiffon"] = const_var(Colors::Lemonchiffon);
  colors["Bisque"] = const_var(Colors::Bisque);
  colors["Honeydew"] = const_var(Colors::Honeydew);
  colors["Slategrey"] = const_var(Colors::Slategrey);
  colors["Seagreen"] = const_var(Colors::Seagreen);
  colors["Antiquewhite"] = const_var(Colors::Antiquewhite);
  colors["Chartreuse"] = const_var(Colors::Chartreuse);
  colors["Greenyellow"] = const_var(Colors::Greenyellow);
  colors["Gray"] = const_var(Colors::Gray);
  colors["LightGray"] = const_var(Colors::LightGray);
  colors["LightGrey"] = const_var(Colors::LightGrey);
  colors["DarkGray"] = const_var(Colors::DarkGray);
  colors["Slategray"] = const_var(Colors::Slategray);
  colors["Gray0"] = const_var(Colors::Gray0);
  colors["Gray10"] = const_var(Colors::Gray10);
  colors["Gray20"] = const_var(Colors::Gray20);
  colors["Gray30"] = const_var(Colors::Gray30);
  colors["Gray40"] = const_var(Colors::Gray40);
  colors["Gray50"] = const_var(Colors::Gray50);
  colors["Gray60"] = const_var(Colors::Gray60);
  colors["Gray70"] = const_var(Colors::Gray70);
  colors["Gray80"] = const_var(Colors::Gray80);
  colors["Gray90"] = const_var(Colors::Gray90);
  colors["Gray100"] = const_var(Colors::Gray100);
  lib->add_global_const(const_var(colors), "Colors");

  // Generated via:
  // e=ENUM; while read x; do echo "{${e}::${x/,}, \"${e}_${x/,}\"},"; done

  utility::add_class<PlotStyle>(
      *lib, "PlotStyle",
      {{PlotStyle::Points, "PlotStyle_Points"},
       {PlotStyle::Dots, "PlotStyle_Dots"},
       {PlotStyle::Lines, "PlotStyle_Lines"},
       {PlotStyle::LinesPoints, "PlotStyle_LinesPoints"},
       {PlotStyle::Steps, "PlotStyle_Steps"},
       {PlotStyle::FSteps, "PlotStyle_FSteps"},
       {PlotStyle::HiSteps, "PlotStyle_HiSteps"},
       {PlotStyle::Impulses, "PlotStyle_Impulses"},
       {PlotStyle::Boxes, "PlotStyle_Boxes"}});

  // Generated via:
  // e=ENUM; while read x; do echo "${e,}s[\"${x/,}\"] =
  // const_var(${e}::${x/,});"; done

  auto plotStyles = dispatch::Dynamic_Object("PlotStyles");
  plotStyles["Points"] = const_var(PlotStyle::Points);
  plotStyles["Dots"] = const_var(PlotStyle::Dots);
  plotStyles["Lines"] = const_var(PlotStyle::Lines);
  plotStyles["LinesPoints"] = const_var(PlotStyle::LinesPoints);
  plotStyles["Steps"] = const_var(PlotStyle::Steps);
  plotStyles["FSteps"] = const_var(PlotStyle::FSteps);
  plotStyles["HiSteps"] = const_var(PlotStyle::HiSteps);
  plotStyles["Impulses"] = const_var(PlotStyle::Impulses);
  plotStyles["Boxes"] = const_var(PlotStyle::Boxes);
  lib->add_global_const(const_var(plotStyles), "PlotStyles");

  utility::add_class<PointType>(
      *lib, "PointType",
      {{PointType::Cross, "PointType_Cross"},
       {PointType::X, "PointType_X"},
       {PointType::Asterisk, "PointType_Asterisk"},
       {PointType::Star, "PointType_Star"},
       {PointType::Box, "PointType_Box"},
       {PointType::FilledBox, "PointType_FilledBox"},
       {PointType::Circle, "PointType_Circle"},
       {PointType::FilledCircle, "PointType_FilledCircle"},
       {PointType::Triangle, "PointType_Triangle"},
       {PointType::FilledTriangle, "PointType_FilledTriangle"},
       {PointType::UpsideDownTriangle, "PointType_UpsideDownTriangle"},
       {PointType::FilledUpsideDownTriangle,
        "PointType_FilledUpsideDownTriangle"},
       {PointType::Diamond, "PointType_Diamond"},
       {PointType::FilledDiamond, "PointType_FilledDiamond"},
       {PointType::Pentagon, "PointType_Pentagon"},
       {PointType::FilledPentagon, "PointType_FilledPentagon"}});

  auto pointTypes = dispatch::Dynamic_Object("PointTypes");
  pointTypes["Cross"] = const_var(PointType::Cross);
  pointTypes["X"] = const_var(PointType::X);
  pointTypes["Asterisk"] = const_var(PointType::Asterisk);
  pointTypes["Star"] = const_var(PointType::Star);
  pointTypes["Box"] = const_var(PointType::Box);
  pointTypes["FilledBox"] = const_var(PointType::FilledBox);
  pointTypes["Circle"] = const_var(PointType::Circle);
  pointTypes["FilledCircle"] = const_var(PointType::FilledCircle);
  pointTypes["Triangle"] = const_var(PointType::Triangle);
  pointTypes["FilledTriangle"] = const_var(PointType::FilledTriangle);
  pointTypes["UpsideDownTriangle"] = const_var(PointType::UpsideDownTriangle);
  pointTypes["FilledUpsideDownTriangle"] =
      const_var(PointType::FilledUpsideDownTriangle);
  pointTypes["Diamond"] = const_var(PointType::Diamond);
  pointTypes["FilledDiamond"] = const_var(PointType::FilledDiamond);
  pointTypes["Pentagon"] = const_var(PointType::Pentagon);
  pointTypes["FilledPentagon"] = const_var(PointType::FilledPentagon);
  lib->add_global_const(const_var(pointTypes), "PointTypes");

  utility::add_class<Smooth>(*lib, "Smooth",
                             {{Smooth::Unique, "Smooth_Unique"},
                              {Smooth::Frequency, "Smooth_Frequency"},
                              {Smooth::FNormal, "Smooth_FNormal"},
                              {Smooth::Cumulative, "Smooth_Cumulative"},
                              {Smooth::CNormal, "Smooth_CNormal"},
                              {Smooth::Bins, "Smooth_Bins"},
                              {Smooth::KDensity, "Smooth_KDensity"},
                              {Smooth::CSplines, "Smooth_CSplines"},
                              {Smooth::ACSplines, "Smooth_ACSplines"},
                              {Smooth::MCSplines, "Smooth_MCSplines"},
                              {Smooth::Bezier, "Smooth_Bezier"},
                              {Smooth::SBezier, "Smooth_SBezier"},
                              {Smooth::Unwrap, "Smooth_Unwrap"}});

  auto smooths = dispatch::Dynamic_Object("Smooths");
  smooths["Unique"] = const_var(Smooth::Unique);
  smooths["Frequency"] = const_var(Smooth::Frequency);
  smooths["FNormal"] = const_var(Smooth::FNormal);
  smooths["Cumulative"] = const_var(Smooth::Cumulative);
  smooths["CNormal"] = const_var(Smooth::CNormal);
  smooths["Bins"] = const_var(Smooth::Bins);
  smooths["KDensity"] = const_var(Smooth::KDensity);
  smooths["CSplines"] = const_var(Smooth::CSplines);
  smooths["ACSplines"] = const_var(Smooth::ACSplines);
  smooths["MCSplines"] = const_var(Smooth::MCSplines);
  smooths["Bezier"] = const_var(Smooth::Bezier);
  smooths["SBezier"] = const_var(Smooth::SBezier);
  smooths["Unwrap"] = const_var(Smooth::Unwrap);
  lib->add_global_const(const_var(smooths), "Smooths");

  bootstrap::standard_library::pair_type<Point>("Point", *lib);
  bootstrap::standard_library::pair_type<Size>("Size", *lib);

  utility::add_class<std::optional<Point>>(
      *lib, "OptionalPoint",
      {constructor<std::optional<Point>()>(),
       constructor<std::optional<Point>(Point)>(),
       constructor<std::optional<Point>(const std::optional<Point> &)>()},
      {{fun(&std::optional<Point>::has_value), "has_value"},
       {fun([](const std::optional<Point> &self) -> Point {
          return self.value();
        }),
        "value"}});

  utility::add_class<std::optional<Size>>(
      *lib, "OptionalSize",
      {constructor<std::optional<Size>()>(),
       constructor<std::optional<Size>(Size)>(),
       constructor<std::optional<Size>(const std::optional<Size> &)>()},
      {{fun(&std::optional<Size>::has_value), "has_value"},
       {fun([](const std::optional<Size> &self) -> Size {
          return self.value();
        }),
        "value"}});

  lib->add(user_type<AutoType>(), "AutoType");
  lib->add_global_const(const_var(Auto), "Auto");

  utility::add_class<LimitValue>(
      *lib, "PlotLimitValue",
      {constructor<LimitValue(AutoType)>(), constructor<LimitValue(double)>(),
       constructor<LimitValue(const LimitValue &)>()},
      {});

  lib->add(type_conversion<AutoType, LimitValue>());
  lib->add(type_conversion<int, LimitValue>());
  lib->add(type_conversion<double, LimitValue>());

  lib->add(fun([](const AutoType &, double value) -> Limit {
             return Limit(Auto, value);
           }),
           "<");
  lib->add(fun([](double value, const AutoType &) -> Limit {
             return Limit(value, Auto);
           }),
           "<");

  utility::add_class<RangeValue>(
      *lib, "PlotRangeValue",
      {constructor<RangeValue(AutoType)>(), constructor<RangeValue(double)>(),
       constructor<RangeValue(Limit)>(),
       constructor<RangeValue(const RangeValue &)>()},
      {});

  lib->add(type_conversion<AutoType, RangeValue>());
  lib->add(type_conversion<int, RangeValue>());
  lib->add(type_conversion<double, RangeValue>());
  lib->add(type_conversion<Limit, RangeValue>());

  bootstrap::standard_library::pair_type<Limit>("PlotLimit", *lib);
  bootstrap::standard_library::pair_type<Range>("PlotRange", *lib);

  utility::add_class<Abscissa>(
      *lib, "Abscissa",
      {{Abscissa::X1, "Abscissa_X1"}, {Abscissa::X2, "Abscissa_X2"}});

  utility::add_class<Ordinate>(
      *lib, "Ordinate",
      {{Ordinate::Y1, "Ordinate_Y1"}, {Ordinate::Y2, "Ordinate_Y2"}});

  utility::add_class<Axis>(*lib, "Axis",
                           {constructor<Axis(Abscissa)>(),
                            constructor<Axis(Ordinate)>(),
                            constructor<Axis(const Axis &)>()},
                           {});

  lib->add(type_conversion<Abscissa, Axis>());
  lib->add(type_conversion<Ordinate, Axis>());

  utility::add_class<AxesPair>(*lib, "AxesPair",
                               {constructor<AxesPair(const AxesPair &)>()}, {});

  auto axes = dispatch::Dynamic_Object("Axes");
  axes["X1"] = const_var(Axes::X1);
  axes["X2"] = const_var(Axes::X2);
  axes["Y1"] = const_var(Axes::Y1);
  axes["Y2"] = const_var(Axes::Y2);
  axes["X1Y1"] = const_var(Axes::X1Y1);
  axes["X1Y2"] = const_var(Axes::X1Y2);
  axes["X2Y1"] = const_var(Axes::X2Y1);
  axes["X2Y2"] = const_var(Axes::X2Y2);
  axes["Count"] = const_var(Axes::Count);
  lib->add_global_const(const_var(axes), "Axes");

  utility::add_class<std::optional<Color>>(
      *lib, "OptionalColor",
      {constructor<std::optional<Color>()>(),
       constructor<std::optional<Color>(Color)>(),
       constructor<std::optional<Color>(const std::optional<Color> &)>()},
      {{fun(&std::optional<Color>::has_value), "has_value"},
       {fun([](const std::optional<Color> &self) -> Color {
          return self.value();
        }),
        "value"}});

  utility::add_class<std::optional<PointType>>(
      *lib, "OptionalPointType",
      {constructor<std::optional<PointType>()>(),
       constructor<std::optional<PointType>(PointType)>(),
       constructor<std::optional<PointType>(
           const std::optional<PointType> &)>()},
      {{fun(&std::optional<PointType>::has_value), "has_value"},
       {fun([](const std::optional<PointType> &self) -> PointType {
          return self.value();
        }),
        "value"}});

  utility::add_class<std::optional<Smooth>>(
      *lib, "OptionalSmooth",
      {constructor<std::optional<Smooth>()>(),
       constructor<std::optional<Smooth>(Smooth)>(),
       constructor<std::optional<Smooth>(const std::optional<Smooth> &)>()},
      {{fun(&std::optional<Smooth>::has_value), "has_value"},
       {fun([](const std::optional<Smooth> &self) -> Smooth {
          return self.value();
        }),
        "value"}});

  utility::add_class<Series>(
      *lib, "Series",
      {constructor<Series(const Series &)>(), constructor<Series(Series &&)>()},
      {{fun<int, Series>(&Series::dataId), "dataId"},
       {fun([](Series &self, std::string title) -> Series {
          return self.title(std::move(title));
        }),
        "title"},
       {fun<const std::string &, Series>(&Series::title), "title"},
       {fun([](Series &self, PlotStyle plotStyle) -> Series {
          return self.plotStyle(plotStyle);
        }),
        "plotStyle"},
       {fun<PlotStyle, Series>(&Series::plotStyle), "plotStyle"},
       {fun([](Series &self, std::optional<Color> color) -> Series {
          return self.color(color);
        }),
        "color"},
       {fun([](Series &self, Color color) -> Series {
          return self.color(color);
        }),
        "color"},
       {fun<std::optional<Color>, Series>(&Series::color), "color"},
       {fun([](Series &self, double lineWidth) -> Series {
          return self.lineWidth(lineWidth);
        }),
        "lineWidth"},
       {fun<double, Series>(&Series::lineWidth), "lineWidth"},
       {fun([](Series &self, std::optional<PointType> pointType) -> Series {
          return self.pointType(pointType);
        }),
        "pointType"},
       {fun([](Series &self, PointType pointType) -> Series {
          return self.pointType(pointType);
        }),
        "pointType"},
       {fun<std::optional<PointType>, Series>(&Series::pointType), "pointType"},
       {fun([](Series &self, double pointSize) -> Series {
          return self.pointSize(pointSize);
        }),
        "pointSize"},
       {fun<double, Series>(&Series::pointSize), "pointSize"},
       {fun([](Series &self, std::optional<Smooth> smooth) -> Series {
          return self.smooth(smooth);
        }),
        "smooth"},
       {fun([](Series &self, Smooth smooth) -> Series {
          return self.smooth(smooth);
        }),
        "smooth"},
       {fun<std::optional<Smooth>, Series>(&Series::smooth), "smooth"},
       {fun([](Series &self, AxesPair axes) -> Series {
          return self.axes(axes);
        }),
        "axes"},
       {fun<AxesPair, Series>(&Series::axes), "axes"},
       {fun([](Series &self, double v0, double v1) -> Series {
          return self.append(v0, v1);
        }),
        "append"},
       {fun([](Series &self, double v0, double v1, double v2) -> Series {
          return self.append(v0, v1, v2);
        }),
        "append"},
       {fun([](Series &self, double v0, double v1, double v2,
               double v3) -> Series { return self.append(v0, v1, v2, v3); }),
        "append"},
       {fun([](Series &self, double v0, double v1, double v2, double v3,
               double v4) -> Series {
          return self.append(v0, v1, v2, v3, v4);
        }),
        "append"},
       {fun([](Series &self, double v0, double v1, double v2, double v3,
               double v4, double v5) -> Series {
          return self.append(v0, v1, v2, v3, v4, v5);
        }),
        "append"},
       {fun([](Series &self, double v0, double v1, double v2, double v3,
               double v4, double v5, double v6) -> Series {
          return self.append(v0, v1, v2, v3, v4, v5, v6);
        }),
        "append"},
       {fun([](Series &self) -> Series { return self.clear(); }), "clear"}});

  lib->add(type_conversion<Series, bool>());

  utility::add_class<Plot>(
      *lib, "Plot",
      {constructor<Plot(std::string)>(), constructor<Plot(const Plot &)>(),
       constructor<Plot(Plot &&)>()},
      {{fun([](Plot &self, std::string title) -> Plot {
          return self.title(std::move(title));
        }),
        "title"},
       {fun<const std::string &, Plot>(&Plot::title), "title"},
       {fun([](Plot &self, std::string font) -> Plot {
          return self.font(std::move(font));
        }),
        "font"},
       {fun([](Plot &self, std::string font, int fontSize) -> Plot {
          return self.font(std::move(font), fontSize);
        }),
        "font"},
       {fun<const std::string &, Plot>(&Plot::font), "font"},

       {fun([](Plot &self, int fontSize) -> Plot {
          return self.fontSize(fontSize);
        }),
        "fontSize"},
       {fun<int, Plot>(&Plot::fontSize), "fontSize"},
       {fun([](Plot &self, std::optional<Point> position) -> Plot {
          return self.position(position);
        }),
        "position"},
       {fun([](Plot &self, Point position) -> Plot {
          return self.position(position);
        }),
        "position"},
       {fun([](Plot &self, int x, int y) -> Plot {
          return self.position(x, y);
        }),
        "position"},
       {fun<std::optional<Point>, Plot>(&Plot::position), "position"},
       {fun([](Plot &self, std::optional<Size> size) -> Plot {
          return self.size(size);
        }),
        "size"},
       {fun([](Plot &self, Size size) -> Plot { return self.size(size); }),
        "size"},
       {fun([](Plot &self, int width, int height) -> Plot {
          return self.size(width, height);
        }),
        "size"},
       {fun<std::optional<Size>, Plot>(&Plot::size), "size"},
       {fun([](Plot &self, Axis axis, const Range &range) -> Plot {
          return self.range(axis, range);
        }),
        "range"},
       {fun([](Plot &self, Axis axis, const RangeValue &min,
               const RangeValue &max) -> Plot {
          return self.range(axis, min, max);
        }),
        "range"},
       {fun<const Range &, Plot, Axis>(&Plot::range), "range"},
       {fun<Series, Plot, std::string>(&Plot::createSeries), "createSeries"},
       {fun<Series, Plot, std::string, const Series &>(&Plot::createSeries),
        "createSeries"},
       {fun(&Plot::numSeries), "numSeries"},
       {fun(&Plot::getSeries), "getSeries"},
       {fun(&Plot::findSeries), "findSeries"},
       {fun(&Plot::removeSeries), "removeSeries"},
       {fun(&Plot::update), "update"}});

  lib->add(type_conversion<Plot, bool>());

  return lib;
}

} // namespace gnuplotxx
