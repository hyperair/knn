#include <functional>
#include <cassert>
#include <classifier.hh>

using knn::classifier;
using knn::simple_classifier;
using knn::tree_classifier;

namespace {
    class distances_list
    {
    public:
        distances_list (int k);
        void insert (double distance, classifier::class_type clss);

        classifier::class_type most_frequent_class () const;

    private:
        const int k;
        std::list<std::pair<double, classifier::class_type> > distances;
    };
}

distances_list::distances_list (const int k) :
    k (k)
{}

void distances_list::insert (const double distance,
                             const classifier::class_type clss)
{
    auto i = distances.begin ();
    int j = 0;

    // Insert sorted
    while (i != distances.end () && j < k && i->first < distance) {
        ++i;
        ++j;
    }

    if (j >= k)
        return;

    i = distances.insert (i, {distance, clss});

    // Find (k-1)'th element
    while (++i != distances.end () && ++j < k);

    // Delete the rest
    distances.erase (i, distances.end ());
}

classifier::class_type distances_list::most_frequent_class () const
{
    std::map<classifier::class_type, int> votes;

    assert (!distances.empty ());

    for (auto i : distances)
        ++votes[i.second];

    classifier::class_type retval = 0;
    int max_votes = 0;

    for (auto i : votes)
        if (max_votes < i.second) {
            max_votes = i.second;
            retval = i.first;
        }

    return retval;
}


classifier::classifier (const int k, metric_type metric) :
    k (k), metric (std::move (metric))
{}

classifier::~classifier () {}


simple_classifier::simple_classifier (const int k, metric_type metric,
                                      const dataset &data) :
    classifier (k, std::move (metric)),
    data (data)
{}

simple_classifier::~simple_classifier () {}

classifier::class_type simple_classifier::classify (const entry &e2) const
{
    distances_list distances (k);

    data.visit ([&] (const entry &e1, class_type clss)
                {
                    distances.insert (metric (e1, e2), clss);
                });

    return distances.most_frequent_class ();
}


tree_classifier::tree_classifier (const int k, metric_type metric,
                                  const dataset &data) :
    classifier (k, std::move (metric))
{
    // TODO: index
}

tree_classifier::~tree_classifier () {}

classifier::class_type tree_classifier::classify (const entry &e) const
{
    // TODO: traverse tree
}
