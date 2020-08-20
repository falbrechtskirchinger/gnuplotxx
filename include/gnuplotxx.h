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

#include <array>
#include <cassert>
#include <concepts>
#include <memory>
#include <optional>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(__has_include) && __has_include(<fmt/format.h>)
#include <fmt/format.h>
#else
#pragma message                                                                \
    "<fmt/format.h> not found! Make sure to include fmtlib prior to this header."
#endif

namespace gnuplotxx {

struct Color {
  constexpr Color() : Color(0, 0, 0) {}

  constexpr Color(int rgba) : m_rgba(rgba) {}

  constexpr Color(int r, int g, int b, int a = 0)
      : m_rgba((a << 24) | (r << 16) | (g << 8) | b) {}

  constexpr Color(double r, double g, double b, double a = 0.0)
      : Color((int)(255 * r + 0.5), (int)(255 * g + 0.5), (int)(255 * b + 0.5),
              (int)(255 * a + 0.5)) {}

  constexpr int red() const { return (m_rgba >> 16) & 0xff; }
  constexpr int green() const { return (m_rgba >> 8) & 0xff; }
  constexpr int blue() const { return m_rgba & 0xff; }
  constexpr int alpha() const { return (m_rgba >> 24) & 0xff; }
  constexpr int rgba() const { return m_rgba; }

  constexpr Color fade(int a) const {
    return {(m_rgba & 0xffffff) | ((a & 0xff) << 24)};
  }
  constexpr Color fade(double a) const { return fade((int)(255 * a + 0.5)); }

  constexpr operator int() const { return m_rgba; }

private:
  int m_rgba;
};

struct Colors {
  // Colors taken from: https://en.wikipedia.org/wiki/Web_colors#Extended_colors

  // Generated via:
  // wget -qO -
  // "https://en.wikipedia.org/wiki/Special:ExpandTemplates?wpInput={{X11 color
  // chart}}" | sed 's/&amp;nbsp;//g' | awk '/\| colspan=/ { match($0,
  // /'"'''"'/); $0=substr($0, RSTART+3); match($0, /'"'''"'/); $0=substr($0, 0,
  // RSTART-1); print "// " $0} /\| ([A-Z][a-z]+)+ \|\|/ {  printf "static
  // inline constexpr Color " $2 "{" substr($5, 5) ", " $6 ", " $7 "}; \t\t\t//
  // #" substr($3, 3) $4 substr($5, 0, 2) "\n"}'

  // We recommend installing the VSCode extension colorize
  // (kamikillerto.vscode-colorize) and adding the following configuration:
  // "colorize.include": ["**/gnuplotxx.h"]

  // Pink colors
  static inline constexpr Color MediumVioletRed{199, 21, 133}; // #C71585
  static inline constexpr Color DeepPink{255, 20, 147};        // #FF1493
  static inline constexpr Color PaleVioletRed{219, 112, 147};  // #DB7093
  static inline constexpr Color HotPink{255, 105, 180};        // #FF69B4
  static inline constexpr Color LightPink{255, 182, 193};      // #FFB6C1
  static inline constexpr Color Pink{255, 192, 203};           // #FFC0CB

  // Red colors
  static inline constexpr Color DarkRed{139, 0, 0};         // #8B0000
  static inline constexpr Color Red{255, 0, 0};             // #FF0000
  static inline constexpr Color Firebrick{178, 34, 34};     // #B22222
  static inline constexpr Color Crimson{220, 20, 60};       // #DC143C
  static inline constexpr Color IndianRed{205, 92, 92};     // #CD5C5C
  static inline constexpr Color LightCoral{240, 128, 128};  // #F08080
  static inline constexpr Color Salmon{250, 128, 114};      // #FA8072
  static inline constexpr Color DarkSalmon{233, 150, 122};  // #E9967A
  static inline constexpr Color LightSalmon{255, 160, 122}; // #FFA07A

  // Orange colors
  static inline constexpr Color OrangeRed{255, 69, 0};   // #FF4500
  static inline constexpr Color Tomato{255, 99, 71};     // #FF6347
  static inline constexpr Color DarkOrange{255, 140, 0}; // #FF8C00
  static inline constexpr Color Coral{255, 127, 80};     // #FF7F50
  static inline constexpr Color Orange{255, 165, 0};     // #FFA500

  // Yellow colors
  static inline constexpr Color DarkKhaki{189, 183, 107};            // #BDB76B
  static inline constexpr Color Gold{255, 215, 0};                   // #FFD700
  static inline constexpr Color Khaki{240, 230, 140};                // #F0E68C
  static inline constexpr Color PeachPuff{255, 218, 185};            // #FFDAB9
  static inline constexpr Color Yellow{255, 255, 0};                 // #FFFF00
  static inline constexpr Color PaleGoldenrod{238, 232, 170};        // #EEE8AA
  static inline constexpr Color Moccasin{255, 228, 181};             // #FFE4B5
  static inline constexpr Color PapayaWhip{255, 239, 213};           // #FFEFD5
  static inline constexpr Color LightGoldenrodYellow{250, 250, 210}; // #FAFAD2
  static inline constexpr Color LemonChiffon{255, 250, 205};         // #FFFACD
  static inline constexpr Color LightYellow{255, 255, 224};          // #FFFFE0

  // Brown colors
  static inline constexpr Color Maroon{128, 0, 0};             // #800000
  static inline constexpr Color Brown{165, 42, 42};            // #A52A2A
  static inline constexpr Color SaddleBrown{139, 69, 19};      // #8B4513
  static inline constexpr Color Sienna{160, 82, 45};           // #A0522D
  static inline constexpr Color Chocolate{210, 105, 30};       // #D2691E
  static inline constexpr Color DarkGoldenrod{184, 134, 11};   // #B8860B
  static inline constexpr Color Peru{205, 133, 63};            // #CD853F
  static inline constexpr Color RosyBrown{188, 143, 143};      // #BC8F8F
  static inline constexpr Color Goldenrod{218, 165, 32};       // #DAA520
  static inline constexpr Color SandyBrown{244, 164, 96};      // #F4A460
  static inline constexpr Color Tan{210, 180, 140};            // #D2B48C
  static inline constexpr Color Burlywood{222, 184, 135};      // #DEB887
  static inline constexpr Color Wheat{245, 222, 179};          // #F5DEB3
  static inline constexpr Color NavajoWhite{255, 222, 173};    // #FFDEAD
  static inline constexpr Color Bisque{255, 228, 196};         // #FFE4C4
  static inline constexpr Color BlanchedAlmond{255, 235, 205}; // #FFEBCD
  static inline constexpr Color Cornsilk{255, 248, 220};       // #FFF8DC

  // Green colors
  static inline constexpr Color DarkGreen{0, 100, 0};            // #006400
  static inline constexpr Color Green{0, 128, 0};                // #008000
  static inline constexpr Color DarkOliveGreen{85, 107, 47};     // #556B2F
  static inline constexpr Color ForestGreen{34, 139, 34};        // #228B22
  static inline constexpr Color SeaGreen{46, 139, 87};           // #2E8B57
  static inline constexpr Color Olive{128, 128, 0};              // #808000
  static inline constexpr Color OliveDrab{107, 142, 35};         // #6B8E23
  static inline constexpr Color MediumSeaGreen{60, 179, 113};    // #3CB371
  static inline constexpr Color LimeGreen{50, 205, 50};          // #32CD32
  static inline constexpr Color Lime{0, 255, 0};                 // #00FF00
  static inline constexpr Color SpringGreen{0, 255, 127};        // #00FF7F
  static inline constexpr Color MediumSpringGreen{0, 250, 154};  // #00FA9A
  static inline constexpr Color DarkSeaGreen{143, 188, 143};     // #8FBC8F
  static inline constexpr Color MediumAquamarine{102, 205, 170}; // #66CDAA
  static inline constexpr Color YellowGreen{154, 205, 50};       // #9ACD32
  static inline constexpr Color LawnGreen{124, 252, 0};          // #7CFC00
  static inline constexpr Color Chartreuse{127, 255, 0};         // #7FFF00
  static inline constexpr Color LightGreen{144, 238, 144};       // #90EE90
  static inline constexpr Color GreenYellow{173, 255, 47};       // #ADFF2F
  static inline constexpr Color PaleGreen{152, 251, 152};        // #98FB98

  // Cyan colors
  static inline constexpr Color Teal{0, 128, 128};             // #008080
  static inline constexpr Color DarkCyan{0, 139, 139};         // #008B8B
  static inline constexpr Color LightSeaGreen{32, 178, 170};   // #20B2AA
  static inline constexpr Color CadetBlue{95, 158, 160};       // #5F9EA0
  static inline constexpr Color DarkTurquoise{0, 206, 209};    // #00CED1
  static inline constexpr Color MediumTurquoise{72, 209, 204}; // #48D1CC
  static inline constexpr Color Turquoise{64, 224, 208};       // #40E0D0
  static inline constexpr Color Aqua{0, 255, 255};             // #00FFFF
  static inline constexpr Color Cyan{0, 255, 255};             // #00FFFF
  static inline constexpr Color Aquamarine{127, 255, 212};     // #7FFFD4
  static inline constexpr Color PaleTurquoise{175, 238, 238};  // #AFEEEE
  static inline constexpr Color LightCyan{224, 255, 255};      // #E0FFFF

  // Blue colors
  static inline constexpr Color Navy{0, 0, 128};               // #000080
  static inline constexpr Color DarkBlue{0, 0, 139};           // #00008B
  static inline constexpr Color MediumBlue{0, 0, 205};         // #0000CD
  static inline constexpr Color Blue{0, 0, 255};               // #0000FF
  static inline constexpr Color MidnightBlue{25, 25, 112};     // #191970
  static inline constexpr Color RoyalBlue{65, 105, 225};       // #4169E1
  static inline constexpr Color SteelBlue{70, 130, 180};       // #4682B4
  static inline constexpr Color DodgerBlue{30, 144, 255};      // #1E90FF
  static inline constexpr Color DeepSkyBlue{0, 191, 255};      // #00BFFF
  static inline constexpr Color CornflowerBlue{100, 149, 237}; // #6495ED
  static inline constexpr Color SkyBlue{135, 206, 235};        // #87CEEB
  static inline constexpr Color LightSkyBlue{135, 206, 250};   // #87CEFA
  static inline constexpr Color LightSteelBlue{176, 196, 222}; // #B0C4DE
  static inline constexpr Color LightBlue{173, 216, 230};      // #ADD8E6
  static inline constexpr Color PowderBlue{176, 224, 230};     // #B0E0E6

  // Purple, violet, and magenta colors
  static inline constexpr Color Indigo{75, 0, 130};             // #4B0082
  static inline constexpr Color Purple{128, 0, 128};            // #800080
  static inline constexpr Color DarkMagenta{139, 0, 139};       // #8B008B
  static inline constexpr Color DarkViolet{148, 0, 211};        // #9400D3
  static inline constexpr Color DarkSlateBlue{72, 61, 139};     // #483D8B
  static inline constexpr Color BlueViolet{138, 43, 226};       // #8A2BE2
  static inline constexpr Color DarkOrchid{153, 50, 204};       // #9932CC
  static inline constexpr Color Fuchsia{255, 0, 255};           // #FF00FF
  static inline constexpr Color Magenta{255, 0, 255};           // #FF00FF
  static inline constexpr Color SlateBlue{106, 90, 205};        // #6A5ACD
  static inline constexpr Color MediumSlateBlue{123, 104, 238}; // #7B68EE
  static inline constexpr Color MediumOrchid{186, 85, 211};     // #BA55D3
  static inline constexpr Color MediumPurple{147, 112, 219};    // #9370DB
  static inline constexpr Color Orchid{218, 112, 214};          // #DA70D6
  static inline constexpr Color Violet{238, 130, 238};          // #EE82EE
  static inline constexpr Color Plum{221, 160, 221};            // #DDA0DD
  static inline constexpr Color Thistle{216, 191, 216};         // #D8BFD8
  static inline constexpr Color Lavender{230, 230, 250};        // #E6E6FA

  // White colors
  static inline constexpr Color MistyRose{255, 228, 225};     // #FFE4E1
  static inline constexpr Color AntiqueWhite{250, 235, 215};  // #FAEBD7
  static inline constexpr Color Linen{250, 240, 230};         // #FAF0E6
  static inline constexpr Color Beige{245, 245, 220};         // #F5F5DC
  static inline constexpr Color WhiteSmoke{245, 245, 245};    // #F5F5F5
  static inline constexpr Color LavenderBlush{255, 240, 245}; // #FFF0F5
  static inline constexpr Color OldLace{253, 245, 230};       // #FDF5E6
  static inline constexpr Color AliceBlue{240, 248, 255};     // #F0F8FF
  static inline constexpr Color Seashell{255, 245, 238};      // #FFF5EE
  static inline constexpr Color GhostWhite{248, 248, 255};    // #F8F8FF
  static inline constexpr Color Honeydew{240, 255, 240};      // #F0FFF0
  static inline constexpr Color FloralWhite{255, 250, 240};   // #FFFAF0
  static inline constexpr Color Azure{240, 255, 255};         // #F0FFFF
  static inline constexpr Color MintCream{245, 255, 250};     // #F5FFFA
  static inline constexpr Color Snow{255, 250, 250};          // #FFFAFA
  static inline constexpr Color Ivory{255, 255, 240};         // #FFFFF0
  static inline constexpr Color White{255, 255, 255};         // #FFFFFF

  // Gray and black colors
  static inline constexpr Color Black{0, 0, 0};                // #000000
  static inline constexpr Color DarkSlateGray{47, 79, 79};     // #2F4F4F
  static inline constexpr Color DimGray{105, 105, 105};        // #696969
  static inline constexpr Color SlateGray{112, 128, 144};      // #708090
  static inline constexpr Color Gray{128, 128, 128};           // #808080
  static inline constexpr Color LightSlateGray{119, 136, 153}; // #778899
  static inline constexpr Color DarkGray{169, 169, 169};       // #A9A9A9
  static inline constexpr Color Silver{192, 192, 192};         // #C0C0C0
  static inline constexpr Color LightGray{211, 211, 211};      // #D3D3D3
  static inline constexpr Color Gainsboro{220, 220, 220};      // #DCDCDC
};

enum class PlotStyle {
  Points,
  Dots,
  Lines,
  LinesPoints,
  Steps,
  FSteps,
  HiSteps,
  Impulses,
  Boxes,
};

enum class PointType {
  Cross,
  X,
  Asterisk,
  Star = Asterisk,
  Box,
  FilledBox,
  Circle,
  FilledCircle,
  Triangle,
  FilledTriangle,
  UpsideDownTriangle,
  FilledUpsideDownTriangle,
  Diamond,
  FilledDiamond,
  Pentagon,
  FilledPentagon,
};

enum class Smooth {
  Unique,
  Frequency,
  FNormal,
  Cumulative,
  CNormal,
  Bins,
  KDensity,
  CSplines,
  ACSplines,
  MCSplines,
  Bezier,
  SBezier,
  Unwrap,
};

namespace detail {

static inline std::unordered_map<PlotStyle, std::string_view> plotStyleLUT = {
    {PlotStyle::Points, "points"},   {PlotStyle::Dots, "dots"},
    {PlotStyle::Lines, "lines"},     {PlotStyle::LinesPoints, "linespoints"},
    {PlotStyle::Steps, "steps"},     {PlotStyle::FSteps, "fsteps"},
    {PlotStyle::HiSteps, "histeps"}, {PlotStyle::Impulses, "impulses"},
    {PlotStyle::Boxes, "boxes"},
};

static inline std::unordered_map<Smooth, std::string_view> smoothLUT = {
    {Smooth::Unique, "unique"},       {Smooth::Frequency, "frequency"},
    {Smooth::FNormal, "fnormal"},     {Smooth::Cumulative, "cumulative"},
    {Smooth::CNormal, "cnormal"},     {Smooth::Bins, "bins"},
    {Smooth::KDensity, "kdensity"},   {Smooth::CSplines, "csplines"},
    {Smooth::ACSplines, "acsplines"}, {Smooth::MCSplines, "mcsplines"},
    {Smooth::Bezier, "bezier"},       {Smooth::SBezier, "sbezier"},
    {Smooth::Unwrap, "unwrap"},
};

} // namespace detail

using Point = std::pair<int, int>;
using Size = std::pair<unsigned int, unsigned int>;

enum class Abscissa {
  X1,
  X2,
};

enum class Ordinate {
  Y1,
  Y2,
};

using Axis = std::variant<Abscissa, Ordinate>;

class AxesPair {
  friend struct Axes;

public:
  constexpr Abscissa x() const { return m_x; }
  constexpr Ordinate y() const { return m_y; }

  // TODO Spaceship?
  bool operator==(const AxesPair &rhs) const {
    return m_x == rhs.m_x && m_y == rhs.m_y;
  }

private:
  constexpr AxesPair(Abscissa x, Ordinate y) : m_x(x), m_y(y) {}

  Abscissa m_x;
  Ordinate m_y;
};

struct Axes {
  static constexpr auto X1 = Abscissa::X1;
  static constexpr auto X2 = Abscissa::X2;
  static constexpr auto Y1 = Ordinate::Y1;
  static constexpr auto Y2 = Ordinate::Y2;

  static constexpr AxesPair X1Y1{X1, Y1};
  static constexpr AxesPair X1Y2{X1, Y2};
  static constexpr AxesPair X2Y1{X2, Y1};
  static constexpr AxesPair X2Y2{X2, Y2};

  static constexpr std::size_t Count = 4;
};

struct AutoType {};

inline bool operator==(const AutoType &, const AutoType &) { return true; }

constexpr AutoType Auto{};

using LimitValue = std::variant<AutoType, int, double>;
using Limit = std::pair<LimitValue, LimitValue>;
using RangeValue = std::variant<AutoType, int, double, Limit>;
using Range = std::pair<RangeValue, RangeValue>;

inline Limit operator<(const AutoType &, double value) { return {Auto, value}; }

inline Limit operator<(double value, const AutoType &) { return {value, Auto}; }

namespace detail {

struct EmptyFormatSpecParser {
  constexpr auto parse(fmt::format_parse_context &ctx) {
    auto it = ctx.begin();

    if (it != ctx.end() && *it != '}')
      throw fmt::format_error("invalid format");

    return it;
  }
};

struct AxisFormatSpecParser {
  constexpr auto parse(fmt::format_parse_context &ctx) {
    auto it = ctx.begin(), end = ctx.end();
    if (it != end && (*it == '1' || *it == '2'))
      spec = *it++;

    if (it != ctx.end() && *it != '}')
      throw fmt::format_error("invalid format");

    return it;
  }

  char spec = '1';
};

constexpr std::size_t axisIndex(const Axis &axis) {
  return std::visit(
      [](auto &&axis) -> std::size_t {
        using T = std::decay_t<decltype(axis)>;
        if constexpr (std::is_same_v<T, Abscissa>) {
          if (axis == Abscissa::X2)
            return 2;
        } else if constexpr (std::is_same_v<T, Ordinate>) {
          switch (axis) {
          case Ordinate::Y1:
            return 1;
          case Ordinate::Y2:
            return 3;
          }
        }
        return 0;
      },
      axis);
}

constexpr std::pair<Axis, std::size_t> axisWithIndexPair(const Axis &axis) {
  return {axis, axisIndex(axis)};
}

} // namespace detail

} // namespace gnuplotxx

template <>
struct fmt::formatter<gnuplotxx::Abscissa>
    : gnuplotxx::detail::AxisFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::Abscissa &x, FormatContext &ctx) {
    auto out = ctx.out();
    auto v = static_cast<char>(x);
    *(out++) = 'x';
    if (v > 0 || spec == '2')
      *(out++) = '1' + v;
    return out;
  }
};

template <>
struct fmt::formatter<gnuplotxx::Ordinate>
    : gnuplotxx::detail::AxisFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::Ordinate &y, FormatContext &ctx) {
    auto out = ctx.out();
    auto v = static_cast<char>(y);
    *(out++) = 'y';
    if (v > 0 || spec == '2')
      *(out++) = '1' + v;
    return out;
  }
};

template <>
struct fmt::formatter<gnuplotxx::Axis>
    : gnuplotxx::detail::AxisFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::Axis &axis, FormatContext &ctx) {
    return std::visit(
        [spec = spec, &ctx](auto &&axis) {
          return format_to(ctx.out(), (spec == '1' ? "{}" : "{:2}"), axis);
        },
        axis);
  }
};

template <>
struct fmt::formatter<gnuplotxx::AxesPair>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::AxesPair &axes, FormatContext &ctx) {
    return format_to(ctx.out(), "{:2}{:2}", axes.x(), axes.y());
  }
};

template <>
struct fmt::formatter<gnuplotxx::AutoType>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::AutoType &, FormatContext &ctx) {
    auto out = ctx.out();
    *(out++) = '*';
    return out;
  }
};

template <>
struct fmt::formatter<gnuplotxx::LimitValue>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::LimitValue &value, FormatContext &ctx) {
    return std::visit(
        [&ctx](auto &&value) { return format_to(ctx.out(), "{}", value); },
        value);
  }
};

template <>
struct fmt::formatter<gnuplotxx::Limit>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::Limit &limit, FormatContext &ctx) {
    const auto &[lower, upper] = limit;
    return format_to(ctx.out(), "{}<{}", lower, upper);
  }
};

template <>
struct fmt::formatter<gnuplotxx::RangeValue>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::RangeValue &value, FormatContext &ctx) {
    return std::visit(
        [&ctx](auto &&value) { return format_to(ctx.out(), "{}", value); },
        value);
  }
};

template <>
struct fmt::formatter<gnuplotxx::Range>
    : gnuplotxx::detail::EmptyFormatSpecParser {
  template <typename FormatContext>
  auto format(const gnuplotxx::Range &range, FormatContext &ctx) {
    const auto &[min, max] = range;

    return format_to(ctx.out(), "[{}:{}]", min, max);
  }
};

namespace gnuplotxx {

template <typename T>
struct IsNumber
    : std::bool_constant<!std::is_same_v<std::remove_cv_t<T>, bool> &&
                         std::is_arithmetic_v<T>> {};

template <typename T> constexpr bool IsNumberV = IsNumber<T>::value;

template <typename T> concept Number = IsNumberV<T>;

namespace detail {

template <typename T, std::size_t I = 0>
constexpr bool isTupleLikeOfNumbersHelper() {
  using std::get;
  if constexpr (I < std::tuple_size_v<T>) {
    bool ret = requires(T t) { IsNumberV<decltype(get<I>(t))>; };

    if constexpr (I < std::tuple_size_v<T> - 1)
      ret &= isTupleLikeOfNumbersHelper<T, I + 1>();

    return ret;
  }

  return false;
}

} // namespace detail

template <typename T>
struct IsTupleLikeOfNumbers
    : std::bool_constant<detail::isTupleLikeOfNumbersHelper<T>()> {};

template <typename T>
constexpr bool IsTupleLikeOfNumbersV = IsTupleLikeOfNumbers<T>::value;

template <typename T> concept TupleLikeOfNumbers = IsTupleLikeOfNumbersV<T>;

namespace detail {

struct SeriesDataPriv;
using SeriesDataPrivPtr = std::shared_ptr<SeriesDataPriv>;

struct SeriesPriv;
using SeriesPrivPtr = std::shared_ptr<SeriesPriv>;

struct PlotPriv;
using PlotPrivPtr = std::shared_ptr<PlotPriv>;

struct SeriesDataPriv {
  SeriesDataPriv(int id) : id(id), empty(true) {}
  int id;
  bool empty;
  bool dirty;
};

struct SeriesPriv {
  SeriesPriv(PlotPriv &plot, SeriesDataPrivPtr data, std::string title)
      : plot(plot), data(data), title(std::move(title)),
        plotStyle(PlotStyle::Points), lineWidth(1.0), pointSize(1.0),
        axes(Axes::X1Y1), axesDirty(true) {}

  PlotPriv &plot;

  SeriesDataPrivPtr data;

  std::string title;
  PlotStyle plotStyle;
  std::optional<Color> color;
  double lineWidth;
  std::optional<PointType> pointType;
  double pointSize;
  std::optional<Smooth> smooth;
  AxesPair axes;

  bool dirty;
  bool axesDirty;
  bool removed;
};

inline bool close(int fd) {
  while (::close(fd) < 0) {
    if (errno != EINTR)
      return false;
  }

  return true;
}

inline bool dup2(int oldfd, int newfd) {
  while (::dup2(oldfd, newfd) < 0) {
    if (errno != EINTR)
      return false;
  }

  return true;
}

inline int open(const char *pathname, int flags) {
  int fd;
  while ((fd = ::open(pathname, flags)) < 0) {
    if (errno != EINTR)
      break;
  }

  return fd;
}

inline ssize_t read(int fd, void *buf, size_t count) {
  ssize_t ret;
  while ((ret = ::read(fd, buf, count)) < 0) {
    if (errno != EINTR)
      break;
  }

  return ret;
}

inline bool writeAll(int fd, const void *buf, std::size_t count) {
  ssize_t ret = 0, written = 0;
  const std::byte *p = static_cast<const std::byte *>(buf);

  do {
    ret = ::write(fd, static_cast<const void *>(p + written), count - written);
    if (ret < 0 && errno != EINTR) {
      return false;
    } else if (ret >= 0) {
      written += ret;
    }
  } while (static_cast<std::size_t>(written) < count);

  return true;
}

inline bool writeAll(int fd, const std::string &buf) {
  return writeAll(fd, buf.data(), buf.size());
}

class Process {
public:
  Process() : fd(-1) {
    int stdinFds[2], execErrFds[2];

    if (::pipe(stdinFds) < 0)
      throw std::system_error(errno, std::generic_category(),
                              "Process: pipe (stdin FDs)");
    if (::pipe(execErrFds) < 0)
      throw std::system_error(errno, std::generic_category(),
                              "Process: pipe (exec error FDs)");

    if (::fcntl(execErrFds[1], F_SETFD, FD_CLOEXEC) < 0)
      throw std::system_error(errno, std::generic_category(), "Process: fcntl");

    if ((pid = ::fork()) < 0)
      throw std::system_error(errno, std::generic_category(), "Process: fork");

    if (pid == 0) {
      auto raiseError = [errFd = execErrFds[1]](int err, std::string_view msg) {
        writeAll(errFd, &err, sizeof(err));
        writeAll(errFd, msg.data(), msg.size());
        ::exit(-1);
      };

      close(execErrFds[0]);
      close(stdinFds[1]);

      if (!dup2(stdinFds[0], STDIN_FILENO))
        raiseError(errno, "child: dup2 (stdin)");

      close(stdinFds[0]);

      int nullFd = open("/dev/null", O_WRONLY);
      if (nullFd < 0)
        raiseError(errno, "child: open (/dev/null)");

      if (!dup2(nullFd, STDOUT_FILENO))
        raiseError(errno, "child: dup2 (stdout)");
      if (!dup2(nullFd, STDERR_FILENO))
        raiseError(errno, "child: dup2 (stderr)");

      close(nullFd);

      ::prctl(PR_SET_PDEATHSIG, SIGTERM);

      ::execlp("gnuplot", "gnuplot", "-d", (char *)NULL);

      raiseError(errno, "execlp");
    }

    close(stdinFds[0]);
    close(execErrFds[1]);

    int epollFd;

    if ((epollFd = ::epoll_create1(0)) < 0)
      throw std::system_error(errno, std::generic_category(),
                              "Process: epoll_create1");

    epoll_event events[1] = {{.events = EPOLLIN, .data = {}}};

    if (::epoll_ctl(epollFd, EPOLL_CTL_ADD, execErrFds[0], &events[0]) < 0)
      throw std::system_error(errno, std::generic_category(),
                              "Process: epoll_ctl");

    while (::epoll_wait(epollFd, events, 1, -1) < 0) {
      if (errno != EINTR)
        throw std::system_error(errno, std::generic_category(),
                                "Process: epoll_wait");
    }

    int err, ret;
    if ((ret = read(execErrFds[0], &err, sizeof(err))) < 0)
      throw std::system_error(errno, std::generic_category(),
                              "Process: read (exec error FD) error code");

    if (ret > 0) {
      std::size_t size, maxSize = 128;
      std::string msg;

      msg.reserve(maxSize);
      msg += "Process: failed to exec gnuplot: ";
      size = msg.size();

      if ((ret = read(execErrFds[0], &msg[size], maxSize - size)) < 0)
        throw std::system_error(errno, std::generic_category(),
                                "Process: read (exec error FD) error message");

      msg.resize(size + ret);

      throw std::system_error(err, std::generic_category(), msg);
    }

    close(execErrFds[1]);
    close(epollFd);

    fd = stdinFds[1];
  }

  ~Process() {
    if (fd < 0)
      return;

    close(fd);
    while (::waitpid(pid, nullptr, 0) < 0) {
      if (errno != EINTR)
        break;
    }
  }

  bool write(const std::string &buf) { return writeAll(fd, buf); }

private:
  pid_t pid;
  int fd;
};

struct PlotPriv {
  PlotPriv(std::string title)
      : title(std::move(title)), font("Sans"), fontSize(9), activeDataId(-1) {}

  Process gp;

  std::string title;
  std::string font;
  int fontSize;

  std::optional<Point> position;
  std::optional<Size> size;

  std::array<Range, Axes::Count> ranges;

  int nextDataId;
  int activeDataId;
  std::vector<SeriesPrivPtr> series;

  std::string buf;
  bool dirty;
  bool rangesDirty;
};

template <Number First, Number... Ns>
void appendHelper(std::string &buf, First first, Ns... rest) {
  fmt::format_to(std::back_inserter(buf), "{}", first);
  if constexpr (sizeof...(rest) > 0) {
    buf += ' ';
    appendHelper(buf, rest...);
  }
}

template <TupleLikeOfNumbers T, std::size_t I = 0>
void appendHelper(std::string &buf, const T &t) {
  using std::get;
  if constexpr (I < std::tuple_size_v<T>) {
    fmt::format_to(std::back_inserter(buf), "{}", get<I>(t));
    if constexpr (I < std::tuple_size_v<T> - 1) {
      buf += ' ';
      appendHelper<T, I + 1>(buf, t);
    } else
      buf += '\n';
  }
}

} // namespace detail

class Series {
  friend class Plot;

public:
  Series() {}
  Series(detail::SeriesPrivPtr series) : m_series(series) {}

  int dataId() const {
    assert(m_series);
    return m_series->data->id;
  }

  Series &title(std::string title) {
    assert(m_series);
    if (m_series->title != title) {
      m_series->title = std::move(title);
      m_series->dirty = true;
    }
    return *this;
  }

  const std::string &title() const {
    assert(m_series);
    return m_series->title;
  }

  Series &with(PlotStyle plotStyle) {
    assert(m_series);
    if (m_series->plotStyle != plotStyle) {
      m_series->plotStyle = plotStyle;
      m_series->dirty = true;
    }
    return *this;
  }

  PlotStyle with() const {
    assert(m_series);
    return m_series->plotStyle;
  }

  Series &color(std::optional<Color> color) {
    assert(m_series);
    if (m_series->color != color) {
      m_series->color = color;
      m_series->dirty = true;
    }
    return *this;
  }

  std::optional<Color> color() const {
    assert(m_series);
    return m_series->color;
  }

  Series &lineWidth(double lineWidth) {
    assert(m_series);
    if (m_series->lineWidth != lineWidth) {
      m_series->lineWidth = lineWidth;
      m_series->dirty = true;
    }
    return *this;
  }

  double lineWidth() const {
    assert(m_series);
    return m_series->lineWidth;
  }

  Series &pointType(std::optional<PointType> pointType) {
    assert(m_series);
    if (m_series->pointType != pointType) {
      m_series->pointType = pointType;
      m_series->dirty = true;
    }
    return *this;
  }

  std::optional<PointType> pointType() const {
    assert(m_series);
    return m_series->pointType;
  }

  Series &pointSize(double pointSize) {
    assert(m_series);
    if (m_series->pointSize != pointSize) {
      m_series->pointSize = pointSize;
      m_series->dirty = true;
    }
    return *this;
  }

  double pointSize() const {
    assert(m_series);
    return m_series->pointSize;
  }

  Series &smooth(std::optional<Smooth> smooth) {
    assert(m_series);
    if (m_series->smooth != smooth) {
      m_series->smooth = smooth;
      m_series->dirty = true;
    }
    return *this;
  }

  std::optional<Smooth> smooth() const {
    assert(m_series);
    return m_series->smooth;
  }

  Series &axes(AxesPair axes) {
    assert(m_series);
    if (m_series->axes != axes) {
      m_series->axes = axes;
      m_series->dirty = m_series->axesDirty = true;
    }
    return *this;
  }

  AxesPair axes() const {
    assert(m_series);
    return m_series->axes;
  }

  template <Number... Ns> Series &append(Ns... args) {
    assert(m_series);

    const auto &[plot, buf, data] = appendPrologue();

    buf += "print \"";
    detail::appendHelper(buf, args...);
    buf += "\"\n";

    return appendEpilogue(plot, buf, data);
  }

  template <TupleLikeOfNumbers T> Series &append(const T &t) {
    assert(m_series);

    const auto &[plot, buf, data] = appendPrologue();

    detail::appendHelper(buf, t);

    return appendEpilogue(plot, buf, data);
  }

  template <std::input_iterator I, std::sentinel_for<I> S>
  Series &append(I first, S last) requires(IsNumberV<typename I::value_type>) {
    assert(m_series);

    const auto &[plot, buf, data] = appendPrologue();

    if (first != last)
      fmt::format_to(std::back_inserter(buf), "print \"{}", *(first++));

    while (first != last)
      fmt::format_to(std::back_inserter(buf), " {}", *(first++));

    buf += "\"\n";

    return appendEpilogue(plot, buf, data);
  }

  template <std::input_iterator I, std::sentinel_for<I> S>
  Series &append(I first, S last, std::size_t batchSize = 64) requires(
      !IsNumberV<typename I::value_type> &&
      IsTupleLikeOfNumbersV<typename I::value_type>) {
    if (first == last)
      return *this;

    assert(m_series);

    const auto &[plot, buf, data] = appendPrologue();

    while (first != last) {
      for (std::size_t i = 0; first != last && i < batchSize; ++i) {
        buf += "print \"";
        detail::appendHelper(buf, *(first++));
        buf += "\"\n";
      }
      if (!plot.gp.write(buf))
        throw std::system_error(errno, std::generic_category(),
                                "Series: append: write print command");
      buf.clear();
    }

    return appendEpilogue(data);
  }

  Series &clear() {
    assert(m_series);

    auto &plot = m_series->plot;
    auto &buf = plot.buf;
    auto &data = *m_series->data;
    buf.clear();

    fmt::format_to(std::back_inserter(buf), "undefine $_{}\n", data.id);
    if (!plot.gp.write(buf))
      throw std::system_error(errno, std::generic_category(),
                              "Series: clear: write");

    data.empty = true;
    data.dirty = true;

    return *this;
  }

  operator bool() const { return (bool)m_series; }

private:
  struct AppendPrologueResult {
    detail::PlotPriv &plot;
    std::string &buf;
    detail::SeriesDataPrivPtr &data;
  };

  AppendPrologueResult appendPrologue() {
    auto &plot = m_series->plot;
    auto &buf = plot.buf;
    auto &data = m_series->data;
    buf.clear();

    if (data->id != plot.activeDataId) {
      fmt::format_to(std::back_inserter(buf), "set print $_{} append\n",
                     data->id);
      plot.activeDataId = data->id;
    }

    return {plot, buf, data};
  }

  Series &appendEpilogue(detail::SeriesDataPrivPtr &data) {
    data->empty = false;
    data->dirty = true;

    return *this;
  }

  Series &appendEpilogue(detail::PlotPriv &plot, std::string &buf,
                         detail::SeriesDataPrivPtr &data) {
    if (!plot.gp.write(buf))
      throw std::system_error(errno, std::generic_category(),
                              "Series: append (epilogue): write");

    return appendEpilogue(data);
  }

  detail::SeriesPrivPtr m_series;
};

class Plot {
public:
  Plot() {}

  Plot(std::string title)
      : m_plot(std::make_shared<detail::PlotPriv>(std::move(title))) {}

  Plot(detail::PlotPrivPtr plot) : m_plot(plot) {}

  Plot &title(std::string title) {
    assert(m_plot);
    if (m_plot->title != title) {
      m_plot->title = std::move(title);
      m_plot->dirty = true;
    }
    return *this;
  }

  const std::string &title() const {
    assert(m_plot);
    return m_plot->title;
  }

  Plot &font(std::string font) {
    assert(m_plot);
    if (m_plot->font != font) {
      m_plot->font = std::move(font);
      m_plot->dirty = true;
    }
    return *this;
  }

  Plot &font(std::string font, int fontSize) {
    assert(m_plot);
    if (m_plot->font != font || m_plot->fontSize != fontSize) {
      m_plot->font = std::move(font);
      m_plot->fontSize = fontSize;
      m_plot->dirty = true;
    }
    return *this;
  }

  const std::string &font() const {
    assert(m_plot);
    return m_plot->font;
  }

  Plot &fontSize(int fontSize) {
    assert(m_plot);
    if (m_plot->fontSize != fontSize) {
      m_plot->fontSize = fontSize;
      m_plot->dirty = true;
    }
    return *this;
  }

  int fontSize() const {
    assert(m_plot);
    return m_plot->fontSize;
  }

  Plot &position(std::optional<Point> position) {
    assert(m_plot);
    if (m_plot->position != position) {
      m_plot->position = position;
      m_plot->dirty = true;
    }
    return *this;
  }

  Plot &position(int x, int y) { return position(Point(x, y)); }

  std::optional<Point> position() const {
    assert(m_plot);
    return m_plot->position;
  }

  Plot &size(std::optional<Size> size) {
    assert(m_plot);
    if (m_plot->size != size) {
      m_plot->size = size;
      m_plot->dirty = true;
    }
    return *this;
  }

  Plot &size(int width, int height) { return size(Size(width, height)); }

  std::optional<Size> size() const {
    assert(m_plot);
    return m_plot->size;
  }

  Plot &range(Axis axis, const Range &range) {
    assert(m_plot);
    auto &m_range = m_plot->ranges[detail::axisIndex(axis)];
    if (m_range != range) {
      m_range = range;
      m_plot->dirty = m_plot->rangesDirty = true;
    }
    return *this;
  }

  Plot &range(Axis axis, const RangeValue &min, const RangeValue &max) {
    return range(axis, {min, max});
  }

  const Range &range(Axis axis) const {
    assert(m_plot);
    return m_plot->ranges[detail::axisIndex(axis)];
  }

  Series createSeries(std::string title) {
    assert(m_plot);

    auto data = std::make_shared<detail::SeriesDataPriv>(m_plot->nextDataId++);
    auto series =
        std::make_shared<detail::SeriesPriv>(*m_plot, data, std::move(title));

    m_plot->series.push_back(series);

    return {series};
  }

  Series createSeries(std::string title, const Series &dataSource) {
    assert(m_plot);

    auto series = std::make_shared<detail::SeriesPriv>(
        *m_plot, dataSource.m_series->data, std::move(title));

    m_plot->series.push_back(series);

    return {series};
  }

  std::size_t numSeries() {
    assert(m_plot);

    return m_plot->series.size();
  }

  Series getSeries(std::size_t index) {
    assert(m_plot);

    if (index < m_plot->series.size())
      return {m_plot->series[index]};

    return {};
  }

  Series findSeries(std::string_view title) {
    assert(m_plot);

    for (auto series : m_plot->series) {
      if (series->title == title)
        return {series};
    }

    return {};
  }

  void removeSeries(Series &series) {
    assert(m_plot);
    assert(series);
    assert(m_plot.get() == &series.m_series->plot);

    series.clear();
    series.m_series->removed = true;
    m_plot->dirty = true;
  }

  void update() {
    using namespace detail;

    assert(m_plot);

    if (m_plot->series.empty())
      return;

    auto &buf = m_plot->buf;
    buf.clear();

    bool dirty = m_plot->dirty;
    bool axesDirty = false;
    std::array<bool, Axes::Count> axisUsed;
    for (auto it = m_plot->series.begin(); it != m_plot->series.end();) {
      auto &series = *it;
      if (series->removed) {
        it = m_plot->series.erase(it);
        dirty = true;
        continue;
      }

      if (!series->data->empty) {
        dirty |= series->dirty | series->data->dirty;
        series->dirty = series->data->dirty = false;

        if (series->axesDirty) {
          axisUsed[axisIndex(series->axes.x())] = true;
          axisUsed[axisIndex(series->axes.y())] = true;
          series->axesDirty = false;
          axesDirty = true;
        }
      }
      it++;
    }

    if (dirty) {
      fmt::format_to(std::back_inserter(buf),
                     "set terminal qt enhanced title \"{}\" font \"{},{}\"",
                     m_plot->title, m_plot->font, m_plot->fontSize);

      if (m_plot->position.has_value()) {
        const auto &[x, y] = m_plot->position.value();
        fmt::format_to(std::back_inserter(buf), " position {},{}", x, y);
      }

      if (m_plot->size.has_value()) {
        const auto &[w, h] = m_plot->size.value();
        fmt::format_to(std::back_inserter(buf), " size {},{}", w, h);
      }

      buf += '\n';
      if (!m_plot->gp.write(buf))
        throw std::system_error(errno, std::generic_category(),
                                "Plot: update: write set term command");

      buf.clear();

      bool first = true;

      if (m_plot->rangesDirty || axesDirty) {
        for (auto [axis, index] :
             {axisWithIndexPair(Axes::X1), axisWithIndexPair(Axes::Y1),
              axisWithIndexPair(Axes::X2), axisWithIndexPair(Axes::Y2)}) {
          fmt::format_to(std::back_inserter(buf), "set {}range {}\n", axis,
                         m_plot->ranges[index]);

          if (axisUsed[index])
            fmt::format_to(std::back_inserter(buf),
                           "set {}tics scale default\n", axis);
          else
            fmt::format_to(std::back_inserter(buf), "unset {}tics\n", axis);
        }
        m_plot->rangesDirty = false;
      }

      buf += "plot ";

      for (auto series : m_plot->series) {
        if (series->data->empty)
          continue;

        if (!first)
          buf += ',';

        first = false;

        fmt::format_to(std::back_inserter(buf), "$_{} title \"{}\"",
                       series->data->id, series->title);

        if (plotStyleLUT.contains(series->plotStyle))
          fmt::format_to(std::back_inserter(buf), " with {}",
                         plotStyleLUT[series->plotStyle]);

        if (series->color.has_value())
          fmt::format_to(std::back_inserter(buf), " linecolor rgb {}",
                         series->color.value().rgba());

        fmt::format_to(std::back_inserter(buf), " linewidth {}",
                       series->lineWidth);

        if (series->pointType.has_value()) {
          fmt::format_to(std::back_inserter(buf), " pointtype {} pointsize {}",
                         static_cast<int>(series->pointType.value()) + 1,
                         series->pointSize);
        }

        if (series->smooth.has_value() &&
            smoothLUT.contains(series->smooth.value()))
          fmt::format_to(std::back_inserter(buf), " smooth {}",
                         smoothLUT[series->smooth.value()]);

        fmt::format_to(std::back_inserter(buf), " axes {}", series->axes);
      }

      if (!first) {
        buf += '\n';
        if (!m_plot->gp.write(buf))
          throw std::system_error(errno, std::generic_category(),
                                  "Plot: update: write plot command");

        m_plot->dirty = false;
      }
    }
  }

  operator bool() const { return (bool)m_plot; }

private:
  detail::PlotPrivPtr m_plot;
};

} // namespace gnuplotxx
