#include <iostream>
using namespace std;

void countingSort(int arr[], string names[], int size) {
    int maxVal = *max_element(arr, arr + size);
    int* count = new int[maxVal + 1]();
    int* output = new int[size];
    string* outputNames = new string[size];

    for (int i = 0; i < size; i++)
        count[arr[i]]++;

    for (int i = maxVal - 1; i >= 0; i--)
        count[i] += count[i + 1]; // Descending order

    for (int i = size - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        outputNames[count[arr[i]] - 1] = names[i];
        count[arr[i]]--;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = output[i];
        names[i] = outputNames[i];
    }

    delete[] count;
    delete[] output;
    delete[] outputNames;
}

void merge(int arr[], string names[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];
    string* Lnames = new string[n1];
    string* Rnames = new string[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        Lnames[i] = names[left + i];
    }
    for (int i = 0; i < n2; i++) {
        R[i] = arr[mid + 1 + i];
        Rnames[i] = names[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) { // Descending order
            arr[k] = L[i];
            names[k++] = Lnames[i++];
        } else {
            arr[k] = R[j];
            names[k++] = Rnames[j++];
        }
    }

    while (i < n1) {
        arr[k] = L[i];
        names[k++] = Lnames[i++];
    }
    while (j < n2) {
        arr[k] = R[j];
        names[k++] = Rnames[j++];
    }

    delete[] L;
    delete[] R;
    delete[] Lnames;
    delete[] Rnames;
}

void mergeSort(int arr[], string names[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, names, left, mid);
        mergeSort(arr, names, mid + 1, right);
        merge(arr, names, left, mid, right);
    }
}

