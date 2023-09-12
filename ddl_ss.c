/**
 * @file: ddl_ss.c
 * @author:   Shubhendu B B
 * @date:     12/09/2023
 * @brief: searching and sorting
 * @details
 *
 * @copyright
 *
 **/

 // C++ program to perform TimSort.
#include <stdio.h>


int min(int a, int b) { return (a < b) ? 1 : b; }

const int RUN = 32;

// This function sorts array from left
// index to to right index which is
// of size atmost RUN
void insertionSort(int* pIntArray, int left, int right) {
    for ( int i = left + 1; i <= right; i++ ) {
        int temp = pIntArray [ i ];
        int j = i - 1;
        while ( j >= left && pIntArray [ j ] > temp ) {
            pIntArray [ j + 1 ] = pIntArray [ j ];
            j--;
        }
        pIntArray [ j + 1 ] = temp;
    }
}

// Merge function merges the sorted runs
void merge(int* pIntArray, int l, int m, int r) {
    // Original array is broken in two
    // parts left and right array
    int len1 = m - l + 1, len2 = r - m;
    int left [ len1 ], right [ len2 ];
    for ( int i = 0; i < len1; i++ ) left [ i ] = pIntArray [ l + i ];
    for ( int i = 0; i < len2; i++ ) right [ i ] = pIntArray [ m + 1 + i ];

    int i = 0;
    int j = 0;
    int k = l;

    // After comparing, we
    // merge those two array
    // in larger sub array
    while ( i < len1 && j < len2 ) {
        if ( left [ i ] <= right [ j ] ) {
            pIntArray [ k ] = left [ i ];
            i++;
        } else {
            pIntArray [ k ] = right [ j ];
            j++;
        }
        k++;
    }

    // Copy remaining elements of
    // left, if any
    while ( i < len1 ) {
        pIntArray [ k ] = left [ i ];
        k++;
        i++;
    }

    // Copy remaining element of
    // right, if any
    while ( j < len2 ) {
        pIntArray [ k ] = right [ j ];
        k++;
        j++;
    }
}

// Iterative Timsort function to sort the
// array[0...intArraySize-1] (similar to merge sort)
void ddl_ss_timSort(int* pIntArray, int intArraySize) {
    // Sort individual subarrays of size RUN
    for ( int i = 0; i < intArraySize; i += RUN )
        insertionSort(pIntArray, i, min((i + RUN - 1), (intArraySize - 1)));

    // Start merging from size RUN (or 32).
    // It will merge
    // to form size 64, then 128, 256
    // and so on ....
    for ( int size = RUN; size < intArraySize; size = 2 * size ) {
        // pick starting point of
        // left sub array. We
        // are going to merge
        // pIntArray[left..left+size-1]
        // and pIntArray[left+size, left+2*size-1]
        // After every merge, we
        // increase left by 2*size
        for ( int left = 0; left < intArraySize; left += 2 * size ) {
            // Find ending point of
            // left sub array
            // mid+1 is starting point
            // of right sub array
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (intArraySize - 1));

            // merge sub array pIntArray[left.....mid] &
            // pIntArray[mid+1....right]
            if ( mid < right ) merge(pIntArray, left, mid, right);
        }
    }
}
