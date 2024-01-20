#ifndef SIGNALFLOW_KDTREE_H
#define SIGNALFLOW_KDTREE_H

#include <limits>
#include <memory>
#include <vector>

class KDTreeNode
{
public:
    KDTreeNode(const std::vector<float> &coordinates,
               KDTreeNode *left,
               KDTreeNode *right,
               const std::vector<std::vector<float>> &bounding_box);
    ~KDTreeNode();
    std::pair<KDTreeNode *, float> get_nearest(const std::vector<float> &target,
                                               KDTreeNode *current_nearest = nullptr,
                                               float current_nearest_distance = std::numeric_limits<float>::infinity());

    std::vector<float> get_coordinates();

private:
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
    std::vector<float> get_nearest(const std::vector<float> &target);

private:
    size_t num_dimensions;
    KDTreeNode *root;
    KDTreeNode *construct_subtree(std::vector<std::vector<float>> data,
                                  size_t dimension_index,
                                  std::vector<std::vector<float>> bounding_box);
};

#endif // SIGNALFLOW_KDTREE_H
