#ifndef NORMALIZER_HH
#define NORMALIZER_HH

#include <dataset.hh>

namespace knn
{
    class normalizer
    {
    public:
        typedef dataset::value_type value_type;
        typedef dataset::index_type index_type;

        static constexpr dataset::value_type normalized_min = 0.0;
        static constexpr dataset::value_type normalized_max = 1.0;

        void scan (const dataset &data);
        void apply (dataset &data) const;

    private:
        class minmax;

        class scaling_info
        {
        public:
            scaling_info (const minmax &m);
            dataset::value_type normalize (dataset::value_type orig) const;

        private:
            double scale_factor;
            dataset::value_type offset;
        };

        class minmax
        {
        public:
            minmax ();
            const scaling_info &get_scaling_info () const;

            dataset::value_type min, max;

        private:
            mutable std::shared_ptr<scaling_info> _scaling_info;
        };

        std::unordered_map<index_type, minmax> minmax_map;
    };
}

#endif
