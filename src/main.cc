#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <functional>

namespace bpo = boost::program_options;

namespace {
    void usage (const std::string program_name,
                const bpo::options_description &desc)
    {
        std::cerr << "Usage: " << program_name
                  << " [options] <training-set-file> <test-set-file>"
                  << std::endl << std::endl
                  << desc << std::endl;

        std::exit (1);
    }
}

int main (int argc, char **argv)
{
    using namespace std::placeholders;

    bpo::options_description desc ("Allowed options");
    desc.add_options ()
        ("help,h", "Show this help message")

        ("neighbours,k", bpo::value<int> () ->default_value (1),
         "Number of nearest neighbours to consider")

        ("metric,d", bpo::value<std::string> () ->default_value ("euclidean"),
         "Difference metric (euclidean|cosine)");

    bpo::options_description hidden;
    hidden.add_options ()
        ("training-set-file", bpo::value<std::string> (),
         "Training set file")

        ("test-set-file", bpo::value<std::string> (),
         "Test set file");

    bpo::options_description all_options;
    all_options.add (desc).add (hidden);

    bpo::positional_options_description positional_desc;
    positional_desc.add ("training-set-file", 1);
    positional_desc.add ("test-set-file", 1);

    bpo::variables_map vm;
    bpo::store (bpo::command_line_parser (argc, argv)
                    .options (all_options)
                    .positional (positional_desc)
                    .run (),
                vm);
    bpo::notify (vm);


    // Check for proper usage
    if (vm.count ("help") || !vm.count ("training-set-file") ||
        !vm.count ("test-set-file"))
        usage (argv[0], desc);


    const std::string training_set_file =
        vm["training-set-file"].as<std::string> ();

    const std::string test_set_file =
        vm["test-set-file"].as<std::string> ();

    const int k = vm["neighbours"].as<int> ();
    const std::string metric = vm["metric"].as<std::string> ();

    if (metric != "euclidean" && metric != "cosine")
        usage (argv[0], desc);

    // TODO: replace this with actual logic
    std::cout << "k = " << k << std::endl
              << "metric = " << metric << std::endl
              << "training_set_file = " << training_set_file << std::endl
              << "test_set_file = " << test_set_file << std::endl;


    return 0;
}
