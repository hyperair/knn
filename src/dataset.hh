#ifndef DATASET_HH
#define DATASET_HH

#include <functional>
#include <list>
#include <memory>
#include <iostream>

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

        template <typename Functor>
        void visit (const Functor &functor) const;

        template <typename Functor>
        void visit (const Functor &functor);


        const std::set<index_type> &dimensions () const {return *_dimensions;}

        size_t size () const {return entries.size ();}

    private:
        std::list<std::pair<entry, class_type> > entries;
        std::unique_ptr<std::set<index_type> > _dimensions;
    };

    std::ostream &operator<< (std::ostream &out, const dataset &data);
}

// inline functions
template <typename Functor>
inline void knn::dataset::visit (const Functor &functor) const
{
    for (const std::pair<entry, class_type> &i : entries)
        functor (i.first, i.second);
}

template <typename Functor>
inline void knn::dataset::visit (const Functor &functor)
{
    for (std::pair<entry, class_type> &i : entries)
        functor (i.first, i.second);
}


#endif  // DATASET_HH
