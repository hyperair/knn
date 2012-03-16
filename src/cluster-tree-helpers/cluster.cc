#include <cluster-tree-helpers/cluster.hh>

using knn::cluster_tree_helpers::cluster;
typedef knn::cluster_tree::nodeptr nodeptr;

bool cluster::insert (const nodeptr &node)
{
    auto backup_sums = sums;
    auto backup_nodes = nodes;

    node->entry_ ().visit ([&] (entry::index_type i, entry::value_type v)
                          {
                              sums[i] += v;
                          });
    nodes.insert (node);

    // Check if radius is exceeded
    nodeptr c = centroid ();

    for (const auto &n : nodes) {
        if (radius >= metric (n->entry_ (), c->entry_ ()))
            continue;

        // Roll back
        sums = std::move (backup_sums);
        nodes = std::move (nodes);

        return false;
    }

    return true;
}

nodeptr cluster::centroid () const
{
    decltype(sums) values;

    for (const auto &i : sums)
        values.insert ({i.first, i.second / nodes.size ()});

    return nodeptr (new cluster_tree::node (entry (std::move (values)),
                                            class_, true));
}
