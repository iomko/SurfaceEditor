# glm_overrides

Used to extend functionalities of a glm library.  

###### Methods:
`bool operator<(const glm::vec3& lhs, const glm::vec3& rhs)` - Defines a strict weak ordering for glm::vec3 using lexicographic comparison. First compares x, then y, then z.  
`bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs)` - Defines a strict weak ordering for std::pair<glm::vec3, glm::vec3> by comparing the first elements of each pair using the vec3 comparator.  

###### structs:
`struct Vec3ApproxComparator` - Comparator that compares two glm::vec3 points with the given epsilon.  
`struct Vec3PairApproxComparator` - Comparator that compares two std::pair<glm::vec3, glm::vec3> structs. It uses the underlying Vec3ApproxComparator struct.  
