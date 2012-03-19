#include <cassert>
#include <thread-pool.hh>

using knn::thread_pool;

thread_pool::thread_pool (int max_threads) :
    max_threads (max_threads < 0 ?
                 std::thread::hardware_concurrency () : max_threads),
    cancelled (false)
{
    assert (this->max_threads > 0);
    threads.reserve (max_threads);

    for (int i = 0; i < max_threads; ++i)
        threads.push_back (std::thread (std::bind (&thread_pool::thread_func,
                                                   std::ref (*this))));
}

thread_pool::~thread_pool ()
{
    cancelled = true;

    for (auto &i : threads)
        if (i.joinable ())
            i.join ();
}

void thread_pool::thread_func ()
{
    while (!cancelled) {
        std::function<void ()> task;

        {
            std::unique_lock<std::mutex> lock (mutex);

            while (tasks.empty ())
                condvar.wait (lock);

            task = tasks.front ();
            tasks.pop ();
        }

        task ();
    }
}
