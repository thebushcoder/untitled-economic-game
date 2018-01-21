/*Name : Jacob O'Brien
*StudentId : s3514336
*/
/*
 * binaryHeap.hpp
 *
 *  Created on: 21 Apr 2016
 *      Author: linbob
 */

#ifndef UTIL_BINARYHEAP_HPP_
#define UTIL_BINARYHEAP_HPP_

#include <cstdlib>
#include <iostream>
#include <vector>

template <class T>

class BinaryHeap{
public:
	BinaryHeap(){}
	//INSERT NEW NODE AND SORT HEAP
	void insert(T node){
		heap.push_back(node);

		int i = heap.size();
		filterUp(--i);
	}
	void filterUp(int index){
		int node = index;
		int parent = getParentIndex(node);

		T tmp = heap[node];

		while(node > START){
			if(heap[parent] > tmp){
				heap[node] = heap[parent];
				node = parent;
				parent = getParentIndex(node);
			}else{
				break;
			}
		}
		heap[node] = tmp;
	}
	void filterDown(int index){
		int parent = index;
		int child = getLeftChildIndex(parent);
		T tmp = heap[parent];

		int size = heap.size();
		size--;
		while (child < heap.size()){
			if(child < size){
				if(heap[child] > heap[getRightChildIndex(parent)]){  // Set Child to largest Child node
					child = getRightChildIndex(parent);
				}
			}
			if(tmp > heap[child]){
				heap[parent] = heap[child];
				parent = child;
				child = getLeftChildIndex(parent);

			}else{
			  break;
			}
		}
		heap[parent] = tmp;
	}
	//REMOVE AND RETURN FIRST ITEM(SMALLEST) THEN RESORT HEAP
	T deleteMin(){
		if(heap.empty()){
			std::cout << "Heap is empty" << std::endl;
			return T();
		}

		T tmp = heap[START];
		int i = heap.size();
		heap[START] = heap[--i];
		heap.pop_back();
		filterDown(START);

		return tmp;
	}
	//RETURN A VECTOR SORTED IN ASCENDING ORDER
	std::vector<T> heapSort(){
		std::vector<T> sortedList;

		int size = heap.size();
		for(int k = 0; k < size; k++){
			sortedList.push_back(deleteMin());
		}

		return sortedList;
	}
	bool empty(){
		return heap.empty();
	}
	int getSize(){
		return heap.size();
	}
	static int getLeftChildIndex(int index){
		return (INDEX_MOD * index) + LEFT_OFFSET;
	}
	static int getRightChildIndex(int index){
		return (INDEX_MOD * index) + RIGHT_OFFSET;
	}
	static int getParentIndex(int index){
		if(index > START){
			return (--index) / INDEX_MOD;
		}else{
			return START;
		}
	}
	std::vector<T> getHeap(){
		return heap;
	}
private:
	static const int START = 0;
	static const int LEFT_OFFSET = 1;
	static const int RIGHT_OFFSET = 2;
	static const int INDEX_MOD = 2;
	std::vector<T> heap;
};

#endif /* UTIL_BINARYHEAP_HPP_ */
