#pragma once

#include "Random.h"

#include "Logging.h"


std::random_device& lemon::RNG::s_rd() {static std::random_device rd; return rd;}
std::mt19937& lemon::RNG::s_random_gen() {static thread_local std::mt19937 gen(s_rd()()); return gen;}
std::mt19937_64& lemon::RNG::s_random_gen_64() {static thread_local std::mt19937_64 gen(s_rd()()); return gen;}
std::mt19937& lemon::RNG::s_seeded_gen() {static thread_local std::mt19937 gen(123); return gen;}
std::mt19937_64& lemon::RNG::s_seeded_gen_64() {static thread_local std::mt19937_64 gen(123); return gen;}
std::uniform_real_distribution<float>& lemon::RNG::s_real_dist() {static std::uniform_real_distribution<float> real_dist(0.0f, 1.0f); return real_dist;}
std::uniform_real_distribution<double>& lemon::RNG::s_real_dist_64() {static std::uniform_real_distribution<double> real_dist(0.0f, 1.0f); return real_dist;}
std::uniform_int_distribution<>& lemon::RNG::s_int_dist() {static std::uniform_int_distribution<> int_dist(INT32_MIN, INT32_MAX); return int_dist;}

uint64_t lemon::RNG::uuid64() {
    return s_int_dist()(s_random_gen_64());
}

int32_t lemon::RNG::randiUnbounded() {
    return s_int_dist()(s_random_gen());
}

int64_t lemon::RNG::randiUnbounded64() {
    return s_int_dist()(s_random_gen_64());
}

int32_t lemon::RNG::srandiUnbounded() {
    return s_int_dist()(s_seeded_gen());
}

int64_t lemon::RNG::srandiUnbounded64() {
    return s_int_dist()(s_seeded_gen_64());
}

int32_t lemon::RNG::randi(int32_t lower, int32_t upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    int diff = (upper - lower);
    return ((randiUnbounded() % diff) + diff) % diff + lower;
}

int64_t lemon::RNG::randi(int64_t lower, int64_t upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    int diff = (upper - lower);
    return ((randiUnbounded64() % diff) + diff) % diff + lower;
}

float lemon::RNG::randf(float lower, float upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    return (upper - lower) * s_real_dist()(s_random_gen()) + lower;
}

double lemon::RNG::randd(double lower, double upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    return (upper - lower) * s_real_dist_64()(s_random_gen()) + lower;
}

int32_t lemon::RNG::srandi(int32_t lower, int32_t upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    int diff = (upper - lower);
    return ((srandiUnbounded() % diff) + diff) % diff + lower;
}

int64_t lemon::RNG::srandi(int64_t lower, int64_t upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    int diff = (upper - lower);
    return ((srandiUnbounded64() % diff) + diff) % diff + lower;
}

float lemon::RNG::srandf(float lower, float upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    return (upper - lower) * s_real_dist()(s_seeded_gen()) + lower;
}

double lemon::RNG::srandd(double lower, double upper) {
    ASSERT(lower <= upper, "Upper bound must be geq to lower bound");
    return (upper - lower) * s_real_dist_64()(s_seeded_gen()) + lower;
}

void lemon::RNG::seed(uint32_t s) {
    s_seeded_gen().seed(s);
    s_seeded_gen_64().seed(s);
}

float lemon::RNG::nrandf(float mean, float std) {
    std::normal_distribution<float> dist(mean, std);
    return dist(s_random_gen());
}

double lemon::RNG::nrandd(double mean, double std) {
    std::normal_distribution<double> dist(mean, std);
    return dist(s_random_gen());
}

float lemon::RNG::nsrandf(float mean, float std) {
    std::normal_distribution<float> dist(mean, std);
    return dist(s_seeded_gen());
}

double lemon::RNG::nsrandd(double mean, double std) {
    std::normal_distribution<double> dist(mean, std);
    return dist(s_seeded_gen());
}
