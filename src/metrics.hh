#ifndef METRICS_HH
#define METRICS_HH

#include <entry.hh>

namespace knn
{
    namespace metrics
    {
        double euclidean (const entry &e1, const entry &e2);
        double cosine (const entry &e1, const entry &e2);
    }

    typedef std::function<double (const entry &, const entry &)> metric_type;
}

#endif  // METRICS_HH
