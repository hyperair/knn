#ifndef THREAD_POOL_HH
#define THREAD_POOL_HH

#include <thread>
#include <future>
#include <memory>
#include <queue>
#include <vector>

namespace knn
{
    class thread_pool
    {
    public:
        thread_pool (int max_threads = -1);
        ~thread_pool ();

        template <typename Func>
        std::future<typename std::result_of<Func ()>::type>
        async (Func func);

    private:
        const int max_threads;

        bool cancelled;

        std::queue<std::function<void ()> > tasks;
        std::vector<std::thread> threads;

        std::mutex mutex;
        std::condition_variable condvar;

        void thread_func ();
    };
}

// Template definition
template <typename Func>
std::future<typename std::result_of<Func ()>::type>
knn::thread_pool::async (Func func)
{
    typedef typename std::result_of<Func ()>::type ret;
    std::shared_ptr<std::promise<ret> > promise (new std::promise<ret>);

    {
        auto task =
            [=] ()
            {
                try {
                    promise->set_value (func ());

                } catch (...) {
                    promise->set_exception (std::current_exception ());
                }
            };

        std::unique_lock<std::mutex> lock (mutex);

        tasks.push (task);

        condvar.notify_one ();
    }

    return promise->get_future ();
}


#endif  // THREAD_POOL_HH
