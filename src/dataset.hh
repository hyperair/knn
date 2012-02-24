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
        dataset ();

        void insert (entry e, int clss);

        void visit (std::function<void(entry, int)> functor);

        std::set<int> dimensions () {return _dimensions;}

    private:
        std::list<std::pair<entry, int> > entries;
        std::set<int> _dimensions;
    };
}


#endif  // DATASET_HH
