#ifndef STATS_HH
#define STATS_HH

#include <numeric>
#include <iterator>
#include <cmath>

namespace knn
{
    namespace stats
    {
        template <typename Iterator>
        double mean (Iterator begin, Iterator end)
        {
            auto sum = std::accumulate (begin, end, 0.0l);

            return double (sum) / std::distance (begin, end);
        }

        template <typename Iterator>
        double stdev (Iterator begin, Iterator end, double mean)
        {
            double sqdiffsum = 0;

            for (Iterator i = begin; i != end; ++i) {
                double diff = *i - mean;

                sqdiffsum += diff * diff;
            }

            return std::sqrt (sqdiffsum / std::distance (begin, end));
        }
    }
}

#endif  // STATS_HH
