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
                nodes {node},
                radius (radius),
                class_ (node->class_ ()),
                metric (std::move (metric))
            {
                node->entry_ ()
                    .visit ([&] (entry::index_type i, entry::value_type v)
                            {
                                sums.insert ({i, v});
                            });
                assert (!nodes.empty ());
            }

            bool insert (const cluster_tree::nodeptr &node);
            cluster_tree::nodeptr centroid () const;

            template <typename Functor>
            void visit_nodes (Functor functor) const
            {
                for (const auto &i : nodes)
                    functor (i);
            }

        private:
            std::set<cluster_tree::nodeptr> nodes;
            std::unordered_map<entry::index_type, entry::value_type> sums;
            double radius;
            dataset::class_type class_;
            metric_type metric;
        };

    }
}
#endif  // CTH_CLUSTER_HH
