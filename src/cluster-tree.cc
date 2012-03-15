#include <cluster-tree.hh>

using knn::cluster_tree;
typedef cluster_tree::nodeptr nodeptr;

class cluster_tree::node
{
public:
    node (entry entry, const dataset::class_type clss) :
        _entry (std::move (entry)), _class (clss) {}

    // some accessor functions
    const entry &entry_ () const {return _entry;}
    dataset::class_type class_ () const {return _class;}
    const std::set<nodeptr> &children () const {return _children;}

    void insert_child (const nodeptr &node) {_children.insert (node);}

private:
    const entry               _entry;
    const dataset::class_type _class;

    std::set<nodeptr>         _children;
};


namespace {
    class node_stats
    {
    public:
        node_stats (const nodeptr &node, knn::metric_type metric);

        void insert (const nodeptr &node);

        std::set<nodeptr> phi () const;
        double gamma () const                   {return _gamma;}
        int phisize () const;          {return nodes.size ();}

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


namespace {
    std::map<nodeptr, node_stats>
    calculate_localities (std::set<nodeptr> &nodes)
    {
        std::map<nodeptr, node_stats> localities;

        for (const nodeptr &n1 : nodes) {
            node_stats stats (n1, metric);

            for (const nodeptr &n2 : nodes)
                if (n1 != n2)
                    stats.insert (n2);

            assert (localities.insert (std::move (n1),
                                       std::move (stats)).second);
        }

        return localities;
    }

    nodeptr build_hypernode (std::set<nodeptr> nodes,
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
        auto localities = calculate_localities (nodes);

        bool inserted =
            hypernodes.insert (build_hypernode (nodes, localities)).second;

        assert (inserted);
    }

    // Cluster the hypernodes
    // TODO: Implement
}

knn::dataset::class_type cluster_tree::classify (const entry &) const
{
    // TODO: Implement
    return 0;
}
