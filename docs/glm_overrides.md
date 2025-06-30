# glm_overrides

### glm
Namespace extension providing custom comparison operators for glm::vec3 and std::pair<glm::vec3, glm::vec3>, enabling their use in ordered containers like std::set and std::map.

###### Methods:
`bool operator<(const glm::vec3& lhs, const glm::vec3& rhs)`
Defines a strict weak ordering for glm::vec3 using lexicographic comparison. First compares x, then y, then z.

`bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs)`
Defines a strict weak ordering for std::pair<glm::vec3, glm::vec3> by comparing the first elements of each pair using the vec3 comparator. If equal, compares the second elements.