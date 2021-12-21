#include <vector>
#include <unordered_set>

template <size_t Dim>
class ConWayCubes {

public:
  // constructor
  ConWayCubes(std::vector<std::vector<int>> active_in);
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

template <size_t Dim>
ConWayCubes<Dim>::ConWayCubes(std::vector<std::vector<int>> active_in) : dim(Dim), it_count(0) {
  // todo: should put a check here that input dimension matches dimension of the ConWayCube
  
  for (int i = 0; i < active_in.size(); i++) {
    active_cubes.insert(active_in[i]);
  }
  
  // generate neighbours
  std::vector<int> curr_coord(dim, 0);
  generate_neighbours(neighbours, curr_coord, 0);
  // we remove the index which represents the center, because of the way neighbours is constructed we know its in the exact middle
  // alternatively we could iterate over all neighbours, and find the one with all coordinates set to 0
  neighbours.erase(neighbours.begin() + neighbours.size()/2);
}

template <size_t Dim>
int ConWayCubes<Dim>::get_active_cubes() const {
  return active_cubes.size();
}

template <size_t Dim>
int ConWayCubes<Dim>::get_iteration() const {
  return it_count;
}

template <size_t Dim>
void ConWayCubes<Dim>::advance_once() {
  // for every active cube, we must also consider its neighbours
  // we may get duplicates, so we use a set to only store unique values
  for (auto it = active_cubes.begin(); it != active_cubes.end(); it++) {
    std::vector<int> coord = *it;
    std::vector<int> copy = coord;
    for (int i = 0; i < neighbours.size(); i++) {
      for (int j = 0; j < coord.size(); j++) {
        copy[j] = coord[j] + neighbours[i][j];
      }
      to_consider.insert(copy);
    }
    // insert original coord, since neighbours only contains neighbouring coords
    to_consider.insert(coord);
  }

  // make copy of current active_cubes in preparation to advance one iteration
  std::unordered_set<std::vector<int>, VectorHasher> prev_active = active_cubes;
  active_cubes.clear();

  for (auto it = to_consider.begin();  it != to_consider.end(); it++) {
    int count = active_neighbours(prev_active, *it);
    if (prev_active.find(*it) != prev_active.end()) {
      // this coordinate is active in the next iteration
      if (count == 2 || count == 3) {
        active_cubes.insert(*it);
      }
    } else {
      if (count == 3) {
        // this coordinate is active in the next iteration
        active_cubes.insert(*it);
      }
    }
  }
  
  to_consider.clear();
  // update iteration counter
  it_count++;
}

template <size_t Dim>
int ConWayCubes<Dim>::active_neighbours(const std::unordered_set<std::vector<int>, VectorHasher>& curr_active, const std::vector<int>& coord) const {
  std::vector<int> copy = coord;
  int result = 0;
  for (int i = 0; i < neighbours.size(); i++) {
    for (int j = 0; j < coord.size(); j++) {
      copy[j] = coord[j] + neighbours[i][j];
    }
    if (curr_active.find(copy) != curr_active.end()) {
      result++;
    }
  }
  return result;
}


template <size_t Dim>
void ConWayCubes<Dim>::generate_neighbours(std::vector<std::vector<int>>& result, std::vector<int>& curr, int i) const {
  if (i == dim) {
    result.push_back(curr);
    return;
  }
  for (int j = -1; j < 2; j++) {
    curr[i] = j;
    generate_neighbours(result, curr, i+1);
  }
  return;
}
  
