#include "knn.hpp"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "data_handler.hpp"

knn::knn(int val) {
    k = val;
}
knn::knn() {
    // NOTHING TO DO
}
knn::~knn() {
    // NOTHING TO DO
}

// O(N^2)

void knn::find_knearest(data* query_point) {
    neighbors = new std::vector<data*>;
    double min = std::numeric_limits<double>::max(); // +∞
    double previous_min = min;
    int index = 0;
    for (int i = 0; i < k; i++) {
        if (i == 0) {
            for (int j = 0; j < training_data->size(); j++) {
                double dist = calculate_distance(query_point, training_data->at(j));
                training_data->at(j)->set_distance(dist);
                if (dist < min) {
                    min = dist;
                    index = j;
                }
            }
            neighbors->push_back(training_data->at(index));
            previous_min = min;
            min = std::numeric_limits<double>::max();
        } else {
            for (int j = 0; j < training_data->size(); j++) {
                double dist = calculate_distance(query_point, training_data->at(j));
                if (dist > previous_min && dist < min) { // 上面找到的是最小的，满足这个条件就是第二小的
                    min = dist;
                    index = j;
                }
            }
            neighbors->push_back(training_data->at(index));
            previous_min = min;
            min = std::numeric_limits<double>::max();
        }
    }

}

void knn::set_k(int val) {
    k = val;
}

int knn::predict() {
    std::map<uint8_t, int> class_freq;
    for (int i = 0; i < neighbors->size(); i++) {
        if (class_freq.find(neighbors->at(i)->get_label()) == class_freq.end()) {
            class_freq[neighbors->at(i)->get_label()] = 1;
        } else {
            class_freq[neighbors->at(i)->get_label()]++;
        }
    }

    int best = 0;
    int max_count = 0;
    for (auto kv : class_freq) {
        if (kv.second > max_count) {
            max_count = kv.second;
            best = kv.first;
        }
    }
    neighbors->clear();
    return best;
}

double knn::calculate_distance(data* query_point, data* input) {
    double dist = 0.0;
    if (query_point->get_feature_vector_size() != input->get_feature_vector_size()) {
        printf("Error Vector Size Mismatch,\n");
        exit(1);
    }
#ifdef EUCLID
    for (unsigned i = 0; i < query_point->get_feature_vector_size(); i++) {
        dist += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
    }
    return sqrt(dist);
#elif defined MANHATTAN
    // PUT MANHATTAN IMPLEMENTION HERE
#endif

    
}
double knn::validate_performance() {
    double current_performance = 0;
    int count = 0;
    int data_index = 0;
    for (data* query_point : *validation_data) {
        find_knearest(query_point);
        int prediction = predict();
        if (prediction == query_point->get_label()) {
            count++;
        }
        data_index++;
        printf("Current Performance for K = %d : %.3f %%\n", k, ((double)count*100.0)/ ((double)data_index));
    }
    current_performance = ((double)count*100.0)/ ((double)validation_data->size());
    printf("Validation Performance = %.3f %%\n", current_performance);
    
    return current_performance;
}
double knn::test_performance() {
    double current_performance = 0;
    int count = 0;

    for (data* query_point : *test_data) {
        find_knearest(query_point);
        int prediction = predict();
        printf("%d -> %d \n", prediction, query_point->get_label());
        if (prediction == query_point->get_label()) {
            count++;
        }
    }
    current_performance = ((double)count*100.0)/ ((double)test_data->size());
    printf("Test Performance = %.3f %%\n", current_performance);
    
    return current_performance;
}

int main() {
    printf("I've entered here");
    data_handler *dh = new data_handler();
    dh->read_feature_vector("../train-images.idx3-ubyte");
    dh->read_feature_labels("../train-labels.idx1-ubyte");
    dh->split_data();
    dh->count_classes();
    knn *knearest = new knn();
    // printf("error 1");
    knearest->set_training_data(dh->get_training_data());
    // printf("error 2");
    knearest->set_validation_data(dh->get_validation_data());
    // printf("error 3");
    knearest->set_test_data(dh->get_test_data());
    printf("error 4");

    double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for (int i = 1; i <= 5; i++) {
        if (i == 1) {
            knearest->set_k(i);
            performance = knearest->validate_performance();
            best_performance = performance;
        } else {
            knearest->set_k(i);
            performance = knearest->validate_performance();
            if (performance > best_performance) {
                best_performance = performance;
                best_k = i;
            }
        }
    }

    knearest->set_k(best_k);
    knearest->test_performance();

    delete knearest;
    delete dh;

    return 0;
}
