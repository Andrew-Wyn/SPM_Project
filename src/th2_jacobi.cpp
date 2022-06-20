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

// Barrier Class
class MyBarrier {
public:
    explicit MyBarrier(std::size_t iCount) :
      mThreshold(iCount),
      mCount(iCount),
      mGeneration(0) {
    }

    void wait() {
        std::unique_lock<std::mutex> lLock{mMutex};
        auto lGen = mGeneration;
        if (!--mCount) {
            mGeneration++;
            mCount = mThreshold;
            mCond.notify_all();
        } else {
            mCond.wait(lLock, [this, lGen] { return lGen != mGeneration; });
        }
    }

private:
    std::mutex mMutex;
    std::condition_variable mCond;
    std::size_t mThreshold;
    std::size_t mCount;
    std::size_t mGeneration;
};


void th2_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res, int nw) {
    int n = A.size();

    // initialization variables
    vector<double> x_k(n, 0);
    vector<double> x_prev(n, 0);

    // getting underling data, return array of double, faster to deal with
    auto A_data = A.data();
    auto b_data = b.data();

    MyBarrier computed_iteration_bar(nw);
    MyBarrier ready_to_restart_bar(nw);

    double dist;

    auto thread_function = [&](const int thread_id, const long start, const long end) {
        int k = 0;

        // locally data pointers, in this case
        // is not necessary to synchronize the switching of the buffers between the threads
        auto x_prev_data = x_prev.data();
        auto x_k_data = x_k.data();

        while ( true ) {
            // Jacobi Iteration
            for (int i=start; i<end; i++) {
                double sigma = 0;

                for (int j=0; j<n; j++) {
                    if  ( j!=i )
                        sigma = sigma + A_data[i][j]*x_prev_data[j];
                }

                x_k_data[i] = (b_data[i] - sigma)/A_data[i][i];
            }

            // barrier to complete the computation
            computed_iteration_bar.wait();

            
            if (distance_stop) {
                // computation of the distance is delegated to a single thread
                if (thread_id == 0){
                    dist = euclidean_distance(x_k, x_prev);
                    
                    if (debug)
                        cout << "Distance : " + to_string(dist) << endl;
                }
                
                // barrier to wait the computation of the distance
                ready_to_restart_bar.wait();

                if (dist <= eps) {
                    break;
                }
            }
            
            // exit it the max iterations are reached
            if (k == max_iterations) {
                if (thread_id == 0 && debug)
                    cout << "WARNING: out of iterations !!" << endl;

                break;
            }
            
            k++;

            // switch the pointers to data's vector to change the arrays pourpose
            auto tmp_data = x_prev_data;
            x_prev_data = x_k_data;
            x_k_data = tmp_data;
        }
    };

    int delta = int(n/nw);

    // create and run workers
    thread workers[nw];

    for (int w=0; w<nw-1; w++) {
        workers[w] = thread(thread_function, w, w*delta, (w+1)*delta);
    }

    workers[nw-1] = thread(thread_function, nw-1, (nw-1)*delta, n);

    // wait workers
    for (int w=0; w<nw; w++) {
        workers[w].join();
    }

    // copying out the result of the method
    res = x_k;
}