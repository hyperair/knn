#include <limits>
#include <normalizer.hh>

using knn::normalizer;

normalizer::scaling_info::scaling_info (const minmax &m) :
    scale_factor ((normalized_max - normalized_min) / (m.max - m.min)),
    offset (normalized_min - m.min)
{}

normalizer::value_type
normalizer::scaling_info::normalize (dataset::value_type orig) const
{
    return (orig + offset) * scale_factor;
}


normalizer::minmax::minmax () :
    min (std::numeric_limits<decltype (min)>::max ()),
    max (std::numeric_limits<decltype (max)>::min ())
{}

const normalizer::scaling_info &normalizer::minmax::get_scaling_info () const
{
    if (!_scaling_info)
        _scaling_info =
            std::shared_ptr<scaling_info> (new scaling_info (*this));

    return *_scaling_info;
}


void normalizer::scan (const dataset &data)
{
    data.visit ([&] (const entry &e, dataset::class_type)
                {
                    for (const index_type i : data.dimensions ()) {
                        minmax &m = minmax_map[i];

                        m.min = std::min (m.min, e[i]);
                        m.max = std::max (m.max, e[i]);
                    }
                });
}

void normalizer::apply (dataset &data) const
{
    data.visit ([&] (entry &e, dataset::class_type)
                {
                    for (const auto &i : minmax_map)
                        e[i.first] =
                            i.second.get_scaling_info ().normalize (e[i.first]);
                });
}
