#include <stdio.h>
#include <stdbool.h>


int sequential_search(double val, double array[], size_t size)
{
    int index = -1;
    int iteratrions = 0;

    for (size_t i = 0; i < size; i++)
    {
        iteratrions++;
        if (array[i] == val)
        {
            index = i;
            break;
        }
    }

    printf("interations done: %d\n", iteratrions);
    return index;
}


int binary_search(double val, double array[], int size)
{
    int iteratrions = 0;
    int low = 0;
    int high = size - 1;

    while (low <= high)
    {
        iteratrions++;
        int mid = (low + high) / 2;
        double guess = array[mid];
        if (guess == val) return mid;
        else if (guess > val)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
        printf("low:%d high:%d\n", low, high);
    }

    printf("interations done: %d\n", iteratrions);
    return -1;
}

int main(void)
{
    double arr[] = {0.5, 1.2, 2.7, 3.4, 4.4, 5.6, 6.2, 7.8, 8.8, 9.1};
    double val = 9.1;

    int index = binary_search(val, arr, sizeof(arr) / sizeof(arr[0]));

    if (index != -1)
    {
        printf("arr[%d] = %g\n", index, val);
    }
    else
    {
        printf("%g not found\n", val);
    }

}
