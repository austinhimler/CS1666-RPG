#include <string>
#include <vector>
#include <iostream>
//#include <algorithm>
#include <map>

using namespace std;

class Source {
	int solution(vector<int> &A) {
		// write your code in C++14 (g++ 6.2.0)
		int a, b;
		int BestPos = 0, Max = 0;
		for (int i = 0; i < A.size()-1; i++) {
			a = i;
			int j;
			for (j = i + 1; j < A.size();j++) {
				if (A[j] != A[a]) {
					b = j;
					break;
				}
			}
			if (j >= A.size()-1) return A.size(); //if the trees only produce 1 or 2 types of fruits
			for (int k = j + 1; k < A.size(); k++) {
				if (A[k] != A[a] && A[k] != A[b]) {
					if (k - a > Max) {
						Max = k - a;
						BestPos = a;
					}
					break;
				}
			}
		}
	}
	return BestPos;
};