#include <vector>
#include <unordered_set>

template <size_t Dim>
class CartesianGrid {

public:
  // constructor
  CartesianGrid(std::vector<std::vector<int>> active_in);
  // return the current number of active cubes
  int get_active_cubes() const;
  // return the current simulation iteration
  int get_iteration() const;
  // advance one iteration
  void advance_once();
  
private:
  // the unordered_set does not have a native vector hasher, so we use this one which
  // i found on stack overflow
  struct VectorHasher {
    int operator()(const std::vector<int> &V) const {
      int hash = V.size();
      for(auto &i : V) {
        hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
      }
      return hash;
    }
  };

  // to indicate the cube dimension
  int dim;
  // simulation iteration counter
  int it_count;
  // for storing active cubes, based on their 3D indices
  std::unordered_set<std::vector<int>, VectorHasher> active_cubes;
  // for storeing which indices need to be considered next, to advance one iteration
  std::unordered_set<std::vector<int>, VectorHasher> to_consider;
  // a vector of relative indices, representing the box around a given point
  std::vector<std::vector<int>> neighbours;

  // to count the totoal number of active neighbours, given the curr_active list and a single coordinate
  int active_neighbours(const std::unordered_set<std::vector<int>, VectorHasher>& curr_active, const std::vector<int>& coord) const;

  // recursively gnereate all neighbours of a coordinate, in an n-dimensional space
  void generate_neighbours(std::vector<std::vector<int>>& result, std::vector<int>& curr, int i) const;
  
  
};