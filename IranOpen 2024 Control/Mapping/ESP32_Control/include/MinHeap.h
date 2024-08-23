#include <iostream>
#include <cstdint>
#include <math.h>
#include <vector>
#include "Grid.h"

using namespace std;




class MinHeap{
  public:
    // A utility function to _swap two elements
    void _swap(Grid & x, Grid & y){
      Grid temp = x;
      x = y;
      y = temp;
    }
    // int * harr; // pointer to array of elements in heap
    // int capacity; // maximum possible size of min heap
    vector<Grid> hvec;
    Grid max;
    
    int heap_size; // Current number of elements in min heap

    MinHeap(){
      heap_size = 0;
      max.f_cost = INT16_MAX;
      // capacity = cap;
      // harr = new int[cap];
    }

    void clear(){
      heap_size = 0;
      hvec.clear();
      max.f_cost = INT16_MAX;
    }

    int parent(int i){
      return (i - 1) / 2;
    }

    int left(int i){
      return (2 * i + 1);
    }

    int right(int i){
      return (2 * i + 2);
    }

    Grid getMin(){
      return hvec.at(0);
    }

    void insertKey(Grid k){
      // if(heap_size == capacity){
      //   cout << "\nOverflow: Could not insert Key\n";
      //   return;
      // }

    // First insert the new key at the end
      heap_size++;
      int i = heap_size - 1;
      if(heap_size <= hvec.size()){
        hvec.at(i) = k;
      }
      else{
        hvec.push_back(k);
      }


    // Fix the min heap property if it is violated
      while(i != 0 && hvec.at(parent(i)).f_cost > hvec.at(i).f_cost){
        _swap(hvec.at(i), hvec.at(parent(i)));
        i = parent(i);
      }
    }

    void decreaseKey(int i, int new_val){
      hvec.at(i).f_cost = new_val;
      while(i != 0 && hvec.at(parent(i)).f_cost > hvec.at(i).f_cost){
        _swap(hvec.at(i), hvec.at(parent(i)));
        i = parent(i);
      }
    }

    // This function deletes key at index i. It first reduced value to minus
    // infinite, then calls extractMin()
    void deleteKey(int i){
      decreaseKey(i, INT16_MIN);
      extractMin();
    }

    int linearValueSearch(int val){
      for(int i = 0; i < heap_size; i++){
        if(hvec.at(i).f_cost == val){
          cout << "Value Found!";
          return i;
        }
      }
      cout << "Value NOT Found!";
      return INT16_MAX;
    }

    int linearGridSearch(Grid g){
      for(int i = 0; i < heap_size; i++){
        if(hvec.at(i).x == g.x && hvec.at(i).y == g.y){
          cout << "Value Found!";
          return i;
        }
      }
      cout << "Value NOT Found!";
      return INT16_MAX;
    }

    // Method to remove minimum element (or root) from min heap
    Grid extractMin(){
      if(heap_size <= 0)
        return max;
      if(heap_size == 1){
        heap_size--;
        return hvec.at(0);
      }
      // Store the minimum value, and remove it from heap
      Grid root = hvec.at(0);
      hvec.at(0) = hvec.at(heap_size - 1);
      heap_size--;
      MinHeapify(0);

      return root;
    }

    // A recursive method to heapify a subtree with the root at given index
    // This method assumes that the subtrees are already heapified
    void MinHeapify(int i){
      int l = left(i);
      int r = right(i);
      int smallest = i;
      if(l < heap_size && hvec.at(l).f_cost < hvec.at(i).f_cost)
        smallest = l;
      if(r < heap_size && hvec.at(r).f_cost < hvec.at(smallest).f_cost)
        smallest = r;
      if(smallest != i){
        _swap(hvec.at(i), hvec.at(smallest));
        MinHeapify(smallest);
      }
    }

    void printArray(){
      for(int i = 0; i < heap_size; i++)
        cout << hvec.at(i).f_cost << " ";
      cout << endl;
    }
    
    int height(){
      return ceil(log2(heap_size + 1)) - 1;
    }

    //   void getUnsortedArray(){
    //   cout << "Enter " << capacity << " no of elements to sort using HEAPSORT" << endl;
    //   for (int i = 0; i < capacity; i++)
    //     cin >> harr[i];
    // }
    // void heapSort(){
    //   int temp[capacity];
    //   for (int j = 0; j < capacity; j++){
    //     //cout<<extractMin()<<" ";
    //     temp[j] = extractMin();
    //     cout << temp[j] << " ";
    //   }
    // }

};