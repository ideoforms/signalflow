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
    KDTreeMatch get_nearest(const std::vector<float> &target,
                            KDTreeMatch current_nearest);

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
    KDTree(std::vector<std::vector<float>> data);
    ~KDTree();
    KDTreeMatch get_nearest(const std::vector<float> &target);

private:
    size_t num_dimensions;
    KDTreeNode *root;
    KDTreeNode *construct_subtree(std::vector<std::vector<float>> data,
                                  size_t dimension_index,
                                  std::vector<std::vector<float>> bounding_box);
};

#endif // SIGNALFLOW_KDTREE_H
