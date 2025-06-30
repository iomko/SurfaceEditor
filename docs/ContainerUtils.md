# ContainerUtils

### utils::containers
Namespace providing utility functions for common std::vector manipulations such as swapping and reversing elements.

###### Methods:
`template <typename T> void swapWithLast(std::vector<T>& vector, size_t index)`
Swaps the element at the specified index with the last element in the vector, if the index is not already the last position. Does nothing if the index is out of bounds or points to the last element.

`template <typename T> void swapLastAndPop(std::vector<T>& vector, size_t index)`
Swaps the element at the specified index with the last element (if not already last), then removes the last element from the vector (pops it). This effectively removes the element at the given index by swapping it to the back and popping.

`template <typename T> void reverseSubrange(std::vector<T>& vec, size_t startingIndex, size_t endingIndex)`
Reverses the elements in the vector between startingIndex and endingIndex inclusive. Does nothing if the indices are invalid (out of range or startingIndex > endingIndex).