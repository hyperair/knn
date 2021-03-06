#ifndef CLASSIFY_HH
#define CLASSIFY_HH

#include <functional>
#include <metrics.hh>
#include <dataset.hh>
#include <cluster-tree.hh>

namespace knn
{
    class classifier
    {
    public:
        typedef dataset::class_type class_type;

        classifier (int k, metric_type metric);
        virtual ~classifier ();

        virtual class_type classify (const entry &e) const = 0;

    protected:
        const int k;
        const metric_type metric;
    };

    class simple_classifier : public classifier
    {
    public:
        simple_classifier (int k, metric_type metric, const dataset &data);
        virtual ~simple_classifier ();

        virtual class_type classify (const entry &e) const;

    private:
        const dataset &data;
    };

    class tree_classifier : public classifier
    {
    public:
        tree_classifier (int k, metric_type metric, const dataset &data);
        virtual ~tree_classifier ();

        virtual class_type classify (const entry &e) const;

    private:
        cluster_tree tree;
    };
}

#endif  // CLASSIFY_HH
