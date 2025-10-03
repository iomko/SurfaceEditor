# Ray2D

# Segment2D struct
Segment2D is a struct that represents a 2D line segment defined by two endpoints.  


# Ray2D struct
Ray2D is a struct that represents a 2D ray defined by an origin point and a direction vector. It provides an intersection test with a 2D segment.  

###### Methods:

bool intersectSegment(const Segment2D& seg, glm::vec2& intersectionPoint) – Checks whether the ray intersects with the given segment. Returns true if an intersection exists and stores the intersection point in intersectionPoint.  
