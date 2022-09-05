// SC2001 Project 1.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int CMP_CNT;

typedef void (SORT_FN_PTR)(int, int, int, int*);

typedef struct timing_info {
	double cpu_time;
	int cmp_cnt;
} TIMING_INFO;

int cmp_lt(int a, int b) {
	CMP_CNT++;
	return a < b;
}

int cmp_gt(int a, int b) {
	CMP_CNT++;
	return a > b;
}

int cmp_lte(int a, int b) {
	CMP_CNT++;
	return a <= b;
}

int cmp_gte(int a, int b) {
	CMP_CNT++;
	return a >= b;
}

void InsertionSort(int start_idx, int end_idx, int unused, int* arr);

int test_alg_on_rand(int count, SORT_FN_PTR fn_ptr, int size_for_mergeinsert, TIMING_INFO* timing_info);

int* gen_input_data(int count);

void MergeInsert(int first, int last, int S, int* arr_start);

void MergeSort(int first, int last, int unused, int* arr_start);

int test_is_sorted(int* array, int count);

int main()
{
	srand(time(NULL));   // Initialization, should only be called once

	TIMING_INFO timing_info;
	int success = test_alg_on_rand(1000, &MergeInsert, 10, &timing_info);

	if (!success)
	{
		printf("Test failed.\n");
		exit(1);
	}
	else
	{
		printf("algorithm,thresh_if_hybrid,arr_size,time,comparisons\n");
		printf("hybrid,10,1000,%f,%d\n", timing_info.cpu_time, timing_info.cmp_cnt);
		exit(0);
	}
}

void swap(int* arr, int a, int b)
{
	int temp = arr[b];
	arr[b] = arr[a];
	arr[a] = temp;
}

void merge(int start, int end, int* arr_start) {
	int mid = (start + end) / 2;

	int first_bucket = start;
	int second_bucket = mid + 1;
	
	int i;
	int tmp;
	int fst_bucket_gt_snd_bucket;

	if (end - start <= 0) {
		return;
	}

	while (first_bucket <= mid && second_bucket <= end) {
		fst_bucket_gt_snd_bucket = cmp_gt(arr_start[first_bucket], arr_start[second_bucket]);

		if (fst_bucket_gt_snd_bucket) {
			//set tmp to the first element in the second bucket
			tmp = arr_start[second_bucket++];

			for (i = ++mid; i > first_bucket; i--) {
				arr_start[i] = arr_start[i - 1];
			}
			arr_start[first_bucket++] = tmp;
		}

		else if (!fst_bucket_gt_snd_bucket) {
			first_bucket++;
		}

		else {
			if (first_bucket == mid && second_bucket == end) {
				break;
			}
			tmp = arr_start[second_bucket++];
			first_bucket++;
			for (i = ++mid; i > first_bucket; i--) {
				arr_start[i] = arr_start[i - 1];
			}
			arr_start[first_bucket++] = tmp;
		}
	}
}

void InsertionSort(int start_idx, int end_idx, int unused, int* arr) {
	int* arr_start = &(arr[start_idx]);
	int n = (end_idx - start_idx) + 1;
	for (int i = 1; i < n; i++) {
		for (int j = i; j > 0; j--) {
			if (cmp_lt(arr_start[j], arr_start[j - 1]))
				swap(arr_start, j, j - 1);
			else break;
		}
	}
}

void MergeSort(int first, int last, int unused, int* arr_start) { // Non-Hybrid Algorithm
	if (last - first <= 0) {
		return;
	}
	int mid = (first + last) / 2;
	MergeInsert(first, mid, unused, arr_start);
	MergeInsert(mid + 1, last, unused, arr_start);
	merge(first, last, arr_start);
}

void MergeInsert(int first, int last, int S, int* arr_start) { // Hybrid Algorithm
	if (last - first <= 0) {
		return;
	}
	else if (last - first > S) {
		int mid = (first + last) / 2;
		MergeInsert(first, mid, S, arr_start);
		MergeInsert(mid + 1, last, S, arr_start);
		merge(first, last, arr_start);
	}
	else if (last - first <= S) {
		InsertionSort(first, last, 0, arr_start);
	}
}

int test_alg_on_rand(int count, SORT_FN_PTR fn_ptr, int size_for_mergeinsert, TIMING_INFO* timing_info) {
	CMP_CNT = 0;

	clock_t start, end;
	double cpu_time_used;

	start = clock();

	int* array = gen_input_data(count);

	fn_ptr(0, count - 1, size_for_mergeinsert, array);

	if (!test_is_sorted(array, count)) {
		return 0;
	};

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	timing_info->cpu_time = cpu_time_used;
	timing_info->cmp_cnt = CMP_CNT;
	
	return 1;
};

int test_is_sorted(int* array, int count) {
	for (int i = 0; i < count-1; i++)
	{
		if (array[i + 1] < array[i])
		{
			return 0;
		}
	}
	return 1;
}

int* gen_input_data(int count) {
	int* array = (int*)malloc(sizeof(int) * count);

	if (array == NULL)
	{
		exit(1);
	}

	for (int i = 0; i < count; i++)
	{
		array[i] = rand();
	}

	return array;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
