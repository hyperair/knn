#include <cassert>
#include <numeric>
#include <vector>
#include <stats.hh>
#include <cluster-tree.hh>
#include <cluster-tree-helpers/cluster.hh>
#include <cluster-tree-helpers/node-stats.hh>

using knn::cluster_tree;
using namespace knn::cluster_tree_helpers;
using knn::entry;

typedef cluster_tree::nodeptr nodeptr;

namespace {
    std::map<nodeptr, node_stats>
    calculate_localities (std::set<nodeptr> &nodes,
                          const knn::metric_type &metric)
    {
        std::map<nodeptr, node_stats> localities;

        for (const nodeptr &n1 : nodes) {
            node_stats stats (n1, metric);

            for (const nodeptr &n2 : nodes)
                if (n1 != n2)
                    stats.insert (n2);

            bool inserted =
                localities.insert ({std::move (n1), std::move (stats)}).second;

            assert (inserted);
        }

        return localities;
    }

    nodeptr build_hypernode (std::set<nodeptr> &nodes,
                             std::map<nodeptr, node_stats> &localities)
    {
        unsigned int max_phisize = 0;
        nodeptr hypernode;
        std::set<nodeptr> phi;

        for (const auto &i : localities) {
            unsigned int phisize = i.second.phisize ();

            if (max_phisize < phisize) {
                max_phisize = phisize;
                hypernode = i.first;
                phi = i.second.phi ();
            }
        }

        assert (hypernode);

        for (const nodeptr &i : phi) {
            hypernode->insert_child (i);
            nodes.erase (i);
        }

        return hypernode;
    }

    double get_cluster_radius (std::set<nodeptr> &nodes,
                               const knn::metric_type &metric,
                               int level)
    {
        std::set<std::pair<nodeptr, nodeptr> > visited;

        // Construct list of distances
        std::vector<double> distances;

        for (const nodeptr &n1 : nodes) {
            for (const nodeptr &n2 : nodes) {
                if (visited.find ({n2, n1}) != visited.end ())
                    continue;

                distances.push_back (metric (n1->entry_ (), n2->entry_ ()));
                visited.insert ({n1, n2});
            }
        }

        double mean = knn::stats::mean (distances.begin (), distances.end ());
        double stdev = knn::stats::stdev (distances.begin (), distances.end (),
                                          mean);

        return mean - 2 * stdev / level;
    }

    std::set<nodeptr> make_clusters (std::set<nodeptr> nodes, double radius)
    {
        std::vector<cluster> clusters;

        for (const nodeptr &n : nodes) {
            
        }

        std::set<nodeptr> centroids;

        for (const cluster &c : clusters)
            centroids.insert (c.centroid ());

        return centroids;
    }
}

cluster_tree::cluster_tree (const dataset &data, metric_type m) :
    metric (std::move (m))
{
    // Construct set of nodes
    std::set<nodeptr> nodes;
    data.visit ([&] (const entry &e, const dataset::class_type c)
                {
                    nodes.insert (nodeptr (new node (e, c)));
                });

    // Construct initial hypernodes (subtrees)
    std::set<nodeptr> hypernodes;
    while (!nodes.empty ()) {
        auto localities = calculate_localities (nodes, metric);

        bool inserted =
            hypernodes.insert (build_hypernode (nodes, localities)).second;

        assert (inserted);
    }

    assert (!hypernodes.empty ());

    // Cluster the hypernodes
    std::set<nodeptr> current_level = std::move (hypernodes);
    int i = 1;

    while (current_level.size () > 1) {
        std::set<nodeptr> next_level;

        double radius = get_cluster_radius (current_level, metric, ++i);
        current_level = make_clusters (std::move (current_level), radius);
    }

    assert (current_level.size () == 1);
}

knn::dataset::class_type cluster_tree::classify (const entry &) const
{
    // TODO: Implement
    return 0;
}
