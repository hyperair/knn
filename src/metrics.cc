#include <cmath>
#include <metrics.hh>

double knn::metrics::euclidean (const entry &e1, const entry &e2)
{
    double sq_sum = 0.0;

    e1.visit ([&] (const entry::index_type index, const entry::value_type value)
              {
                  entry::value_type diff = e2[index] - value;
                  sq_sum += diff * diff;
              });

    return sq_sum;
}

double knn::metrics::cosine (const entry &e1, const entry &e2)
{
    entry::value_type numerator = 0;
    entry::value_type sq_sum1 = 0;
    entry::value_type sq_sum2 = 0;

    e1.visit ([&] (const entry::index_type index, const entry::value_type value)
              {
                  const entry::value_type value2 = e2[index];

                  numerator += value * value2;
                  sq_sum1 += value * value;
                  sq_sum2 += value2 * value2;
              });

    return double (numerator) / (std::sqrt (sq_sum1) * std::sqrt (sq_sum2));
}
