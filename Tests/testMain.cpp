#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <new>
#include <cstdlib>

// The actual atomic counters live here permanently in one place
static std::atomic<size_t> global_allocations{0};
static std::atomic<size_t> global_deallocations{0};

// Implement the functions exposed in the header
size_t get_active_allocations() {
    return global_allocations.load() - global_deallocations.load();
}

void reset_allocation_counters() {
    global_allocations.store(0);
    global_deallocations.store(0);
}

// Global overrides trap every allocation across the entire test execution
void* operator new(std::size_t size) {
    global_allocations++;
    void* p = std::malloc(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* p) noexcept {
    global_deallocations++;
    std::free(p);
}