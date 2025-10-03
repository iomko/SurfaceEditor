# Quadtree

### QuadtreeNode<T>
Represents a node in a quadtree structure, which recursively partitions 2D space into four child nodes.

Stores the bounding region, depth, parent and child pointers, data contained in the node, and status flags.

###### Methods:

`QuadtreeNode(glm::vec2 minVector, glm::vec2 maxVector, size_t depth, int childIndex, QuadtreeNode<T> parentNode = nullptr)`
Constructor that initializes node boundaries, depth, child index, and parent pointer.  

`~QuadtreeNode()`
Destructor (called automatically, cleans up children if implemented).  

`QuadtreeNode<T>* accessNeighbourChild()`
Returns a pointer to the neighboring (next) child node of the parent.  

`bool operator==(const QuadtreeNode<T>& other)`
Equality operator comparing nodes based on their bounding region.

`bool operator!=(const QuadtreeNode<T>& other)`
Inequality operator.

`BoundingRegion2D getBounds()`
Returns the 2D bounding box of the node.

`void addData(T data, const BoundingRegion2D& dataBounds, std::map<T, std::vector<QuadtreeNode<T>*>>& map)`
Adds data to this node. May split the node into children if maximum depth is not reached and updates the data-to-node mapping.

`template <typename QueryShape> void findData(const QueryShape& queryShape, const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg, std::vector<QuadtreeNode<T>*>& accumulatedQuadtreeNodes)`
Finds nodes intersecting a query shape, using a custom intersection algorithm.

### QuadtreeIterator

Iterator for traversing a quadtree in a depth-first (postorder) manner.

Provides standard iterator operations.

###### Methods:

`QuadtreeIterator()`
Default constructor.

`QuadtreeIterator(PointerType node)`
Constructor initializing with a pointer to a quadtree node.

`QuadtreeIterator& operator++()`
Increment operator (moves to the next node in postorder traversal).

`ReferenceType operator()*`
Dereference operator returning a reference to the current node.

`PointerType operator->()`
Dereference operator returning a pointer to the current node.

`bool operator==(const QuadtreeIterator& other) const`
Equality comparison.

`bool operator!=(const QuadtreeIterator& other) const`
Inequality comparison.

### Quadtree

Main quadtree class managing the root node and providing methods for insertion, search, removal, and iteration.

Maps data elements to the leaf nodes containing them.

###### Methods:

`Quadtree(glm::vec2 minVector, glm::vec2 maxVector)`
Constructor initializing the root node with given boundaries.

`Quadtree(Quadtree<T>&& other) noexcept`
Move constructor.

`Iterator end()`
Returns an end iterator.

`Iterator begin()`
Returns a begin iterator (starts at the first leaf node in preorder).

`~Quadtree()`
Destructor.

`bool isEmpty()`
Checks if the quadtree contains any data.

`void addDataToQuadtree(T data, const BoundingRegion2D& dataBounds)`
Adds data to the root node.

`template <typename QueryShape> void findMaxDepthNodes(const QueryShape& queryShape, const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg, std::vector<QuadtreeNode<T>*>& accumulatedQuadtreeNodes)`
Finds leaf nodes at maximum depth intersecting the query shape.

`template <typename QueryShape> void findDataInQuadtree(const QueryShape& queryShape, const std::function<bool(const BoundingRegion2D&, const QueryShape&)>& quadtreeBoundsIntersectAlg, const std::function<bool(const T&, const QueryShape&)>& dataIntersectAlg)`
Finds data in the quadtree that intersects a query shape using given intersection algorithms.

`void removeData(T data)`
Removes data from the quadtree and updates the data-to-node mapping.

`void cleanAndRetrieveData()`
Declared but empty method for cleaning and retrieving data.
