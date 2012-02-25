#ifndef CLASSIFY_HH
#define CLASSIFY_HH

#include <functional>
#include <dataset.hh>

namespace knn
{
    class classifier
    {
    public:
        typedef std::function<double (const entry &, const entry &)>
        metric_type;
        typedef dataset::class_type class_type;

        classifier (int k, metric_type metric, const dataset &data);

        class_type classify (const entry &e) const;

    private:
        const int k;
        const metric_type metric;
        const dataset &data;
    };
}

#endif  // CLASSIFY_HH
