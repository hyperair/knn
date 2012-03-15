#include <cluster-tree.hh>

using knn::cluster_tree;
typedef cluster_tree::nodeptr nodeptr;

class cluster_tree::node
{
public:
    node (entry entry, const dataset::class_type clss) :
        _entry (std::move (entry)), _class (clss), _gamma (0.0) {}

    double gamma () const                       {return _gamma;}
    void gamma (double v)                       {_gamma = v;}

    const entry &entry_ () const                {return _entry;}

    const std::set<nodeptr> &phi () const       {return _phi;}
    void phi (std::set<nodeptr> newphi)         {_phi = std::move (newphi);}

    dataset::class_type class_ () const   {return _class;}

private:
    const entry               _entry;
    const dataset::class_type _class;
    double                    _gamma; // distance to nearest distinct node
    std::set<nodeptr>         _phi;   // nodes nearer than gamma
};


namespace {
    class node_stats
    {
    public:
        node_stats (const nodeptr &node, knn::metric_type metric);

        void insert (const nodeptr &node);

        std::set<nodeptr> phi () const;
        double gamma () const                   {return _gamma;}

    private:
        const nodeptr main_node;
        std::list<std::pair<double, nodeptr> > nodes;
        double _gamma;

        knn::metric_type metric;

        void trim ();
    };
}

node_stats::node_stats (const nodeptr &node, knn::metric_type metric) :
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

std::set<nodeptr> node_stats::phi () const
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


cluster_tree::cluster_tree (const dataset &data, metric_type m) :
    metric (std::move (m))
{
    std::set<nodeptr> nodes;

    data.visit ([&] (const entry &e, const dataset::class_type c)
                {
                    nodes.insert (nodeptr (new node (e, c)));
                });

    // Step 1: Compute localities of all nodes
    for (const nodeptr &n1 : nodes) {
        node_stats stats (n1, metric);

        for (const nodeptr &n2 : nodes)
            if (n1 != n2)
                stats.insert (n2);

        n1->gamma (stats.gamma ());
        n1->phi (stats.phi ());
    }

    // Step 2:
    // TODO:
}

knn::dataset::class_type cluster_tree::classify (const entry &) const
{
    // TODO: Implement
    return 0;
}
