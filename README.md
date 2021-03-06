# gnuplotxx

[![Build Status](https://travis-ci.com/falbrechtskirchinger/gnuplotxx.svg?branch=master)](https://travis-ci.com/falbrechtskirchinger/gnuplotxx)

gnuplotxx is a single-header C++20 gnuplot (Qt terminal) driver for your quick & dirty data visualization needs.

# Usage

Add the header file `gnuplotxx.h` (located in include/) to your project.  
Until C++20's `<format>` is available, we depend on [fmtlib]. Make sure you either have `<fmt/format.h>` in your include path or include this header manually, before including `gnuplotxx.h`.

Create a plot window:
```cpp
auto plot = Plot("<Window Title>");
```

Create a data series:
```cpp
auto series = plot.createSeries("<Data Series Title>");
```

Append data:
```cpp
series.append(x, y);
```

The plot is not automatically (re-)drawn. To display your data, use:
```cpp
plot.update();
```

See `src/demo1.cpp` for a complete example.

# ChaiScript

The header `gnuplotxx-chaiscript.h` (also located in include/) offers integration with [ChaiScript].
See `src/chaiscript.cpp` for a demonstration of how to use it.


[fmtlib]: <https://github.com/fmtlib/fmt>
[ChaiScript]: <https://github.com/ChaiScript/ChaiScript>
