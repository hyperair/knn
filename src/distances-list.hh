#ifndef DISTANCES_LIST_HH
#define DISTANCES_LIST_HH

#include <list>
#include <map>

namespace knn
{
    // Class that manages a sorted list of distances and a payload of type T
    // It automatically trims the length to the limit speicfied in the
    // constructor
    template <typename T>
    class distances_list
    {
    public:
        distances_list (int limit) : limit (limit) {}
        void insert (double distance, T clss);

        T most_frequent () const;

        template <typename Functor>
        void visit (const Functor &f) const;

    private:
        const int limit;
        std::list<std::pair<double, T> > distances;
    };
}


// Template function definitions
template <typename T>
void knn::distances_list<T>::insert (double distance, T clss)
{
    auto i = distances.begin ();

    int j = 0;

    while (i != distances.end () && j < limit && i->first < distance) {
        ++i;
        ++j;
    }

    if (j >= limit)
        return;

        i = distances.insert (i, {distance, clss});

    // Find (limit-1)'th element
    while (++i != distances.end () && ++j < limit);

    // Delete the rest
    distances.erase (i, distances.end ());
}

template <typename T>
T knn::distances_list<T>::most_frequent () const
{
    std::map<T, int> votes;

    assert (!distances.empty ());

    for (auto i : distances)
        ++votes[i.second];

    const T *retval = 0;
    int max_votes = 0;

    for (auto i : votes)
        if (max_votes < i.second) {
            max_votes = i.second;
            retval = &i.first;
        }

    return *retval;
}


template <typename T>
template <typename Functor>
void knn::distances_list<T>::visit (const Functor &f) const
{
    for (auto &i : distances)
        f (i.first, i.second);
}

#endif  // DISTANCES_LIST_HH
