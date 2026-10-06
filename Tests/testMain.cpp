#include <catch2/catch_session.hpp>

#include "../src/Core/Application.h"

// allocation counters...
#include <atomic>
#include <new>
#include <cstdlib>

static std::atomic<size_t> global_allocations{0};
static std::atomic<size_t> global_deallocations{0};

size_t get_active_allocations()
{
    return global_allocations.load() - global_deallocations.load();
}

void reset_allocation_counters()
{
    global_allocations.store(0);
    global_deallocations.store(0);
}

void* operator new(std::size_t size)
{
    global_allocations++;
    void* p = std::malloc(size);

    if (!p)
        throw std::bad_alloc();

    return p;
}

void operator delete(void* p) noexcept
{
    global_deallocations++;
    std::free(p);
}


int main(int argc, char** argv)
{
    std::printf("Creating application...\n");

    Application& app =
        Application::getInstance(100, 100, "SurfaceEditor");

    std::printf("Running tests...\n");

    int result = Catch::Session().run(argc, argv);

    std::printf("Tests finished: %d\n", result);

    std::printf("Closing application...\n");
    app.close();

    std::printf("Application closed.\n");

    return result;
}

