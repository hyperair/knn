#ifndef DATASET_HH
#define DATASET_HH

#include <functional>
#include <list>
#include <memory>

#include <entry.hh>

namespace knn
{
    class dataset
    {
    public:
        typedef entry::index_type index_type;
        typedef entry::value_type value_type;
        typedef int class_type;

        dataset ();
        dataset (const dataset &);
        dataset (dataset &&) = default;

        dataset &operator= (dataset);

        void insert (entry e, class_type clss);

        void visit (const std::function<void (const entry &,
                                              class_type)> &functor) const;
        void visit (const std::function<void (entry &, class_type)> &functor);


        const std::set<index_type> &dimensions () const {return *_dimensions;}

    private:
        std::list<std::pair<entry, class_type> > entries;
        std::unique_ptr<std::set<index_type> > _dimensions;
    };
}


#endif  // DATASET_HH
