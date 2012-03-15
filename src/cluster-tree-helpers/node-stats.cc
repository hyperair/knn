#include <cluster-tree-helpers/node-stats.hh>

using knn::cluster_tree_helpers::node_stats;

node_stats::node_stats (const nodeptr &node, metric_type metric) :
    main_node (node), metric (std::move (metric)) {}

void node_stats::insert (const nodeptr &node)
{
    const double distance = metric (node->entry_ (), main_node->entry_ ());

    if (node->class_ () == main_node->class_ ()) {
        for (auto i = nodes.begin (); i != nodes.end (); ++i)
            if (i->first > distance) {
                nodes.insert (i, {distance, node});
                break;
            }

    } else {
        if (distance < _gamma) {
            _gamma = distance;
            trim ();
        }
    }
}

std::set<node_stats::nodeptr> node_stats::phi () const
{
    std::set<nodeptr> phi;

    for (auto &i : nodes)
        phi.insert (i.second);

    return phi;
}

void node_stats::trim ()
{
    auto i = nodes.begin ();
    for (; i != nodes.end (); ++i)
        if (_gamma < i->first)
            break;

    nodes.erase (i, nodes.end ());
}