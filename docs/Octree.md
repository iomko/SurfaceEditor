# Octree

### OctreeNode<T>
Represents a node in an octree structure, which partitions 3D space recursively into eight child nodes.

Stores bounding region, depth, pointers to children and parent, data contained in the node, and status flags.

###### Methods:

`OctreeNode(glm::vec3 minVector, glm::vec3 maxVector, size_t depth, int childIndex, OctreeNode<T>* parentNode = nullptr)`
Constructor that initializes node boundaries, depth, child index, and parent pointer.

`~OctreeNode()`
Destructor (called automatically).

`OctreeNode<T>* accessNeighbourChild()`
Returns a pointer to the neighboring (next) child node of the parent.

`bool operator==(const OctreeNode<T>& other)`
Equality operator comparing nodes based on their boundaries.

`bool operator!=(const OctreeNode<T>& other)`
Inequality operator.

`AABBBoundingRegion getBounds()`
Returns the axis-aligned bounding box of the node.

`void addData(T data, const AABBBoundingRegion& dataBounds, std::map<T, std::vector<OctreeNode<T>*>>& map)`
Adds data to this node, may split the node and updates the data-to-node mapping.

`template <typename QueryShape> void findData(const QueryShape& queryShape, const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg, std::vector<OctreeNode<T>*>& accumulatedOctreeNodes)`
Template method to find data in the node that intersects a query shape, using a custom intersection algorithm.

### OctreeIterator<Octree>
Iterator for traversing an Octree in a depth-first (postorder) manner.

Provides standard iterator operations.

###### Methods:

`OctreeIterator()` - Default constructor.

`OctreeIterator(PointerType node)` - Constructor initializing with a pointer to an octree node.

`OctreeIterator& operator++()` - Increment operator (moves to the next node in postorder traversal).

`ReferenceType operator*()` - Dereference operator returning a reference to the current node.

`PointerType operator->()` - Dereference operator returning pointer to current node.

`bool operator==(const OctreeIterator& other) const` - Equality comparison.

`bool operator!=(const OctreeIterator& other) const` - Inequality comparison.

### Octree<T>
Main octree class managing the root node and providing methods for insertion, search, removal, and iteration.

Maps data elements to the leaf nodes containing them.

###### Methods:

`Octree(glm::vec3 minVector, glm::vec3 maxVector)` - Constructor initializing the root node with given boundaries.

`Octree(Octree<T>&& other) noexcept` - Move constructor.

`OctreeIterator<Octree<T>> end()` - Returns an end iterator.

`OctreeIterator<Octree<T>> begin()` - Returns a begin iterator (starts at the first leaf node in preorder).

`~Octree()` - Destructor.

`bool isEmpty()` - Checks if the octree contains any data.

`void addDataToOctree(T data, const AABBBoundingRegion& dataBounds)` - Adds data to the root node.

`template <typename QueryShape> void findMaxDepthNodes(const QueryShape& queryShape, const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg, std::vector<OctreeNode<T>*>& accumulatedOctreeNodes)`
Finds leaf nodes at maximum depth intersecting the query shape.

`template <typename QueryShape> void findDataInOctree(const QueryShape& queryShape, const std::function<bool(const AABBBoundingRegion&, const QueryShape&)>& octreeBoundsIntersectAlg, const std::function<bool(const T&, const QueryShape&)>& dataIntersectAlg)`
Finds data in the octree that intersects a query shape using given intersection algorithms.

`void removeData(T data)` - Removes data from the octree.

`void cleanAndRetrieveData()` - Declared but empty method for cleaning and retrieving data.
