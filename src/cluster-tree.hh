#ifndef CLUSTER_TREE_HH
#define CLUSTER_TREE_HH

#include <memory>
#include <dataset.hh>
#include <metrics.hh>

namespace knn
{
    class cluster_tree
    {
    public:
        class node;
        typedef std::shared_ptr<node> nodeptr;

        cluster_tree (const dataset &data, metric_type metric);

        dataset::class_type classify (const entry &e) const;

    private:
        const metric_type metric;
    };
}

#endif  // CLUSTER_TREE_HH
