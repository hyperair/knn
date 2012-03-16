#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <functional>
#include <chrono>

#include <parser.hh>
#include <classifier.hh>
#include <metrics.hh>
#include <normalizer.hh>

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
         "Difference metric (euclidean|cosine)")

        ("classifier,c", bpo::value<std::string> () ->default_value ("simple"),
         "Classifier implementation (simple|tree)");

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
    const std::string metric_string = vm["metric"].as<std::string> ();
    const std::string classifier_string = vm["classifier"].as<std::string> ();

    if ((metric_string != "euclidean" && metric_string != "cosine") ||
        (classifier_string != "simple" && classifier_string != "tree"))
        usage (argv[0], desc);

    std::cout << "k = " << k << std::endl
              << "metric = " << metric_string << std::endl
              << "training_set_file = " << training_set_file << std::endl
              << "test_set_file = " << test_set_file << std::endl;

    knn::dataset training = knn::parse_file (training_set_file);
    knn::dataset test = knn::parse_file (test_set_file);

    knn::normalizer normalizer;
    normalizer.scan (training);
    normalizer.scan (test);
    normalizer.apply (training);
    normalizer.apply (test);

    knn::metric_type metric =
        metric_string == "euclidean" ? &knn::metrics::euclidean :
                                       &knn::metrics::cosine;

    std::unique_ptr<knn::classifier> classifier
        (classifier_string == "simple" ?
         (knn::classifier *)new knn::simple_classifier (k, metric, training) :
         (knn::classifier *)new knn::tree_classifier (k, metric, training));

    int total = 0;
    int correct = 0;

    auto start_time = std::chrono::high_resolution_clock::now ();
    test.visit ([&] (const knn::entry &e, knn::dataset::class_type clss)
                {
                    ++total;

                    if (clss == classifier->classify (e))
                        ++correct;
                });
    auto end_time = std::chrono::high_resolution_clock::now ();

    std::cout << "Correctly classified " << correct << " out of " << total
              << " entries, with an accuracy of "
              << (double (correct) / total * 100) << '%' << std::endl;
    std::cout << "Time taken: "
              << std::chrono::duration<double> (end_time - start_time).count ()
              << "s." << std::endl;
    return 0;
}
