#include <stdio.h>

int max(int *array,int start, int end) { //s- start, //e - end
	if(start==end)
		return array[start];
	int mid = (start+end)/2;
	int max1 = max(array,start,mid);
	int max2 = max(array,mid+1,end);
	return max1>max2?max1:max2;
}

int main() {
	int array[10] = {453,222,42,34,23,4,465,4545,6534,45};
	printf("max: %d",max(array,0,9));
}
