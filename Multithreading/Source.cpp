#include <iostream>
#include <iomanip>
#include <ctime>
#include <process.h>
#include <Windows.h>
/*
--------------------------------
Yasmeen Ibrahim Ahmed - 15P6049
Ahmed Maged Mohamed - 15P1047
--------------------------------
*/


using namespace std;

const int MAXARRAY = 10;
double *arrMain = new double[MAXARRAY];
double* arrFirst = new double[MAXARRAY / 2];
double* arrSecond = new double[MAXARRAY / 2];
double* arrFinal = new double[MAXARRAY];
CRITICAL_SECTION sync;

unsigned __stdcall thread1(void *arg);
unsigned __stdcall thread2(void *arg);
unsigned __stdcall thread3(void *arg);

void swap(double *x, double *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

void sort(double arr[], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}



void setarr(double arrMain[], double x[], int start, int end) {
	int j = 0;
	for (int i = start; i < end; i++)
	{
		x[j] = arrMain[i];
		j++;
	}
}

void merge(double arrFirst[], double arrSecond[], double arrFinal[], int MAXARRAY) {
	int i = 0, j = 0, k = 0;
	while (i<MAXARRAY/2 && j <MAXARRAY / 2)
	{
		if (arrFirst[i] < arrSecond[j])
			arrFinal[k++] = arrFirst[i++];
		else
			arrFinal[k++] = arrSecond[j++];
	}

	while (i < MAXARRAY/2)
		arrFinal[k++] = arrFirst[i++];

	while (j < MAXARRAY/2)
		arrFinal[k++] = arrSecond[j++];
}

void printarray(double arr[], int n)
{
	cout << "(";
	for (int i = 0; i < n; i++)
	{
		cout << arr[i];
		if (i!= n-1)
			cout << " ,";
		else
			cout << ")";
		
	}
	cout << endl;
}


void main()
{
	clock_t tstart, tend;
	HANDLE hs[3];
	unsigned th1, th2, th3;
	InitializeCriticalSection(&sync);
	for (int i = 0; i < MAXARRAY; i++)
	{
		cin >> arrMain[i];
	}

	cout << endl << "Sorting array: " ;
	printarray(arrMain, MAXARRAY);
	tstart = clock();

	hs[0] = (HANDLE)_beginthreadex(NULL, 0, thread1, NULL, CREATE_SUSPENDED, &th1);
	hs[1] = (HANDLE)_beginthreadex(NULL, 0, thread2, NULL, CREATE_SUSPENDED, &th2);
	hs[2] = (HANDLE)_beginthreadex(NULL, 0, thread3, NULL, CREATE_SUSPENDED, &th3);

	ResumeThread(hs[0]);
	ResumeThread(hs[1]);

	WaitForMultipleObjects(2, hs, true, INFINITE);

	ResumeThread(hs[2]);
	WaitForSingleObject(hs[2], INFINITE);

	tend = clock();
	cout << "_____________________________________________" << endl << "Time elapsed sorting array: " << ((tend - tstart) / (double)CLOCKS_PER_SEC) << endl << endl;
	system("PAUSE");

}

unsigned __stdcall thread1(void *arg)
{
	EnterCriticalSection(&sync);
	setarr(arrMain, arrFirst, 0, MAXARRAY / 2);
	sort(arrFirst, MAXARRAY / 2);
	cout << "First Half Sorted: ";
	printarray(arrFirst, MAXARRAY/2);
	LeaveCriticalSection(&sync);

	return 3;
}

unsigned __stdcall thread2(void *arg)
{
	EnterCriticalSection(&sync);
	setarr(arrMain, arrSecond, MAXARRAY / 2, MAXARRAY);
	sort(arrSecond, MAXARRAY / 2);
	cout << "Second Half Sorted: ";
	printarray(arrSecond, MAXARRAY / 2);
	LeaveCriticalSection(&sync);
	return 3;
}

unsigned __stdcall thread3(void *arg)
{

	EnterCriticalSection(&sync);
	merge(arrFirst, arrSecond, arrFinal, MAXARRAY);
	cout << "Final Sorted Array: ";
	printarray(arrFinal, MAXARRAY);
	LeaveCriticalSection(&sync);
	return 3;
}


