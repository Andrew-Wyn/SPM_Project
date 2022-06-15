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

using namespace std;

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
    ThreadPool() {}

    ThreadPool(int nw) {     // Constructor
      pool.resize(nw);
      num_workers = nw;
      for (int i=0; i<nw; i++) {
        pool[i] = thread(thread_f, std::ref(q));
      }
    }

    void initialize(int nw) {
        pool.resize(nw);
        num_workers = nw;
        for (int i=0; i<nw; i++) {
            pool[i] = thread(thread_f, std::ref(q));
        }
    }

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

class ThParallelFor {
private:
    ThreadPool tp;
    int num_workers;
public:
    ThParallelFor(int nw) { // Constructor
        tp.initialize(nw);
        num_workers = nw;
    }

    template<class Function>
    void parallel_for(const long start, const long end, Function functor) {
        int delta = int((end-start)/num_workers);

        auto fun = [&](const long bucket_start, const long bucket_end){
            for (int i=bucket_start; i<bucket_end; i++) {
                functor(i);
            }
        };

        vector<future<void>> futures(num_workers);
        for (int worker=0; worker<num_workers-1; worker++) {
            
            futures[worker] = tp.push(fun, worker*delta, (worker+1)*delta);
        }
        futures[num_workers-1] = tp.push(fun, (num_workers-1)*delta, end);

        for (int worker=0; worker<num_workers; worker++) {
            futures[worker].get();
        }
    }

    void destroy() {
        tp.terminate();
    }
};

void th_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, vector<double> &res, int nw) {
    int k = 0;

    int n = A.size();

    vector<double> x_start(n);
    vector<double> x_k(n);
    vector<double> x_prev(n);
    x_k = x_start;

    auto A_data = A.data();
    auto b_data = b.data();

    // setting up the parallel for in FastFlow
    ThParallelFor pf(nw);

    while ( true ) {
        x_prev = x_k;

        auto x_prev_data = x_prev.data();
        auto x_k_data = x_k.data();

        // parallel for - map pattern
        pf.parallel_for(0, n, [&](const long i){
            double sigma = 0;

            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }

            x_k[i] = (b_data[i] - sigma)/A_data[i][i];
        });

        auto dist = euclidean_distance(x_k, x_prev);

        cout << dist << endl;

        if (k == 100) {
            cout << "WARNING: out of iterations !!" << endl;
            break;
        }
        if (dist <= eps)
            break;
        k ++;
    }

    pf.destroy();

    res = x_k;
}