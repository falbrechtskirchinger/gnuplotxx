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
  // Generated via:
  // gnuplot <<<"show colors"  |& tail -n+2 | \
  //   awk '/^ /{ \
  //     printf("static constexpr Color "); \
  //     c = ""; \
  //     n = split($1, a, "-"); \
  //     for(i=1; i<=n; ++i) \
  //       c = c sprintf("%s%s", toupper(substr(a[i], 1, 1)), \
  //                             substr(a[i], 2)); \
  //     printf("%-16s{%3d, %3d, %3d};\n", c, $4, $5, $6); }'
  static constexpr Color White{255, 255, 255};
  static constexpr Color Black{0, 0, 0};
  static constexpr Color DarkGrey{160, 160, 160};
  static constexpr Color Red{255, 0, 0};
  static constexpr Color WebGreen{0, 192, 0};
  static constexpr Color WebBlue{0, 128, 255};
  static constexpr Color DarkMagenta{192, 0, 255};
  static constexpr Color DarkCyan{0, 238, 238};
  static constexpr Color DarkOrange{192, 64, 0};
  static constexpr Color DarkYellow{200, 200, 0};
  static constexpr Color Royalblue{65, 105, 225};
  static constexpr Color Goldenrod{255, 192, 32};
  static constexpr Color DarkSpringGreen{0, 128, 64};
  static constexpr Color Purple{192, 128, 255};
  static constexpr Color Steelblue{48, 96, 128};
  static constexpr Color DarkRed{139, 0, 0};
  static constexpr Color DarkChartreuse{64, 128, 0};
  static constexpr Color Orchid{255, 128, 255};
  static constexpr Color Aquamarine{127, 255, 212};
  static constexpr Color Brown{165, 42, 42};
  static constexpr Color Yellow{255, 255, 0};
  static constexpr Color Turquoise{64, 224, 208};
  static constexpr Color Grey0{0, 0, 0};
  static constexpr Color Grey10{26, 26, 26};
  static constexpr Color Grey20{51, 51, 51};
  static constexpr Color Grey30{77, 77, 77};
  static constexpr Color Grey40{102, 102, 102};
  static constexpr Color Grey50{127, 127, 127};
  static constexpr Color Grey60{153, 153, 153};
  static constexpr Color Grey70{179, 179, 179};
  static constexpr Color Grey{192, 192, 192};
  static constexpr Color Grey80{204, 204, 204};
  static constexpr Color Grey90{229, 229, 229};
  static constexpr Color Grey100{255, 255, 255};
  static constexpr Color LightRed{240, 50, 50};
  static constexpr Color LightGreen{144, 238, 144};
  static constexpr Color LightBlue{173, 216, 230};
  static constexpr Color LightMagenta{240, 85, 240};
  static constexpr Color LightCyan{224, 255, 255};
  static constexpr Color LightGoldenrod{238, 221, 130};
  static constexpr Color LightPink{255, 182, 193};
  static constexpr Color LightTurquoise{175, 238, 238};
  static constexpr Color Gold{255, 215, 0};
  static constexpr Color Green{0, 255, 0};
  static constexpr Color DarkGreen{0, 100, 0};
  static constexpr Color SpringGreen{0, 255, 127};
  static constexpr Color ForestGreen{34, 139, 34};
  static constexpr Color SeaGreen{46, 139, 87};
  static constexpr Color Blue{0, 0, 255};
  static constexpr Color DarkBlue{0, 0, 139};
  static constexpr Color MidnightBlue{25, 25, 112};
  static constexpr Color Navy{0, 0, 128};
  static constexpr Color MediumBlue{0, 0, 205};
  static constexpr Color Skyblue{135, 206, 235};
  static constexpr Color Cyan{0, 255, 255};
  static constexpr Color Magenta{255, 0, 255};
  static constexpr Color DarkTurquoise{0, 206, 209};
  static constexpr Color DarkPink{255, 20, 147};
  static constexpr Color Coral{255, 127, 80};
  static constexpr Color LightCoral{240, 128, 128};
  static constexpr Color OrangeRed{255, 69, 0};
  static constexpr Color Salmon{250, 128, 114};
  static constexpr Color DarkSalmon{233, 150, 122};
  static constexpr Color Khaki{240, 230, 140};
  static constexpr Color DarkKhaki{189, 183, 107};
  static constexpr Color DarkGoldenrod{184, 134, 11};
  static constexpr Color Beige{245, 245, 220};
  static constexpr Color Olive{160, 128, 32};
  static constexpr Color Orange{255, 165, 0};
  static constexpr Color Violet{238, 130, 238};
  static constexpr Color DarkViolet{148, 0, 211};
  static constexpr Color Plum{221, 160, 221};
  static constexpr Color DarkPlum{144, 80, 64};
  static constexpr Color DarkOlivegreen{85, 107, 47};
  static constexpr Color Orangered4{128, 20, 0};
  static constexpr Color Brown4{128, 20, 20};
  static constexpr Color Sienna4{128, 64, 20};
  static constexpr Color Orchid4{128, 64, 128};
  static constexpr Color Mediumpurple3{128, 96, 192};
  static constexpr Color Slateblue1{128, 96, 255};
  static constexpr Color Yellow4{128, 128, 0};
  static constexpr Color Sienna1{255, 128, 64};
  static constexpr Color Tan1{255, 160, 64};
  static constexpr Color Sandybrown{255, 160, 96};
  static constexpr Color LightSalmon{255, 160, 112};
  static constexpr Color Pink{255, 192, 192};
  static constexpr Color Khaki1{255, 255, 128};
  static constexpr Color Lemonchiffon{255, 255, 192};
  static constexpr Color Bisque{205, 183, 158};
  static constexpr Color Honeydew{240, 255, 240};
  static constexpr Color Slategrey{160, 182, 205};
  static constexpr Color Seagreen{193, 255, 193};
  static constexpr Color Antiquewhite{205, 192, 176};
  static constexpr Color Chartreuse{124, 255, 64};
  static constexpr Color Greenyellow{160, 255, 32};
  static constexpr Color Gray{190, 190, 190};
  static constexpr Color LightGray{211, 211, 211};
  static constexpr Color LightGrey{211, 211, 211};
  static constexpr Color DarkGray{160, 160, 160};
  static constexpr Color Slategray{160, 182, 205};
  static constexpr Color Gray0{0, 0, 0};
  static constexpr Color Gray10{26, 26, 26};
  static constexpr Color Gray20{51, 51, 51};
  static constexpr Color Gray30{77, 77, 77};
  static constexpr Color Gray40{102, 102, 102};
  static constexpr Color Gray50{127, 127, 127};
  static constexpr Color Gray60{153, 153, 153};
  static constexpr Color Gray70{179, 179, 179};
  static constexpr Color Gray80{204, 204, 204};
  static constexpr Color Gray90{229, 229, 229};
  static constexpr Color Gray100{255, 255, 255};
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
  if constexpr (I < std::tuple_size_v<T>) {
    fmt::format_to(std::back_inserter(buf), "{}", get<I>(t));
    if constexpr (I < std::tuple_size_v<T> - 1) {
      buf += ' ';
      append<T, I + 1>(buf, t);
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

  Series &plotStyle(PlotStyle plotStyle) {
    assert(m_series);
    if (m_series->plotStyle != plotStyle) {
      m_series->plotStyle = plotStyle;
      m_series->dirty = true;
    }
    return *this;
  }

  PlotStyle plotStyle() const {
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
