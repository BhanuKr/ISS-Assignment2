#include <cstddef>
#include <iomanip>
#include <iostream>

#include "../config.hpp"

int main() {
    timer::time_point start = timer::now();

    double integral = 0.0;
    for (size_t i = 0; i < STEPS; i += 1) {
        double x = i * dx;

        integral += func(x) * dx;
    }

    timer::time_point end = timer::now();

    std::cout << "Result: " << std::setprecision(17) << integral << std::endl;
    std::cout << "Time:   " << get_time_ms(end - start) << " seconds" << std::endl;

    return 0;
}
