#pragma once

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

struct Utilities {
    template <typename T>
    static auto Sigmoid(T val) {
        return 1. / (1. + exp(-val));
    }

    template <typename T>
    static auto SoftMax(const T* classes, size_t number_of_classes) {
        auto max = *max_element(classes, classes + number_of_classes);
        double sum = accumulate(classes, classes + number_of_classes, 0.0, [max](auto current_sum, auto current_element) {
            return current_sum + exp(current_element - max);
            });
        double term = log(sum);

        vector<T> result;
        result.reserve(number_of_classes);
        transform(classes, classes + number_of_classes, back_inserter(result), [max, term](auto current_element) {
            return static_cast<T>(exp(current_element - max - term));
            });
        return result;
    }

    template <typename T>
    static auto SoftMax(const vector<T>& classes) {
        return SoftMax(classes.data(), classes.size());
    }

    template <typename T>
    static auto Logistic(const vector<T>& classes) {
        vector<T> result;
        transform(classes.begin(), classes.end(), back_inserter(result), [](auto current_element) {
            return Sigmoid(current_element);
            });
        return result;
    }

};

