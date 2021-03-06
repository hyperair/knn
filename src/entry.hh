#ifndef ENTRY_HH
#define ENTRY_HH

#include <set>
#include <unordered_map>
#include <exception>
#include <functional>
#include <iostream>

namespace knn
{
    class entry
    {
    public:
        typedef int index_type;
        typedef double value_type;

        explicit entry (std::unordered_map<index_type, value_type> values);

        value_type operator[] (index_type index) const;
        value_type &operator[] (index_type index);

        template <typename Functor>
        void visit (const Functor &functor) const;

        template <typename Iterator>
        void expand_dimensions (Iterator begin, const Iterator &end);

    private:
        std::unordered_map<index_type, value_type>  values;
    };

    std::ostream &operator<< (std::ostream &out, const entry &e);
}

// Template implementation
template <typename Functor>
void knn::entry::visit (const Functor &functor) const
{
    for (const std::pair<index_type, value_type> &i : values)
        functor (i.first, i.second);
}

template <typename Iterator>
inline void knn::entry::expand_dimensions (Iterator begin, const Iterator &end)
{
    for (; begin != end; ++begin)
        if (values.find (*begin) == values.end ())
            values.insert ({*begin, 0});
}

#endif  // ENTRY_HH
