#ifndef SIGNALFLOW_KDTREE_H
#define SIGNALFLOW_KDTREE_H

#include <limits>
#include <memory>
#include <vector>

class KDTreeMatch
{
public:
    KDTreeMatch(int index,
                std::vector<float> coordinate,
                float distance)
        : index(index), coordinate(coordinate), distance(distance) {}
    int index;
    std::vector<float> coordinate;
    float distance;

    int get_index() { return this->index; }
    std::vector<float> get_coordinate() { return this->coordinate; }
    float get_distance() { return this->distance; }

    // Sorting operators
    bool operator<(const KDTreeMatch &other) const { return distance < other.distance; }
    bool operator>(const KDTreeMatch &other) const { return distance > other.distance; }
};

class KDTreeMatches
{
public:
    KDTreeMatches(size_t capacity);
    void add(const KDTreeMatch &match);
    float get_worst_distance() const;
    bool is_full() const;
    const std::vector<KDTreeMatch> &get_matches() const;
    void sort();

private:
    std::vector<KDTreeMatch> matches;
    size_t capacity;
};

class KDTreeNode
{
public:
    KDTreeNode(int index,
               const std::vector<float> &coordinates,
               KDTreeNode *left,
               KDTreeNode *right,
               const std::vector<std::vector<float>> &bounding_box);
    ~KDTreeNode();
    void get_nearest(const std::vector<float> &target,
                     KDTreeMatches &matches);

    int get_index();
    std::vector<float> get_coordinates();

private:
    int index;
    std::vector<float> coordinates;
    KDTreeNode *left;
    KDTreeNode *right;
    std::vector<std::vector<float>> bounding_box;
};

class KDTree
{
public:
    KDTree(std::vector<std::vector<float>> data, size_t num_neighbors = 1);
    ~KDTree();
    std::vector<KDTreeMatch> get_nearest(const std::vector<float> &target);

private:
    size_t num_dimensions;
    size_t num_neighbors;
    KDTreeNode *root;
    KDTreeNode *construct_subtree(std::vector<std::vector<float>> data,
                                  size_t dimension_index,
                                  std::vector<std::vector<float>> bounding_box);
};

#endif // SIGNALFLOW_KDTREE_H
