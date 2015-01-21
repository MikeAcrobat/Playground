//
//  main.cpp
//  3d technics
//
//  Created by Mike on 12/6/14.
//  Copyright (c) 2014 Mike. All rights reserved.
//

#include "Application.h"

//std::vector<unsigned int> generate_random_vars() {
//    std::vector<unsigned int> res;
//    
//    int count = rand() % 3000 + 1000;
//    res.reserve(count);
//    for (int i = 0; i < count; i++) {
//        res.push_back(rand() % 5000);
//    }
//    
//    return res;
//}

int main(int argc, const char * argv[]) {
//    static auto vector = generate_random_vars();
//    int size = (int)vector.size();
//    
//    auto assert_sorted = [=] {
//        for (int i = 0; i < size - 2; i++) {
//            assert(vector[i] <= vector[i + 1]);
//        }
//    };
    
    // insertion
//    for (int i = 2; i < size; i++) {
//        for (int k = i; k > 0 && vector[k] < vector[k - 1]; k--) {
//            auto a = vector[k];
//            vector[k] = vector[k - 1];
//            vector[k - 1] = a;
//        }
//    }
    
    // selection
//    for (int i = 0; i < size; i++) {
//        int min_value = vector[i];
//        int min_index = i;
//        for (int j = i; j < size; j++) {
//            if (vector[j] < min_value) {
//                min_value = vector[j];
//                min_index = j;
//            }
//        }
//        
//        vector[min_index] = vector[i];
//        vector[i] = min_value;
//    }
    
    // bubble
//    for (int i = 0; i < size; i++) {
//        bool swapped = false;
//        for (int j = int(size) - 1; j > i; j--) {
//            if (vector[j] < vector[j - 1]) {
//                auto a          = vector[j];
//                vector[j]       = vector[j - 1];
//                vector[j - 1]   = a;
//                swapped = true;
//            }
//        }
//        if (swapped) break;
//    }

    // quick
//    {
//        auto swap = [=] (int i, int j) {
//            auto tmp    = vector[j];
//            vector[j]   = vector[i];
//            vector[i]   = tmp;
//        };
//        
//        std::function<void (int, int)> sort_func = nullptr;
//        
//        std::function<void (int, int)> sort = [&] (int lo, int hi) {
//            if (lo >= hi) return;
//            
//            std::cout << "sorting start = " << lo << " end = " << hi << std::endl;
//            
//            int pivot_index = lo;
//            int pivot_value = vector[pivot_index];
//            
//            pivot_index = lo;
//            
//            for (int i = lo + 1; i <= hi; i++) {
//                if (vector[i] < pivot_value) {
//                    pivot_index ++;
//                    swap(i, pivot_index);
//                }
//            }
//            
//            swap(lo, pivot_index);
//
//            sort_func(lo, pivot_index - 1);
//            sort_func(pivot_index + 1, hi);
//        };
//        
//        sort_func = sort;
//        
//        sort_func(0, size - 1);
//    }
//
//    assert_sorted();

//    return 0;
    
    Application app;
    return app.run();
}
