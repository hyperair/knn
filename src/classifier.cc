#include <functional>
#include <cassert>
#include <classifier.hh>
#include <distances-list.hh>

using knn::classifier;
using knn::simple_classifier;
using knn::tree_classifier;
typedef knn::distances_list<classifier::class_type> distances_list;


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
    ::distances_list distances (k);

    data.visit ([&] (const entry &e1, class_type clss)
                {
                    distances.insert (metric (e1, e2), clss);
                });

    return distances.most_frequent ();
}


tree_classifier::tree_classifier (const int k, metric_type metric,
                                  const dataset &data) :
    classifier (k, std::move (metric)),
    tree (data, this->metric)
{}

tree_classifier::~tree_classifier () {}

classifier::class_type tree_classifier::classify (const entry &e) const
{
    return tree.classify (e);
}
