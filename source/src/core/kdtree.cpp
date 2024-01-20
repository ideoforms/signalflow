#include "signalflow/core/kdtree.h"
#include <algorithm>
#include <cassert>
#include <math.h>
#include <string>

void print_data(std::vector<std::vector<float>> data)
{
    for (auto coordinate : data)
    {
        printf("[%.2f, %.2f], ", coordinate[0], coordinate[1]);
    }
    printf("\n");
}

float distance_from_point_to_point(const std::vector<float> &pointA,
                                   const std::vector<float> &pointB)
{
    assert(pointA.size() == pointB.size());
    float sum_squares = 0.0;
    for (size_t i = 0; i < pointA.size(); i++)
    {
        sum_squares += powf(pointA[i] - pointB[i], 2);
    }
    return sqrtf(sum_squares);
}

float distance_from_point_to_bounding_box(const std::vector<float> &point,
                                          const std::vector<std::vector<float>> &bounding_box)
{
    assert(point.size() == bounding_box.size());

    std::vector<float> distances_in_each_dimension(point.size(), 0.0f);

    for (size_t dimension = 0; dimension < point.size(); dimension++)
    {
        if (point[dimension] < bounding_box[dimension][0])
        {
            distances_in_each_dimension[dimension] = point[dimension] - bounding_box[dimension][0];
        }
        else if (point[dimension] > bounding_box[dimension][1])
        {
            distances_in_each_dimension[dimension] = point[dimension] - bounding_box[dimension][1];
        }
        else
        {
            distances_in_each_dimension[dimension] = 0.0f;
        }
    }

    float distance = 0.0f;
    for (float dist : distances_in_each_dimension)
    {
        distance += dist * dist;
    }
    distance = sqrtf(distance);

    return distance;
}

KDTreeNode::KDTreeNode(int index,
                       const std::vector<float> &coordinates,
                       KDTreeNode *left,
                       KDTreeNode *right,
                       const std::vector<std::vector<float>> &bounding_box)
    : index(index), coordinates(coordinates), left(left), right(right), bounding_box(bounding_box)
{
}

KDTreeNode::~KDTreeNode()
{
    if (this->left)
    {
        delete this->left;
    }
    if (this->right)
    {
        delete this->right;
    }
}

KDTreeMatch KDTreeNode::get_nearest(const std::vector<float> &target,
                                    KDTreeMatch current_nearest)
{
    float distance = distance_from_point_to_point(target, this->coordinates);

    if (distance < current_nearest.distance)
    {
        current_nearest.coordinate = this->coordinates;
        current_nearest.index = this->index;
        current_nearest.distance = distance;
    }

    /*------------------------------------------------------------------------------
     * Prune the tree by skipping any forks that can't contain any nodes closer
     * than the current closest.
     *
     * TODO: Further optimization can be made by ordering this left/right
     *       pruning based on whether the target would be to the left or right
     *       of this node. Need to implement and benchmark.
     *------------------------------------------------------------------------------*/
    if (this->left && distance_from_point_to_bounding_box(target, left->bounding_box) < current_nearest.distance)
    {
        current_nearest = left->get_nearest(target, current_nearest);
    }

    if (this->right && distance_from_point_to_bounding_box(target, right->bounding_box) < current_nearest.distance)
    {
        current_nearest = right->get_nearest(target, current_nearest);
    }

    return current_nearest;
}

int KDTreeNode::get_index()
{
    return this->index;
}

std::vector<float> KDTreeNode::get_coordinates()
{
    return this->coordinates;
}

KDTree::KDTree(std::vector<std::vector<float>> data)
{
    if (data.empty())
    {
        throw std::runtime_error("KDTree: Data array cannot be empty");
    }
    this->num_dimensions = data[0].size();

    /*------------------------------------------------------------------------------
     * For each datapoint, append an integer label.
     *
     * This is ignored in the tree construction algorithm, but is later popped
     * off to coordinate and used so that each node in the tree can be labelled
     * with its unique index.
     *
     * Needs to be appended to the same structure as the data array is repeatedly
     * re-ordered through the process, and this is more efficient that
     * sorting an array of coordinates and an array of labels.
     *------------------------------------------------------------------------------*/
    for (auto i = 0; i < data.size(); i++)
    {
        data[i].push_back(i);
    }
    std::vector<std::vector<float>> bounding_box(num_dimensions);
    for (auto &bounds : bounding_box)
    {
        bounds = { -std::numeric_limits<float>::max(),
                   std::numeric_limits<float>::max() };
    }
    this->root = this->construct_subtree(data, 0, bounding_box);
}

KDTree::~KDTree()
{
    if (this->root)
    {
        delete this->root;
    }
}

KDTreeNode *KDTree::construct_subtree(std::vector<std::vector<float>> data,
                                      size_t dimension_index,
                                      std::vector<std::vector<float>> bounding_box)
{
    /*--------------------------------------------------------------------------------
     * Sort the data in increasing order according to this node's dimension_index
     *--------------------------------------------------------------------------------*/
    std::sort(data.begin(),
              data.end(),
              [&dimension_index](std::vector<float> a, std::vector<float> b) {
                  return (a[dimension_index] < b[dimension_index]);
              });
    size_t mid_point = data.size() / 2;
    size_t left_length = mid_point;
    size_t right_length = data.size() - left_length - 1;
    /*--------------------------------------------------------------------------------
     * Partition the data into three:
     *  - left partition (may be null)
     *  - coordinate of this node
     *  - right partition (may be null)
     *--------------------------------------------------------------------------------*/
    std::vector<std::vector<float>> dimension_partition_left(left_length);
    std::vector<std::vector<float>> dimension_partition_right(right_length);
    std::copy(data.begin(),
              data.begin() + left_length,
              dimension_partition_left.begin());
    std::copy(data.begin() + left_length + 1,
              data.end(),
              dimension_partition_right.begin());

    std::vector<float> coordinate = data[mid_point];
    int node_index = coordinate[coordinate.size() - 1];
    coordinate.pop_back();

    size_t dimension_index_next = (dimension_index + 1) % this->num_dimensions;

    KDTreeNode *left_tree = nullptr;
    if (dimension_partition_left.size() > 0)
    {
        std::vector<std::vector<float>> bounding_box_left(bounding_box.begin(),
                                                          bounding_box.end());
        bounding_box_left[dimension_index][1] = coordinate[dimension_index];

        left_tree = this->construct_subtree(dimension_partition_left,
                                            dimension_index_next,
                                            bounding_box_left);
    }

    KDTreeNode *right_tree = nullptr;
    if (dimension_partition_right.size() > 0)
    {
        std::vector<std::vector<float>> bounding_box_right(bounding_box.begin(),
                                                           bounding_box.end());
        bounding_box_right[dimension_index][0] = coordinate[dimension_index];

        right_tree = this->construct_subtree(dimension_partition_right,
                                             dimension_index_next,
                                             bounding_box_right);
    }

    KDTreeNode *node = new KDTreeNode(node_index,
                                      coordinate,
                                      left_tree,
                                      right_tree,
                                      bounding_box);

    return node;
}

KDTreeMatch KDTree::get_nearest(const std::vector<float> &target)
{
    if (target.size() != this->num_dimensions)
    {
        throw std::runtime_error("Target has an invalid number of dimensions (expected = " + std::to_string(this->num_dimensions) + ", actual = " + std::to_string(target.size()) + ")");
    }
    auto result = this->root->get_nearest(target, KDTreeMatch(0, {}, std::numeric_limits<float>::max()));
    return result;
}