#ifndef CTH_NODE_STATS_HH
#define CTH_NODE_STATS_HH

#include <metrics.hh>
#include <cluster-tree.hh>
#include <vector>

namespace knn
{
    namespace cluster_tree_helpers
    {
        // Helper class to compute node localities
        class node_stats
        {
        public:
            typedef cluster_tree::nodeptr nodeptr;

            node_stats (const nodeptr &node, metric_type metric);

            void insert (const nodeptr &node);

            std::set<nodeptr> phi () const;
            double gamma () const   {return _gamma;}
            int phisize () const    {return nodes.size ();}

            void trim ();

        private:
            const nodeptr main_node;
            std::vector<std::pair<double, nodeptr> > nodes;
            double _gamma;

            knn::metric_type metric;
        };
    }
}

#endif  // CTH_NODE_STATS_HH
