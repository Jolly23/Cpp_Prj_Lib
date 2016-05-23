// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <process.h>
using namespace std;


int main()
{	
	int a = 1, b = 2, c = 5, d = 10, e = 20, f = 50, g = 100, h = 200;
	int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0;
	int sum = 0;
	int money = 0;
	int testTime = 0;
	
	while (true) {
		i2 = 0;
		while (true) {
			i3 = 0;
			while (true) {
				i4 = 0;
				while (true) {
					i5 = 0;
					while (true) {
						i6 = 0;
						while (true) {
							i7 = 0;
							while (true) {
								i8 = 0;
								while (true) {
									testTime++;
									cout << sum << endl;
									money = a*i8 + b*i7 + c*i6 + d*i5 + e*i4 + f*i3 + g*i2 + h*i1;
									if (money == 200) {
										++sum;
										break;
									}
									if (money > 200)
										break;
									i8 += 1;
								}
								i7 += 1;
								if (i7*b+i6*c+i5*d+i4*e+i3*f+i2*g+i1*h > 200)
									break;
							}

							i6 += 1;
							if (i6*c+i5*d+i4*e+i3*f+i2*g+i1*h > 200)
								break;
						}

						i5 += 1;
						if (i5*d+i4*e+i3*f+i2*g+i1*h > 200)
							break;
					}

					i4 += 1;
					if (i4*e+i3*f+i2*g+i1*h > 200)
						break;
				}

				i3 += 1;
				if (i3*f+i2*g+i1*h > 200)
					break;
			}

			i2 += 1;
			if (i2*g+i1*h > 200)
				break;
		}

		i1 += 1;
		if (i1*h > 200)
			break;
	}
	/*
	while (true) {
		i2 = 0;

		while (true) {
			money = a*i2 + b*i1;
			if (money == 10) {
				sum++;
				break;
			}
			if (money > 10) {
				break;
			}

			i2 += 1;
		}

		i1 += 1;
		if (i1*b > 10)
			break;
	}*/
	cout << "Way = " << sum << endl;
	cout << "SumTimes" << testTime << endl;
	system("pause");
    return 0;
}

