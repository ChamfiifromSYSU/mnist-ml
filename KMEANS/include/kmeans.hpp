#ifndef __KMEANS_HPP
#define __KMEANS_HPP

#include "commom.hpp"
#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <map>
#include "data_handler.hpp"

typedef struct cluster {

    std::vector<double> *centroid; // 记录聚类中心
    std::vector<data *> *cluster_points; // 记录聚类中心所有点
    std::map<int, int> class_counts; // 每个类别的数量
    int most_frequent_class;

    cluster(data *initial_point) {
        centroid = new std::vector<double>;  // 记录聚类中心的所有特征值，just one data
        cluster_points = new std::vector<data *>;
        for (auto value : *(initial_point->get_feature_vector())) {
            centroid->push_back(value);
        }
        cluster_points->push_back(initial_point);
        class_counts[initial_point->get_label()] = 1;
        most_frequent_class = initial_point->get_label();
    }

    void add_to_cluster(data* point) {
        // running average
        int previous_size = cluster_points->size();
        cluster_points->push_back(point);
        for (int i = 0; i < centroid->size(); i++) {
            double value = centroid->at(i);
            value *= previous_size;
            value += point->get_feature_vector()->at(i);
            value /= (double)cluster_points->size();  // 就相当于加了一个数进来，重新算平均
            centroid->at(i) = value;
        }
        if (class_counts.find(point->get_label()) == class_counts.end()) {
            class_counts[point->get_label()] = 1;
        } else {
            class_counts[point->get_label()]++;
        }
        set_most_frequent_class(); // added new data so need to update most_frequent_class
    }

    void set_most_frequent_class() {
        int best_class;
        int freq = 0;
        for (auto kv : class_counts) {
            if (kv.second > freq) {
                freq = kv.second;
            }
        }
        most_frequent_class = best_class;
    }

} cluster_t;

class kmeans : public common_data {
    int num_clusters;
    std::vector<cluster_t *> *clusters;
    std::unordered_set<int> *used_indexes;
  public:
    kmeans(int k);
    void init_clusters();
    void init_clusters_for_each_class();
    void train();
    double euclidean_distance(std::vector<double> *, data *);
    double validate();
    double test();
};


#endif