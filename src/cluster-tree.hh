#ifndef CLUSTER_TREE_HH
#define CLUSTER_TREE_HH

#include <memory>
#include <set>
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
        nodeptr root;
    };

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
}

#endif  // CLUSTER_TREE_HH
