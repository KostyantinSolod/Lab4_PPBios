#include <iostream>
#include <vector>
#include <omp.h>
#include <locale>
#include <windows.h>


// ��������� ���������� ���� ������
int calculateSum(const std::vector<int>& arr) {
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// ���������� ���������� ���� ������
int calculateSumParallel(const std::vector<int>& arr) {
    int sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    return sum;
}

// ��������� ���������� ����� ���� ������
void calculatePairwiseSum(std::vector<int>& arr) {
    while (arr.size() > 1) {
        int newSize = arr.size() / 2;

        for (int i = 0; i < newSize; ++i) {
            arr[i] += arr[arr.size() - i - 1];
        }

        arr.resize((arr.size() + 1) / 2);
    }
}

// ���������� ���������� ����� ���� ������
void calculatePairwiseSumParallel(std::vector<int>& arr) {
    while (arr.size() > 1) {
        int newSize = arr.size() / 2;

#pragma omp parallel for
        for (int i = 0; i < newSize; ++i) {
            arr[i] += arr[arr.size() - i - 1];
        }

        arr.resize((arr.size() + 1) / 2);
    }
}

// ����� ���������� �������� ������
void findMinElement(const std::vector<int>& arr, int& minValue, int& minIndex) {
    minValue = arr[0];
    minIndex = 0;

#pragma omp parallel
    {
        int localMinValue = minValue;
        int localMinIndex = minIndex;

#pragma omp for
        for (int i = 0; i < arr.size(); ++i) {
            if (arr[i] < localMinValue) {
                localMinValue = arr[i];
                localMinIndex = i;
            }
        }

        if (localMinValue < minValue) {
#pragma omp critical
            {
                if (localMinValue < minValue) { // �������� ��������
                    minValue = localMinValue;
                    minIndex = localMinIndex;
                }
            }
        }
    }
}
int main() {
    const int size = 500000;
    std::vector<int> arr(size);

    // ���������� ������
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }

    // ������� ��������� ��'������ �����
    arr[21810] = -100;

    int totalSum, minValue, minIndex;
    int task;

    std::locale::global(std::locale(""));
    std::cin.imbue(std::locale());
    std::cout.imbue(std::locale());


    // ���� ������ ��������
    std::cout << "����i�� �������� (1 - ����i����� ����, 2 - ���������� ����, "
        "3 - ����i����� ����� ����, 4 - ���������� ����� ����, "
        "5 - ����� �i�i�������� ��������): ";
    std::cin >> task;


    if (task == 1) {
        totalSum = calculateSum(arr);
        std::cout << "�������� ���� �������i� (����i�����): " << totalSum << std::endl;
    }
    else if (task == 2) {
        totalSum = calculateSumParallel(arr);
        std::cout << "�������� ���� �������i� (����������): " << totalSum << std::endl;
    }
    else if (task == 3) {
        calculatePairwiseSum(arr);
        totalSum = arr[0];
        std::cout << "�������� ����� ���� (����i�����): " << totalSum << std::endl;
    }
    else if (task == 4) {
        calculatePairwiseSumParallel(arr);
        totalSum = arr[0];
        std::cout << "�������� ����� ���� (����������): " << totalSum << std::endl;
    }
    else if (task == 5) {
        findMinElement(arr, minValue, minIndex);
        std::cout << "�i�i������� �������: " << minValue
            << ", �����i�: " << minIndex << std::endl;
    }
    else {
        std::cerr << "���i���� ���i� ��������!" << std::endl;
    }

    return 0;
}
