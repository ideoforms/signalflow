import marimo

__generated_with = "0.1.76"
app = marimo.App()


@app.cell
def __():
    import marimo as mo
    import altair as alt
    import vega_datasets
    import pandas as pd
    from dataclasses import dataclass
    import numpy as np

    cars = vega_datasets.data.cars()
    return alt, cars, dataclass, mo, np, pd, vega_datasets


@app.cell
def __(alt, cars):
    chart = alt.Chart(cars).mark_point(size=3).encode(
        x='Horsepower',
        y='Miles_per_Gallon').properties(width=600, height=400)

    chart
    return chart,


@app.cell
def __(cars, dataclass, np, pd):
    distance_calcs = 0

    @dataclass
    class KDTreeNode:
        datapoint: pd.Series
        left: KDTreeNode
        right: KDTreeNode

        #---------------------------------------------------------------------------
        # bounding_box contains the left/right bounds in each dimension that
        # correspond to this node's bounding box.
        # https://www.cs.princeton.edu/courses/archive/fall20/cos226/precepts/kd-tree-bounding-boxes.pdf
        #---------------------------------------------------------------------------
        bounding_box: np.ndarray

        @property
        def coordinate(self):
            return [value for value in self.datapoint]
            
        def __str__(self):
            return str(self.coordinate)

        def __iter__(self):
            yield self
            if self.left:
                for node in self.left:
                    yield node
            if self.right:
                for node in self.right:
                    yield node
                    
        def get_nearest(self,
                        target: list[float],
                        current_nearest: KDTreeNode = None,
                        current_nearest_distance: float = np.inf):
            global distance_calcs
            distance_calcs += 1
            distance = np.linalg.norm(np.array(target) - np.array(self.coordinate))
            if distance < current_nearest_distance:
                current_nearest = self
                current_nearest_distance = distance
            #------------------------------------------------------------------------------
            # TODO: Further optimisation can be made by ordering this left/right
            #       pruning based on whether the target would be to the left or right
            #       of this node. Need to implement and measure.
            #------------------------------------------------------------------------------
            if self.left and distance_from_point_to_bounding_box(target, self.left.bounding_box) < current_nearest_distance:
                current_nearest, current_nearest_distance = \
                    self.left.get_nearest(target,
                                          current_nearest,
                                          current_nearest_distance)
            if self.right and distance_from_point_to_bounding_box(target, self.right.bounding_box) < current_nearest_distance:
                current_nearest, current_nearest_distance = \
                    self.right.get_nearest(target,
                                           current_nearest,
                                           current_nearest_distance)
            return current_nearest, current_nearest_distance

    def distance_from_point_to_bounding_box(point: np.ndarray,
                                            bounding_box: np.ndarray):
        """
        Given an N-dimensional point, and a bounding box that is defined as a list
        of [min, max] limits for each of N dimensions, return the Euclidean distance
        between the point and the bounding box.
        """
        assert len(point) == bounding_box.shape[0]
        distances_in_each_dimension = np.zeros(len(point))
        for dimension in range(len(point)):
            if point[dimension] < bounding_box[dimension][0]:
                distance_in_dimension = point[dimension] - bounding_box[dimension][0]
            elif point[dimension] > bounding_box[dimension][1]:
                distance_in_dimension = point[dimension] - bounding_box[dimension][1]
            else:
                distance_in_dimension = 0.0
            distances_in_each_dimension[dimension] = distance_in_dimension
        distance = np.linalg.norm(distances_in_each_dimension)
        return distance
            

    class KDTree:
        def __init__(self, dataset: pd.DataFrame, dimensions: list[str]):
            for dimension in dimensions:
                assert dimension in dataset.columns
                #-------------------------------------------------------------------------
                # Remove any rows that have nan values
                #-------------------------------------------------------------------------
                dataset = dataset[np.isfinite(dataset[dimension])]
            self.dataset = dataset[dimensions]
            self.dimensions = dimensions
            self.root = self.construct_subtree(self.dataset)

        @property
        def num_dimensions(self):
            return len(self.dimensions)

        def construct_subtree(self,
                              data: pd.DataFrame,
                              dimension_index: int = 0,
                              bounding_box: np.ndarray = None) -> KDTreeNode:
            if bounding_box is None:
                bounding_box = np.tile([-np.inf, np.inf], [self.num_dimensions, 1])
                
            #------------------------------------------------------------------------------
            # Sort by the current dimension
            #------------------------------------------------------------------------------
            dimension_name = self.dimensions[dimension_index]
            data = data.sort_values(by=dimension_name)
            dimension_data = data[dimension_name]

            #------------------------------------------------------------------------------
            # Partition into left and right, popping the row corresponding to this node
            #------------------------------------------------------------------------------
            dimension_median = np.median(dimension_data)
            dimension_partition_left = data[dimension_data < dimension_median]
            dimension_partition_right = data[dimension_data >= dimension_median]
            data_point = dimension_partition_right.iloc[0]
            dimension_partition_right = dimension_partition_right.iloc[1:]

            #------------------------------------------------------------------------------
            # Recurse to construct the left and right trees
            #------------------------------------------------------------------------------
            dimension_index_next = (dimension_index + 1) % len(self.dimensions)
            left_tree = None
            if len(dimension_partition_left) > 0:
                left_bounding_box = bounding_box.copy()
                left_bounding_box[dimension_index][1] = dimension_median
                left_tree = self.construct_subtree(dimension_partition_left,
                                                   dimension_index_next,
                                                   left_bounding_box)
            right_tree = None
            if len(dimension_partition_right) > 0:
                right_bounding_box = bounding_box.copy()
                right_bounding_box[dimension_index][0] = dimension_median
                right_tree = self.construct_subtree(dimension_partition_right,
                                                    dimension_index_next,
                                                    right_bounding_box)

            #------------------------------------------------------------------------------
            # Return a KDTreeNode encapsulating the datapoint and the left/right trees
            #------------------------------------------------------------------------------
            node = KDTreeNode(data_point, left_tree, right_tree, bounding_box)
            return node

        def get_nearest(self, target: list[float]):
            assert len(target) == len(self.dimensions)
            return self.root.get_nearest(target)

        def __iter__(self):
            for node in self.root:
                yield node

    tree = KDTree(cars, ["Horsepower", "Miles_per_Gallon"])
    nearest = tree.get_nearest([60, 5])
    print("Nearest point: %s" % nearest[0])
    print("Number of calculations made: %d" % distance_calcs)
    return (
        KDTree,
        KDTreeNode,
        distance_calcs,
        distance_from_point_to_bounding_box,
        nearest,
        tree,
    )


@app.cell
def __(tree):
    #--------------------------------------------------------------------------------
    # Render PDF to output.pdf
    # Requires networkx and pygraphviz
    #--------------------------------------------------------------------------------
    import networkx as nx

    G = nx.DiGraph()
    for node in tree:
        G.add_node(str(node))
    for node in tree:
        if node.left:
            G.add_edge(str(node), str(node.left))
        if node.right:
            G.add_edge(str(node), str(node.right))
    ag = nx.nx_agraph.to_agraph(G)
    ag.layout(prog='dot')
    ag.draw(format='pdf', path='output.pdf')
    return G, ag, node, nx


if __name__ == "__main__":
    app.run()
