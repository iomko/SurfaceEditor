#pragma once
#include "Structures/ExtendedHalfEdge.h"

enum class FeatureObjectType {
    Face,
    Edge,
    Point
};

template<typename T>
struct FeatureObjectTypeTrait;

template<>
struct FeatureObjectTypeTrait<ExtendedFace> {
    static constexpr FeatureObjectType value = FeatureObjectType::Face;
};

template<>
struct FeatureObjectTypeTrait<ExtendedEdge> {
    static constexpr FeatureObjectType value = FeatureObjectType::Edge;
};

template<>
struct FeatureObjectTypeTrait<ExtendedVertex> {
    static constexpr FeatureObjectType value = FeatureObjectType::Point;
};