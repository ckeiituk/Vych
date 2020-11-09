#include "matplotlibcpp.h"
#include <cmath>

namespace plt = matplotlibcpp;

int main()
{
    int n = 5000;
    std::vector<double> x(n), y(n);
    for(int i=0; i<n; ++i) {
        double step = i;
        x.at(i) = step;
        y.at(i) = sqrt(double(step));
    }

    plt::figure_size(1200, 780);
    plt::named_plot("√x", x, y, "r");
    plt::xlim(0, 200);
    plt::ylim(0, 15);
    plt::title("Построение графика функции");
    plt::legend();
    plt::save("./test.png");
    plt::show();
}