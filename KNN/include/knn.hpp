#ifndef __KNN_H
#define __KNN_H

#include "commom.hpp"

class knn : public common_data {
    int k;
    std::vector<data*>* neighbors;

  public:
    knn(int); // n neighbors
    knn();
    ~knn();

    void find_knearest(data* query_point);
    void set_k(int val); // change k and retrain the model

    int predict(); // return the predicted class
    double calculate_distance(data* query_point, data* input);
    double validate_performance();
    double test_performance();

};

#endif