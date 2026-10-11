#include "../src/Utils/ContainerUtils.h"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("swapWithLast swaps a middle element with the last element", "[containerutils]") {
    std::vector<int> data = {1, 2, 3, 4};
    utils::containers::swapWithLast(data, 1);

    REQUIRE(data.size() == 4);
    REQUIRE(data[0] == 1);
    REQUIRE(data[1] == 4);
    REQUIRE(data[3] == 2);
}

TEST_CASE("swapWithLast leaves the last element unchanged", "[containerutils]") {
    std::vector<int> data = {1, 2, 3, 4};
    utils::containers::swapWithLast(data, 3);

    REQUIRE(data == std::vector<int>({1, 2, 3, 4}));
}

TEST_CASE("swapLastAndPop removes the selected element by swapping with last", "[containerutils]") {
    std::vector<int> data = {1, 2, 3, 4};
    utils::containers::swapLastAndPop(data, 1);

    REQUIRE(data.size() == 3);
    REQUIRE(data[0] == 1);
    REQUIRE(data[1] == 4);
    REQUIRE(data[2] == 3);
}

TEST_CASE("swapLastAndPop on last index just pops back", "[containerutils]") {
    std::vector<int> data = {1, 2, 3, 4};
    utils::containers::swapLastAndPop(data, 3);

    REQUIRE(data.size() == 3);
    REQUIRE(data == std::vector<int>({1, 2, 3}));
}

TEST_CASE("reverseSubrange reverses a valid range and ignores invalid ranges", "[containerutils]") {
    std::vector<int> data = {0, 1, 2, 3, 4};
    utils::containers::reverseSubrange(data, 1, 3);
    REQUIRE(data == std::vector<int>({0, 3, 2, 1, 4}));

    std::vector<int> invalid = {0, 1, 2};
    utils::containers::reverseSubrange(invalid, 2, 1);
    REQUIRE(invalid == std::vector<int>({0, 1, 2}));
}
