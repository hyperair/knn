#ifndef CTH_CLUSTER_HH
#define CTH_CLUSTER_HH

#include <set>
#include <map>
#include <cluster-tree.hh>
#include <dataset.hh>
#include <entry.hh>

namespace knn
{
    namespace cluster_tree_helpers
    {
        // Helper for generating clusters
        class cluster
        {
        public:
            cluster (cluster_tree::nodeptr node, double radius,
                     metric_type metric) :
                nodes {std::move (node)},
                radius (radius),
                class_ (node->class_ ()),
                metric (std::move (metric))
            {}

            bool insert (const cluster_tree::nodeptr &node);
            cluster_tree::nodeptr centroid () const;

        private:
            std::set<cluster_tree::nodeptr> nodes;
            std::map<entry::index_type, entry::value_type> sums;
            const double radius;
            const dataset::class_type class_;
            metric_type metric;
        };

    }
}
#endif  // CTH_CLUSTER_HH
