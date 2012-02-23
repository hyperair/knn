#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <functional>

namespace bpo = boost::program_options;

namespace {
    template <typename T>
    void store (const T source, T& dest)
    {
        dest = source;
    }
}

int main (int argc, char **argv)
{
    using namespace std::placeholders;

    int k;
    std::string metric;

    auto ksetter = std::bind (store<int>, _1, std::ref (k));
    auto metricsetter = std::bind (store<std::string>, _1, std::ref (metric));

    bpo::options_description desc ("Allowed options");
    desc.add_options ()
        ("help,h", "Show this help message")

        ("neighbours,k",
         bpo::value<int> ()
            ->notifier (ksetter)
            ->default_value (1),
         "Number of nearest neighbours to consider")

        ("metric,d",
         bpo::value<std::string> ()
            ->notifier (metricsetter)
            ->default_value ("euclidean"),
         "Difference metric (euclidean|cosine)");

    bpo::variables_map vm;
    bpo::store (bpo::parse_command_line (argc, argv, desc), vm);
    bpo::notify (vm);

    if (vm.count ("help")) {
        std::cerr << desc << std::endl;
        return 1;
    }

    std::cout << "k = " << k << std::endl
              << "metric = " << metric << std::endl;


    return 0;
}
