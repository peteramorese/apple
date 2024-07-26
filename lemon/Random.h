#pragma once

#include <random>

#include "Options.h"

namespace lemon {

class RNG {
    public:
        /// @brief Random integer unique ID
        /// @return Random uint64_t between 0 and INT64_MAX
        LMN_INL static uint64_t uuid64();

        /// @brief Random (unbounded) int32
        /// @return Random int32_t between INT32_MIN and INT32_MAX
        LMN_INL static int32_t randiUnbounded();

        /// @brief Random (unbounded) int64
        /// @return Random int64_t between INT64_MIN and INT64_MAX
        LMN_INL static int64_t randiUnbounded64();

        /// @brief Seeded random (unbounded) int32
        /// @return Random int32_t between INT32_MIN and INT32_MAX
        LMN_INL static int32_t srandiUnbounded();

        /// @brief Seeded random (unbounded) int64
        /// @return Random int64_t between INT64_MIN and INT64_MAX
        LMN_INL static int64_t srandiUnbounded64();

        /// @brief Random int32 in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand int in range [lower upper)
        LMN_INL static int32_t randi(int32_t lower, int32_t upper);

        /// @brief Random int64 in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand int in range [lower upper)
        LMN_INL static int64_t randi(int64_t lower, int64_t upper);

        /// @brief Random floating point in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand float in range [lower upper)
        LMN_INL static float randf(float lower, float upper);

        /// @brief Random double in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand double in range [lower upper)
        LMN_INL static double randd(double lower, double upper);

        /// @brief Seeded random int32 in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand int in range [lower upper)
        LMN_INL static int32_t srandi(int32_t lower, int32_t upper);

        /// @brief Seeded random int64 in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand int in range [lower upper)j
        LMN_INL static int64_t srandi(int64_t lower, int64_t upper);

        /// @brief Seeded random floating point in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand float in range [lower upper)
        LMN_INL static float srandf(float lower, float upper);

        /// @brief Seeded random double in a range [lower, upper)
        /// @param lower Closed lower bound
        /// @param upper Open upper bound
        /// @return Rand double in range [lower upper)
        LMN_INL static double srandd(double lower, double upper);

        /// @brief Seed all `srand` functions (both 32 and 64)
        /// @param s Seed
        LMN_INL static void seed(uint32_t s);

        /// @brief Normally distributed random floating point
        /// @param mean Mean
        /// @param std Standard deviation
        /// @return Normally distributed float sample
        LMN_INL static float nrandf(float mean = 0.0f, float std = 1.0f);

        /// @brief Normally distributed random double
        /// @param mean Mean
        /// @param std Standard deviation
        /// @return Normally distributed double sample
        LMN_INL static double nrandd(double mean = 0.0, double std = 1.0);

        /// @brief Normally distributed seeded random floating point
        /// @param mean Mean
        /// @param std Standard deviation
        /// @return Normally distributed float sample
        LMN_INL static float nsrandf(float mean = 0.0, float std = 1.0);

        /// @brief Normally distributed seeded random double
        /// @param mean Mean
        /// @param std Standard deviation
        /// @return Normally distributed double sample
        LMN_INL static double nsrandd(double mean = 0.0, double std = 1.0);

    private:
        LMN_INL static std::random_device& s_rd();
        LMN_INL static std::mt19937& s_random_gen();
        LMN_INL static std::mt19937_64& s_random_gen_64();
        LMN_INL static std::mt19937& s_seeded_gen();
        LMN_INL static std::mt19937_64& s_seeded_gen_64();
        LMN_INL static std::uniform_real_distribution<float>& s_real_dist();
        LMN_INL static std::uniform_real_distribution<double>& s_real_dist_64();
        LMN_INL static std::uniform_int_distribution<>& s_int_dist();
};

}

#include "impl/Random_impl.hpp"