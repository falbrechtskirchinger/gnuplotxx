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

  // Generated via:
  // wget -qO -
  // "https://en.wikipedia.org/wiki/Special:ExpandTemplates?wpInput={{X11 color
  // chart}}" | sed 's/&amp;nbsp;//g' | awk '/\| colspan=/ { match($0,
  // /'"'''"'/); $0=substr($0, RSTART+3); match($0, /'"'''"'/); $0=substr($0, 0,
  // RSTART-1); print "// " $0} /\| ([A-Z][a-z]+)+ \|\|/ {  printf "colors[\""
  // $2 "\"] = const_var(Colors::" $2 ");\n"}'

  auto colors = dispatch::Dynamic_Object("Colors");
  // Pink colors
  colors["MediumVioletRed"] = const_var(Colors::MediumVioletRed);
  colors["DeepPink"] = const_var(Colors::DeepPink);
  colors["PaleVioletRed"] = const_var(Colors::PaleVioletRed);
  colors["HotPink"] = const_var(Colors::HotPink);
  colors["LightPink"] = const_var(Colors::LightPink);
  colors["Pink"] = const_var(Colors::Pink);
  // Red colors
  colors["DarkRed"] = const_var(Colors::DarkRed);
  colors["Red"] = const_var(Colors::Red);
  colors["Firebrick"] = const_var(Colors::Firebrick);
  colors["Crimson"] = const_var(Colors::Crimson);
  colors["IndianRed"] = const_var(Colors::IndianRed);
  colors["LightCoral"] = const_var(Colors::LightCoral);
  colors["Salmon"] = const_var(Colors::Salmon);
  colors["DarkSalmon"] = const_var(Colors::DarkSalmon);
  colors["LightSalmon"] = const_var(Colors::LightSalmon);
  // Orange colors
  colors["OrangeRed"] = const_var(Colors::OrangeRed);
  colors["Tomato"] = const_var(Colors::Tomato);
  colors["DarkOrange"] = const_var(Colors::DarkOrange);
  colors["Coral"] = const_var(Colors::Coral);
  colors["Orange"] = const_var(Colors::Orange);
  // Yellow colors
  colors["DarkKhaki"] = const_var(Colors::DarkKhaki);
  colors["Gold"] = const_var(Colors::Gold);
  colors["Khaki"] = const_var(Colors::Khaki);
  colors["PeachPuff"] = const_var(Colors::PeachPuff);
  colors["Yellow"] = const_var(Colors::Yellow);
  colors["PaleGoldenrod"] = const_var(Colors::PaleGoldenrod);
  colors["Moccasin"] = const_var(Colors::Moccasin);
  colors["PapayaWhip"] = const_var(Colors::PapayaWhip);
  colors["LightGoldenrodYellow"] = const_var(Colors::LightGoldenrodYellow);
  colors["LemonChiffon"] = const_var(Colors::LemonChiffon);
  colors["LightYellow"] = const_var(Colors::LightYellow);
  // Brown colors
  colors["Maroon"] = const_var(Colors::Maroon);
  colors["Brown"] = const_var(Colors::Brown);
  colors["SaddleBrown"] = const_var(Colors::SaddleBrown);
  colors["Sienna"] = const_var(Colors::Sienna);
  colors["Chocolate"] = const_var(Colors::Chocolate);
  colors["DarkGoldenrod"] = const_var(Colors::DarkGoldenrod);
  colors["Peru"] = const_var(Colors::Peru);
  colors["RosyBrown"] = const_var(Colors::RosyBrown);
  colors["Goldenrod"] = const_var(Colors::Goldenrod);
  colors["SandyBrown"] = const_var(Colors::SandyBrown);
  colors["Tan"] = const_var(Colors::Tan);
  colors["Burlywood"] = const_var(Colors::Burlywood);
  colors["Wheat"] = const_var(Colors::Wheat);
  colors["NavajoWhite"] = const_var(Colors::NavajoWhite);
  colors["Bisque"] = const_var(Colors::Bisque);
  colors["BlanchedAlmond"] = const_var(Colors::BlanchedAlmond);
  colors["Cornsilk"] = const_var(Colors::Cornsilk);
  // Green colors
  colors["DarkGreen"] = const_var(Colors::DarkGreen);
  colors["Green"] = const_var(Colors::Green);
  colors["DarkOliveGreen"] = const_var(Colors::DarkOliveGreen);
  colors["ForestGreen"] = const_var(Colors::ForestGreen);
  colors["SeaGreen"] = const_var(Colors::SeaGreen);
  colors["Olive"] = const_var(Colors::Olive);
  colors["OliveDrab"] = const_var(Colors::OliveDrab);
  colors["MediumSeaGreen"] = const_var(Colors::MediumSeaGreen);
  colors["LimeGreen"] = const_var(Colors::LimeGreen);
  colors["Lime"] = const_var(Colors::Lime);
  colors["SpringGreen"] = const_var(Colors::SpringGreen);
  colors["MediumSpringGreen"] = const_var(Colors::MediumSpringGreen);
  colors["DarkSeaGreen"] = const_var(Colors::DarkSeaGreen);
  colors["MediumAquamarine"] = const_var(Colors::MediumAquamarine);
  colors["YellowGreen"] = const_var(Colors::YellowGreen);
  colors["LawnGreen"] = const_var(Colors::LawnGreen);
  colors["Chartreuse"] = const_var(Colors::Chartreuse);
  colors["LightGreen"] = const_var(Colors::LightGreen);
  colors["GreenYellow"] = const_var(Colors::GreenYellow);
  colors["PaleGreen"] = const_var(Colors::PaleGreen);
  // Cyan colors
  colors["Teal"] = const_var(Colors::Teal);
  colors["DarkCyan"] = const_var(Colors::DarkCyan);
  colors["LightSeaGreen"] = const_var(Colors::LightSeaGreen);
  colors["CadetBlue"] = const_var(Colors::CadetBlue);
  colors["DarkTurquoise"] = const_var(Colors::DarkTurquoise);
  colors["MediumTurquoise"] = const_var(Colors::MediumTurquoise);
  colors["Turquoise"] = const_var(Colors::Turquoise);
  colors["Aqua"] = const_var(Colors::Aqua);
  colors["Cyan"] = const_var(Colors::Cyan);
  colors["Aquamarine"] = const_var(Colors::Aquamarine);
  colors["PaleTurquoise"] = const_var(Colors::PaleTurquoise);
  colors["LightCyan"] = const_var(Colors::LightCyan);
  // Blue colors
  colors["Navy"] = const_var(Colors::Navy);
  colors["DarkBlue"] = const_var(Colors::DarkBlue);
  colors["MediumBlue"] = const_var(Colors::MediumBlue);
  colors["Blue"] = const_var(Colors::Blue);
  colors["MidnightBlue"] = const_var(Colors::MidnightBlue);
  colors["RoyalBlue"] = const_var(Colors::RoyalBlue);
  colors["SteelBlue"] = const_var(Colors::SteelBlue);
  colors["DodgerBlue"] = const_var(Colors::DodgerBlue);
  colors["DeepSkyBlue"] = const_var(Colors::DeepSkyBlue);
  colors["CornflowerBlue"] = const_var(Colors::CornflowerBlue);
  colors["SkyBlue"] = const_var(Colors::SkyBlue);
  colors["LightSkyBlue"] = const_var(Colors::LightSkyBlue);
  colors["LightSteelBlue"] = const_var(Colors::LightSteelBlue);
  colors["LightBlue"] = const_var(Colors::LightBlue);
  colors["PowderBlue"] = const_var(Colors::PowderBlue);
  // Purple, violet, and magenta colors
  colors["Indigo"] = const_var(Colors::Indigo);
  colors["Purple"] = const_var(Colors::Purple);
  colors["DarkMagenta"] = const_var(Colors::DarkMagenta);
  colors["DarkViolet"] = const_var(Colors::DarkViolet);
  colors["DarkSlateBlue"] = const_var(Colors::DarkSlateBlue);
  colors["BlueViolet"] = const_var(Colors::BlueViolet);
  colors["DarkOrchid"] = const_var(Colors::DarkOrchid);
  colors["Fuchsia"] = const_var(Colors::Fuchsia);
  colors["Magenta"] = const_var(Colors::Magenta);
  colors["SlateBlue"] = const_var(Colors::SlateBlue);
  colors["MediumSlateBlue"] = const_var(Colors::MediumSlateBlue);
  colors["MediumOrchid"] = const_var(Colors::MediumOrchid);
  colors["MediumPurple"] = const_var(Colors::MediumPurple);
  colors["Orchid"] = const_var(Colors::Orchid);
  colors["Violet"] = const_var(Colors::Violet);
  colors["Plum"] = const_var(Colors::Plum);
  colors["Thistle"] = const_var(Colors::Thistle);
  colors["Lavender"] = const_var(Colors::Lavender);
  // White colors
  colors["MistyRose"] = const_var(Colors::MistyRose);
  colors["AntiqueWhite"] = const_var(Colors::AntiqueWhite);
  colors["Linen"] = const_var(Colors::Linen);
  colors["Beige"] = const_var(Colors::Beige);
  colors["WhiteSmoke"] = const_var(Colors::WhiteSmoke);
  colors["LavenderBlush"] = const_var(Colors::LavenderBlush);
  colors["OldLace"] = const_var(Colors::OldLace);
  colors["AliceBlue"] = const_var(Colors::AliceBlue);
  colors["Seashell"] = const_var(Colors::Seashell);
  colors["GhostWhite"] = const_var(Colors::GhostWhite);
  colors["Honeydew"] = const_var(Colors::Honeydew);
  colors["FloralWhite"] = const_var(Colors::FloralWhite);
  colors["Azure"] = const_var(Colors::Azure);
  colors["MintCream"] = const_var(Colors::MintCream);
  colors["Snow"] = const_var(Colors::Snow);
  colors["Ivory"] = const_var(Colors::Ivory);
  colors["White"] = const_var(Colors::White);
  // Gray and black colors
  colors["Black"] = const_var(Colors::Black);
  colors["DarkSlateGray"] = const_var(Colors::DarkSlateGray);
  colors["DimGray"] = const_var(Colors::DimGray);
  colors["SlateGray"] = const_var(Colors::SlateGray);
  colors["Gray"] = const_var(Colors::Gray);
  colors["LightSlateGray"] = const_var(Colors::LightSlateGray);
  colors["DarkGray"] = const_var(Colors::DarkGray);
  colors["Silver"] = const_var(Colors::Silver);
  colors["LightGray"] = const_var(Colors::LightGray);
  colors["Gainsboro"] = const_var(Colors::Gainsboro);
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
          return self.with(plotStyle);
        }),
        "with"},
       {fun<PlotStyle, Series>(&Series::with), "with"},
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
