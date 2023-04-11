#include <stdio.h>

void print_array(int array[], int arSize)
{
	for (int i = 0; i < arSize; i++)
	{
		printf("%4d ", array[i]);
	}
	putchar('\n');
}

void bubble_sort(int array[], int arSize)
{
	for (int i = 0; i < arSize; i++)
	{
		for (int j = 0; j < (arSize - i - 1); j++)
		{
			if (array[j] > array[j+1])
			{
				int t = array[j+1];
				array[j+1] = array[j];
				array[j] = t;

				print_array(array, arSize);
				getchar();
			}
		}
	}
}

int main()
{
	int a[10];
	for (int i = 0; i < 10; i++)
	{
		scanf("%d", &a[i]);
	}
	getchar();
	bubble_sort(a, 10);
	return 0;
}
