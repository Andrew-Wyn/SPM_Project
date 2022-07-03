#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <cstdarg>
#include <memory>
#include <functional>
#include <cstdio>

#include "th_jacobi.hpp"
#include "utils.hpp"

using namespace std;

// Shared queue class
template <typename QTYPE>
class sharedQueue {

private:
  std::queue<QTYPE> q;
  std::mutex m;
  std::condition_variable c;

public:
  QTYPE pop()  {
    std::unique_lock<std::mutex> l(m);
    while(q.empty())
      c.wait(l);
    QTYPE ret = q.front();
    q.pop();
    return ret;
  }

  void push(QTYPE v) { // unique lock is a RAII pattern style lock
    std::unique_lock<std::mutex> l(m);
    q.push(v);
    c.notify_one();
  }

};

// function executed by the thread in the ThreadPool class,
// each thread wait the operation to compute
void thread_f(sharedQueue<function<int()>> &q) {
    while (true) {
        auto el = q.pop();
        int sig = el();

        if (sig == 0) {
            break;
        }
    }
}

class ThreadPool {

private:
    vector<thread> pool;
    sharedQueue<function<int()>> q;
    int num_workers;
public:
    // Constructor
    ThreadPool() {}

    // Constructor
    ThreadPool(int nw) {
      pool.resize(nw);
      num_workers = nw;
      for (int i=0; i<nw; i++) {
        pool[i] = thread(thread_f, std::ref(q));
      }
    }

    // initialization of the pool
    // useful when the number of worker is not known at the creation of the ThreadPool object
    void initialize(int nw) {
        pool.resize(nw);
        num_workers = nw;
        for (int i=0; i<nw; i++) {
            pool[i] = thread(thread_f, std::ref(q));
        }
    }

    // push the function to be computed into the shared queue
    // and return the future to the caller
    template< class Function, class... Args >
    auto push(Function f, Args&&... args) {
        function<decltype(f(args...))()> func = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);

        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<int()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
            return 1;
        };

        q.push(wrapper_func);

        return task_ptr -> get_future();
    }

    // terminate the thread pool
    // sending a termination message to each worker
    void terminate() {

        for (int i=0; i<num_workers; i++) {
            std::function<int()> end_func = []() {
                return 0;
            };
            q.push(end_func);
        }

        for (int i=0; i<num_workers; i++) {
            pool[i].join();
        }

    }
};

// Parallel For implementation
class ThParallelFor {
private:
    ThreadPool tp;
    int num_workers;
public:
    ThParallelFor(int nw) { // Constructor
        tp.initialize(nw);
        num_workers = nw;
    }

    // method to execute the parallel for, taking start and end position
    // and the function to map over the array
    template<class Function>
    void parallel_for(const long start, const long end, Function functor) {
        int delta = int((end-start)/num_workers);

        // define the operations for each bucket
        auto fun = [&](const long bucket_start, const long bucket_end){
            for (int i=bucket_start; i<bucket_end; i++) {
                functor(i);
            }
        };

        // run the operations for each bucket
        future<void> futures[num_workers];
        for (int worker=0; worker<num_workers-1; worker++) {
            
            futures[worker] = tp.push(fun, start + worker*delta, start + (worker+1)*delta);
        }
        futures[num_workers-1] = tp.push(fun, start + (num_workers-1)*delta, end);
        
        // wait the results
        for (int worker=0; worker<num_workers; worker++) {
            futures[worker].get();
        }
    }

    // destroy the underlying thread pool
    void destroy() {
        tp.terminate();
    }
};

void th_jacobi_method(vector<vector<float>> &A, vector<float> &b, float eps, int max_iterations, bool distance_stop, bool debug, vector<float> &res, int nw) {
    int k = 0;

    int n = A.size();

    // initialization variables
    vector<float> x_k(n);
    vector<float> x_prev(n);

    // getting underling data, return array of float, faster to deal with
    auto A_data = A.data();
    auto b_data = b.data();
    auto x_prev_data = x_prev.data();
    auto x_k_data = x_k.data();

    // setting up the parallel for in FastFlow
    ThParallelFor pf(nw);

    while ( true ) {
        // exit it the max iterations are reached
        if (k == max_iterations) {
            if (debug)
                cout << "WARNING: out of iterations !!" << endl;
            
            break;
        }

        // Jacobi Iteration
        pf.parallel_for(0, n, [&](const long i){
            float sigma = 0;

            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }

            x_k_data[i] = (b_data[i] - sigma)/A_data[i][i];
        });

        if (distance_stop) {
            // compute the euclidean distance between x_k and x_prev
            auto dist = euclidean_distance(x_k, x_prev);

            if (debug)
                cout << "Distance : " + to_string(dist) << endl;
            
            if (dist <= eps)
                break;
        }

        k++;

        // switch the pointers to data's vector to change the arrays pourpose
        auto tmp_data = x_prev_data;
        x_prev_data = x_k_data;
        x_k_data = tmp_data;
    }

    // terminate the parallel_for's thread pool
    pf.destroy();

    // copying out the result of the method
    res = x_k;
}