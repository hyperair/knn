#include <metrics.hh>

double knn::metrics::euclidean (const entry &e1, const entry &e2)
{
    double sq_sum;

    e1.visit ([&] (const entry::index_type index, const entry::value_type value)
              {
                  entry::value_type diff = e2[index] - value;
                  sq_sum += diff * diff;
              });

    return sq_sum;
}

double knn::metrics::cosine (const entry &, const entry &)
{
    // TODO: Implement
    return double ();
}
