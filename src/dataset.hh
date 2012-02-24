#ifndef DATASET_HH
#define DATASET_HH

#include <functional>
#include <list>

#include <entry.hh>

namespace knn
{
    class dataset
    {
    public:
        void insert (entry e, int clss);

        void
        visit (const std::function<void(const entry &, int)> &functor) const;

        std::set<int> dimensions () const {return _dimensions;}

    private:
        std::list<std::pair<entry, int> > entries;
        std::set<int> _dimensions;
    };
}


#endif  // DATASET_HH
