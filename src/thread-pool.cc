#include <cassert>
#include <thread-pool.hh>
#include <iostream>

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
    condvar.notify_all ();

    for (auto &i : threads)
        i.join ();
}

void thread_pool::thread_func ()
{
    for (;;) {
        std::function<void ()> task;

        {
            std::unique_lock<std::mutex> lock (mutex);

            while (!cancelled && tasks.empty ())
                condvar.wait (lock);

            if (cancelled)
                return;

            task = tasks.front ();
            tasks.pop ();
        }

        task ();
    }
}
