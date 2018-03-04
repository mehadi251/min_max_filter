#include <iostream>
#include <math.h>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace cv;
using namespace std;

int rows, cols;

void histogram(Mat image,string x)
{
	int freq[256];
	memset(freq, 0, sizeof(freq));
	int rs = image.rows;
	int cls = image.cols;
	
	for (int i = 0; i < rs; i++)
	{
		uchar *im_ptr = image.ptr<uchar>(i);
		for (int j = 0; j < cls; j++)
		{
			freq[im_ptr[j]]++;
			
		}
	}
	Mat histo(256, 255, CV_8UC1, 255);
	float minimum=255;
	for (int i = 0; i < 256; i++)
	{
		if (freq[i] < minimum && freq[i]>0)minimum = freq[i];
	}
	minimum /= 256;
	cout << minimum << endl;
	for (int i = 0; i < 256; i++)
	{
		float plot =(float)freq[i] / (rows*cols);
		plot *= 256;
		plot /= minimum;
		cout << plot << endl;
		line(histo, Point(i, 256-plot), Point(i, 256),.5);
	}
	imshow(x, histo);

}


int main() {
	int mask_size;
	cout << "Mask Size? : ";
	cin >> mask_size;

	if (mask_size % 2 == 0)
	{
		cout << "Invalid mask_size";
		return 0;
	}

	Mat image = imread("lena.jpg", 0);
	imshow("Original Image", image);
	

	int size = mask_size / 2;
	rows = image.rows;
	cols = image.cols;


	Mat max_filtered, min_filtered;
	copyMakeBorder(image, max_filtered, size, size, size, size, BORDER_REPLICATE);
	copyMakeBorder(image, min_filtered, size, size, size, size, BORDER_REPLICATE);

	for (int x = 0; x < rows; x++)
	{
		uchar *ptr_in = image.ptr<uchar>(x);

		uchar *mxptr_out = max_filtered.ptr<uchar>(x);
		uchar *mnptr_out = min_filtered.ptr<uchar>(x);

		for (int y = 0; y < cols; y++)
		{
			int max = 0, min = 256;
			int sum = 0, pos = 0;
			
			for (int i = -size; i <= size; i++)
			{
				for (int j = -size; j <= size; j++)
				{
					int cindex = y + j;

						int new_val = (int)((ptr_in+i)[cindex]);
						
						if (new_val < min)
						{
							min = new_val;
						}
						if (new_val > max)
						{
							max = new_val;
						}
				}
			}
			mxptr_out[y] = max;
			mnptr_out[y] = min;
		}
	}

	imshow("Max Filter", max_filtered);

	imshow("Min Filter", min_filtered);

	histogram(image, "Histogram of Input Image");
	histogram(max_filtered, "Histogram of Max Filter");
	histogram(min_filtered, "Histogram of Mix Filter");

	waitKey(0);
	return 0;
}
