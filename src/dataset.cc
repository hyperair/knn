#include <map>
#include <limits>
#include <dataset.hh>

using knn::dataset;

dataset::dataset () :
    _dimensions (new std::set<index_type>)
{}

dataset::dataset (const dataset &other) :
    entries (other.entries),
    _dimensions (new std::set<index_type> (*other._dimensions))
{}

dataset &dataset::operator= (dataset copy)
{
    entries = std::move (copy.entries);
    _dimensions = std::move (copy._dimensions);

    return *this;
}

void dataset::insert (entry e, class_type clss)
{
    e.visit ([&] (index_type idx, value_type) {_dimensions->insert (idx);});
    entries.push_back (std::make_pair (std::move (e), clss));
}

void dataset::visit (const std::function<void (const entry &,
                                               class_type)> &functor) const
{
    for (const std::pair<entry, class_type> &i : entries)
        functor (i.first, i.second);
}

void dataset::visit (const std::function<void (entry &, class_type)> &functor)
{
    for (std::pair<entry, class_type> &i : entries)
        functor (i.first, i.second);
}

namespace {
    struct minmax
    {
        dataset::value_type min;
        dataset::value_type max;

        minmax ();
    };

    const dataset::value_type normalized_min = 0.0;
    const dataset::value_type normalized_max = 1.0;

    struct scaling_info
    {
        double scale_factor;
        dataset::value_type offset;

        scaling_info (const minmax &m);
        dataset::value_type normalize (dataset::value_type orig) const;
    };
}

void dataset::normalize ()
{
    std::map<index_type, minmax> minmax_map;

    // First pass: accumulate min and max
    visit ([&] (const entry &entry, class_type)
           {
               for (const index_type i : *_dimensions) {
                   minmax &m = minmax_map[i];

                   m.min = std::min (m.min, entry[i]);
                   m.max = std::max (m.max, entry[i]);
               }
           });

    // Calculate normalization parameters
    std::map<index_type, scaling_info> scale_map;
    for (const auto &i : minmax_map)
        scale_map.insert ({i.first, scaling_info (i.second)});

    // Second pass: normalize all values
    visit ([&] (entry &entry, class_type)
           {
               for (const auto &i : scale_map)
                   entry[i.first] = i.second.normalize (entry[i.first]);
           });
}


minmax::minmax ():
    min (std::numeric_limits<decltype (min)>::max ()),
    max (std::numeric_limits<decltype (max)>::min ())
{}


scaling_info::scaling_info (const minmax &m) :
    scale_factor ((normalized_max - normalized_min) / (m.max - m.min)),
    offset (normalized_min - m.min)
{
}

dataset::value_type
scaling_info::normalize (dataset::value_type orig) const
{
    return (orig + offset) * scale_factor;
}
