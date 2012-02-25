#ifndef ENTRY_HH
#define ENTRY_HH

#include <set>
#include <map>
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

        explicit entry (std::map<index_type, value_type> values);

        value_type operator[] (index_type index) const;
        value_type &operator[] (index_type index);

        void visit (const std::function<void (index_type,
                                              value_type)> &functor) const;

        template <typename Iterator>
        void expand_dimensions (Iterator begin, const Iterator &end);

    private:
        std::map<index_type, value_type>  values;
    };

    class invalid_dimension : public std::exception
    {
    public:
        invalid_dimension (entry::index_type dimension);

        // override from std::exception
        virtual const char *what () const throw ();
        entry::index_type dimension () const {return _dimension;}

    private:
        const entry::index_type _dimension;
    };

    std::ostream &operator<< (std::ostream &out, const entry &e);
}

// Template implementation
template <typename Iterator>
void knn::entry::expand_dimensions (Iterator begin, const Iterator &end)
{
    for (; begin != end; ++begin)
        if (values.find (*begin) == values.end ())
            values.insert ({*begin, 0});
}

#endif  // ENTRY_HH
