#include <iostream>
#include <vector>
#include <omp.h>
#include <locale>
#include <windows.h>


// Послідовне обчислення суми масиву
int calculateSum(const std::vector<int>& arr) {
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// Паралельне обчислення суми масиву
int calculateSumParallel(const std::vector<int>& arr) {
    int sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    return sum;
}

// Послідовне обчислення парної суми масиву
void calculatePairwiseSum(std::vector<int>& arr) {
    while (arr.size() > 1) {
        int newSize = arr.size() / 2;

        for (int i = 0; i < newSize; ++i) {
            arr[i] += arr[arr.size() - i - 1];
        }

        arr.resize((arr.size() + 1) / 2);
    }
}

// Паралельне обчислення парної суми масиву
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

// Пошук мінімального елементу масиву
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
                if (localMinValue < minValue) { // Повторна перевірка
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

    // Заповнення масиву
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }

    // Вставка довільного від'ємного числа
    arr[21810] = -100;

    int totalSum, minValue, minIndex;
    int task;

    std::locale::global(std::locale(""));
    std::cin.imbue(std::locale());
    std::cout.imbue(std::locale());


    // Меню вибору завдання
    std::cout << "Оберiть завдання (1 - Послiдовна сума, 2 - Паралельна сума, "
        "3 - Послiдовна парна сума, 4 - Паралельна парна сума, "
        "5 - Пошук мiнiмального елементу): ";
    std::cin >> task;


    if (task == 1) {
        totalSum = calculateSum(arr);
        std::cout << "Загальна сума елементiв (послiдовно): " << totalSum << std::endl;
    }
    else if (task == 2) {
        totalSum = calculateSumParallel(arr);
        std::cout << "Загальна сума елементiв (паралельно): " << totalSum << std::endl;
    }
    else if (task == 3) {
        calculatePairwiseSum(arr);
        totalSum = arr[0];
        std::cout << "Загальна парна сума (послiдовно): " << totalSum << std::endl;
    }
    else if (task == 4) {
        calculatePairwiseSumParallel(arr);
        totalSum = arr[0];
        std::cout << "Загальна парна сума (паралельно): " << totalSum << std::endl;
    }
    else if (task == 5) {
        findMinElement(arr, minValue, minIndex);
        std::cout << "Мiнiмальний елемент: " << minValue
            << ", позицiя: " << minIndex << std::endl;
    }
    else {
        std::cerr << "Невiрний вибiр завдання!" << std::endl;
    }

    return 0;
}
