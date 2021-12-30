#include <vector>

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

// copy from https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
struct PairHasher
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};