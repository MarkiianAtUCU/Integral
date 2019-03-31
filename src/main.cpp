#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <thread>
#include <atomic>
#include <iomanip>
#include <vector>
#include "time_meter.h"
#include "config_reader.h"


double f (double x1, double x2)
{
    double a = 20;
    double b = 0.2;
    double c = 2 * M_PI;
    return -a * exp(-b * sqrt(0.5 * (x1 * x1 + x2 * x2))) - exp(0.5 * (cos(c * x1) + cos(c * x2))) + a + exp(1);
}

void integral (double x_from, double x_to, double y_from, double y_to, double precision, std::atomic<double> &res)
{
    double local_res = 0;
    double x = x_from;
    while (x <= x_to) {
        double y = y_from;
        while (y <= y_to) {
            local_res += f(x, y);
            y += precision;
        }
        x += precision;
    }
    res = res + local_res;
}

double runner (double precission, int n_threads, double start_x, double end_x, double start_y, double end_y)
{
    std::atomic<double> res{0};

    double step = (end_x - start_x) / n_threads;
    std::vector<std::thread> threads;

    threads.reserve((size_t) n_threads);
    for (int i = 0; i < n_threads; ++i) {
        threads.emplace_back(integral, start_x + step * i,
                             start_x + step * (i + 1) - (i == n_threads - 1 ? 0 : precission), start_y, end_y,
                             precission, std::ref(res));
    }

    for (auto &t: threads) {
        t.join();
    }

    return res * precission * precission;
}

int main (int argc, char *argv[])
{

    if (argc != 2) {
        throw std::invalid_argument("Usage:  integral config_file\n"
                                    "           Where:  config file -- configuration file for program");
    }
    config *c = new config(argv[1]);

    int num_threads = c->get_int("num_threads");

    double ERR_abs = c->get_double("err_abs");
    double ERR_rel = c->get_double("err_rel");
    double start_x = c->get_double("x_start");
    double end_x = c->get_double("x_end");
    double start_y = c->get_double("y_start");
    double end_y = c->get_double("y_end");


    double prev = 99;
    double now = -99;
    double precision = 0.4;

    auto start = get_current_time_fenced();
    while (!(std::abs(prev - now) <= ERR_abs && std::abs(prev - now) / prev <= ERR_rel)) {
        precision /= 2;
        prev = now;
        now = runner(precision, num_threads, start_x, end_x, start_y, end_y);
    }
    auto end = get_current_time_fenced();
    long long time = to_us(end - start);

    std::cout << "Res: " << std::setprecision(7) << now << std::endl;
    std::cout << "ABS_err: " << std::setprecision(7) << std::abs(prev - now) << std::endl;
    std::cout << "REL_err: " << std::setprecision(7) << std::abs(prev - now) / prev << std::endl;
    std::cout << "T: " << time << std::endl;
    return 0;
}

