#include "data_handler.hpp"
#include "data.hpp"

data_handler::data_handler() {
    data_array = new std::vector<data *>;
    test_data = new std::vector<data *>;
    training_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}

data_handler::~data_handler() {
    // free danamically allocated data;
}

void data_handler::read_feature_vector(std::string path) {
    uint32_t header[4];  // train-data has 4 32b int header; |MAGIC|NUM IMAGES|ROWSIZE|COLSIZE|
    unsigned char bytes[4]; // read all 32bits into this char
    FILE *f = fopen(path.c_str(), "r"); // fopen is a C function
    if (f) {
        // read header
        for (int i = 0; i < 4; i++) {
            if (fread(bytes, sizeof(bytes), 1, f)) { // bytes用来接收fstream里的数据，1是读取数据项的个数，然后转为小端存到header里
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting Input file header.\n");
        int image_size = header[2] * header[3];

        // read feature vector
        for (int i = 0; i < header[1]; i++) {
            data *d = new data();  // !! q1: how to manage this memory??
            uint8_t element[1];
            for (int j = 0; j < image_size; j++) {  // 逐个读取像素到feature_vector里
                if (fread(element, sizeof(element), 1, f)) {
                    d->append_to_feature_vector(element[0]);
                } else {
                    printf("Error Reading from file.\n");
                    exit(1);
                }
            }
            data_array->push_back(d);
        }
        printf("Successfully read and stored %lu feature vector.\n", data_array->size());
    } else {
        printf("Could not find file.\n");
        exit(1);
    }
}

void data_handler::read_feature_labels(std::string path) {
    uint32_t header[2];  // label 有2个32bit int header; |MAGIC|NUM IMAGES
    unsigned char bytes[2]; // read all 32bits into this char
    FILE *f = fopen(path.c_str(), "r"); // fopen is a C function
    if (f) {
        // read header
        for (int i = 0; i < 2; i++) {
            if (fread(bytes, sizeof(bytes), 1, f)) { // bytes用来接收fstream里的数据，1是读取数据项的个数，然后转为小端存到header里
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting Label file header.\n");

        for (int i = 0; i < header[1]; i++) {
            uint8_t element[1];
            if (fread(element, sizeof(element), 1, f)) {
                data_array->at(i)->set_label(element[0]);
            } else {
                printf("Error Reading from file.\n");
                exit(1);
            }
        }
        printf("Successfully read and stored labels.\n");
    } else {
        printf("Could not find file.\n");
        exit(1);
    }
}

void data_handler::split_data() {
    // random split dataset
    std::unordered_set<int> used_indexes;
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int valid_size = data_array->size() * VALIDATION_PERCENT;

    // training data
    int count = 0;
    while (count < train_size) {
        int rand_index = rand() % data_array->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) { // 没有
            training_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    // test data
    count = 0;
    while (count < test_size) {
        int rand_index = rand() % data_array->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) { // 没有
            test_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    // validation data
    count = 0;
    while (count < valid_size) {
        int rand_index = rand() % data_array->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) { // 没有
            validation_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    printf("Training Data Size: %lu.\n", training_data->size());
    printf("Test Data Size: %lu.\n", test_data->size());
    printf("Validation Data Size: %lu.\n", validation_data->size());

}

void data_handler::count_classes() {
    int count = 0;
    // 一开始class_map是空的
    for (unsigned i = 0; i < data_array->size(); i++) {
        if (class_map.find(data_array->at(i)->get_label()) == class_map.end()) { // 如果class_map里没有这个label
            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enumerated_label(count);
            count++;
        }
    }
    num_classes = count;
    printf("Successfully Extracted %d Unique Classes.\n", count);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes) {
    return (uint32_t) ((bytes[0] << 24) | 
                        (bytes[1] << 16) |
                        (bytes[2] << 8) |
                        (bytes[3]));
}

std::vector<data*> *data_handler::get_training_data() {
    return training_data;
} // return a pointer of vector
std::vector<data*> *data_handler::get_test_data() {
    return test_data;
}
std::vector<data*> *data_handler::get_validation_data() {
    return test_data;
}


int main() {
    data_handler *dh = new data_handler();
    dh->read_feature_labels("./train-labels.idx3-ubyte");
    dh->read_feature_vector("./train-images.idx3-ubyte");
    dh->split_data();
    dh->count_classes();

}