#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv){

	Mat depth = imread("data/depth/depth26510.png", 1);
	
	Size depth_size(depth.cols, depth.rows);

	for (int i = 0; i < depth_size.height; i++){
		Vec3b *src = depth.ptr<Vec3b>(i);
		for (int j = 0; j < depth_size.width; j++){
			Vec3b depth_info = src[j];
			double depth_data = depth_info[0];
			if (depth_data <= 150 && depth_data >= 20){
				//src[j] = Vec3b(0,0,255);
				//cout << "[" << j << "," << i << "] = " << depth_data << endl;
			}
			else{
				src[j] = Vec3b(0, 0, 0);
			}
		}
	}

	namedWindow("depth", CV_WINDOW_AUTOSIZE);
	imshow("depth", depth);
	imwrite("data/depth_data.png", depth);
	waitKey(0);

	return 0;
}