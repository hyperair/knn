#include <functional>
#include <classifier.hh>

using knn::classifier;

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


classifier::classifier (const int k, metric_type metric,
                        const dataset &data) :
    k (k), metric (std::move (metric)), data (data)
{}

classifier::class_type classifier::classify (const entry &e2) const
{
    distances_list distances (k);

    data.visit ([&] (const entry &e1, class_type clss)
                {
                    distances.insert (metric (e1, e2), clss);
                });

    return distances.most_frequent_class ();
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
    while (i != distances.end () && ++j < k && i->first < distance)
        ++i;

    if (j >= k)
        return;

    distances.insert (i, {distance, clss});

    // Find (k-1)'th element
    while (++i != distances.end () && ++j < k);

    // Delete the rest
    distances.erase (i, distances.end ());
}

classifier::class_type distances_list::most_frequent_class () const
{
    std::map<classifier::class_type, int> votes;

    for (auto i : distances)
        ++votes[i.second];

    classifier::class_type retval;
    int max_votes = 0;

    for (auto i : votes)
        if (max_votes < i.second) {
            max_votes = i.second;
            retval = i.first;
        }

    return retval;
}
