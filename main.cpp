
#include <iostream>
#include <cassert>
#include <chrono>

// TODO: Implement and add this file
#include "DynamicAllocator.h"

#define DefaultTests


//#define Benchmark
//#define TestDynamicAllocator
//#define TestNewDelete

int main(int argc, const char* argv[])
{
    using IndexType = uint16_t;
    using Pool = DynamicAllocator<8, IndexType>;

#ifdef DefaultTests
    // A few test types to create in the pool
    struct IAm2byte { int16_t x; };
    struct IAm4byte { int32_t x; };
    struct IAm8byte { int64_t x; };
    struct IAmPolymorphic
    {
        virtual ~IAmPolymorphic() noexcept
        {
            std::cout << __func__ << std::endl;
        }
    };

    // Pool with largest possible size w.r.t. index type
    // Pool pool {(IndexType)-1 - 1};

    // Capacity too large for the given index type,
    // since max value is reserved as a null value
    // Pool pool_ { (IndexType)-1 };


    const int capacity = 4;
    std::cout << "Creating pool with capacity " << capacity << "..." << std::endl;
    Pool pool{ capacity };

    pool.dump_pool();

    std::cout << "Creating 2 objects..." << std::endl;
    auto p1 = pool.create<IAm2byte>((int16_t)1);
    auto p2 = pool.create<IAm4byte>((int32_t)2);

    pool.dump_pool();
    assert(pool.count_free() == capacity - 2);

    std::cout << "Creating 2 objects..." << std::endl;
    auto p3 = pool.create<IAm8byte>((int64_t)3);
    auto p4 = pool.create<IAmPolymorphic>();

    pool.dump_pool();
    assert(pool.count_free() == capacity - 4);

    assert(p1->x == 1);
    assert(p2->x == 2);
    assert(p3->x == 3);

    std::cout << "Destroying 2 objects..." << std::endl;
    pool.destroy(p4);
    pool.destroy(p3);

    pool.dump_pool();
    assert(pool.count_free() == capacity - 2);

    std::cout << "Destroying 2 objects..." << std::endl;
    pool.destroy(p1);
    pool.destroy(p2);

    pool.dump_pool();
    assert(pool.count_free() == capacity);
#endif

#ifdef Benchmark

    const IndexType N = (IndexType)-1 - 1;
    int Nmult = 1000;
    struct A { int a, b, c; };
    A* ptrs[N];

#ifdef TestDynamicAllocator
    {
        // Start timings
        std::cout << "Running DynamicAllocator tests..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        {
            DynamicAllocator<sizeof(A), IndexType> pool{ N };

            for (int n = 0; n < Nmult; n++)
            {
                for (int i = 0; i < N; i++)
                    ptrs[i] = pool.create<A>(0, 1, 2);
                for (int i = 0; i < N; i++)
                    pool.destroy(ptrs[i]);
            }
        }

        // Show timings
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    }
#endif

#ifdef TestNewDelete
    {
        // Start timings
        std::cout << "Running new/delete tests ..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        for (int n = 0; n < Nmult; n++)
        {
            for (int i = 0; i < N; i++)
                ptrs[i] = new A{ 0, 1, 2 };
            for (int i = 0; i < N; i++)
                delete ptrs[i];
        }

        // Show timings
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    }
#endif

#endif

    return 0;
}