//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace std;
//using namespace cv;
//
//int main()
//{
//	//Load raw image
//	const char* path0 = "D:\\huangshun\\Documents\\Projects\\border positioning\\images\\back cover 1.bmp";
//	Mat img0 = imread(path0, IMREAD_GRAYSCALE); //Default IMREAD_UNCHANGED, Do Not Use it, will cause unpredicted trouble and trap because of alpha channel is introduced to Mat.
//	if (img0.empty())
//	{
//		cout << "!!!Empty image!!!" << endl;
//		return -1;
//	}
//	namedWindow("raw image", WINDOW_NORMAL);
//	imshow("raw image", img0);
//	waitKey();
//
//	//Calc thresh image
//	Mat thresh0;
//	//Mat thresh0 = Mat::zeros(img0.size(), CV_8UC1);
//	int back_cover_1_thresh = 200;
//	threshold(img0, thresh0, back_cover_1_thresh, 255, THRESH_BINARY);
//	namedWindow("thresh", WINDOW_NORMAL);
//	imshow("thresh", thresh0);
//	waitKey();
//
//	//Calc canny border
//	Mat canny0;
//	Canny(thresh0, canny0, 0, 10, 3);
//	namedWindow("canny", WINDOW_NORMAL);
//	imshow("canny", canny0);
//	waitKey();
//
//	// Standard Hough Line Transform
//	vector<Vec4i> lines0; // will hold the results of the detection
//	HoughLinesP(canny0, lines0, 1, CV_PI / 180, 100, 500, 100);
//	// Draw the lines
//	Mat img0_empty = Mat::zeros(img0.size(), CV_8UC1);
//	for (size_t i = 0; i < lines0.size(); i++)
//	{
//		if (lines0[i][0] < 500 || lines0[i][2] < 500 || lines0[i][1] > 2200 || lines0[i][3] > 2200)
//		{
//			continue;
//		}
//		line(img0_empty, Point(lines0[i][0], lines0[i][1]), Point(lines0[i][2], lines0[i][3]), Scalar(255), 3, LINE_AA);
//	}
//	namedWindow("lines", WINDOW_NORMAL);
//	imshow("lines", img0_empty);
//	waitKey();
//
//	//Find biggest bright area's contour and mask
//	vector<vector<Point> > contours0;
//	vector<Vec4i> hier0;
//	findContours(thresh0, contours0, hier0, RETR_TREE, CHAIN_APPROX_NONE);
//	int id_contour0 = -1;
//	double area_max0 = 0;
//	for (int i = 0; i < contours0.size(); i++)
//	{
//		double area0 = contourArea(contours0[i]);
//		if (area0 > area_max0)
//		{
//			area_max0 = area0;
//			id_contour0 = i;
//		}
//	}
//
//	Mat mask0 = Mat::zeros(img0.size(), CV_8UC1);
//	drawContours(mask0, contours0, id_contour0, Scalar::all(255), -1);
//	Rect rect1 = boundingRect(mask0);
//	//RotatedRect rrect1 = minAreaRect(contours0[id_contour0]);
//	namedWindow("mask0", WINDOW_NORMAL);
//	imshow("mask0", mask0);
//	waitKey();
//	namedWindow("border", WINDOW_NORMAL);
//	imshow("border", img0(rect1));
//	waitKey();
//
//	//Invert img0(rect1) to get the inner border
//	Mat thresh0_rect1_inv;
//	threshold(img0(rect1), thresh0_rect1_inv, back_cover_1_thresh, 255, THRESH_BINARY_INV);
//	namedWindow("img0_rect1_inv", WINDOW_NORMAL);
//	imshow("img0_rect1_inv", thresh0_rect1_inv);
//	waitKey();
//	vector<vector<Point> > contours0_inv;
//	vector<Vec4i> hier0_inv;
//	findContours(thresh0_rect1_inv, contours0_inv, hier0_inv, RETR_TREE, CHAIN_APPROX_NONE);
//	int id_contour0_inv = -1;
//	double area_max0_inv = 0;
//	for (int i = 0; i < contours0_inv.size(); i++)
//	{
//		double area0 = contourArea(contours0_inv[i]);
//		if (area0 > area_max0_inv)
//		{
//			area_max0_inv = area0;
//			id_contour0_inv = i;
//		}
//	}
//	Mat mask0_inv = Mat::zeros(img0(rect1).size(), CV_8UC1);;
//	drawContours(mask0_inv, contours0_inv, id_contour0_inv, Scalar::all(255), -1);
//	Rect rect2 = boundingRect(mask0_inv);
//	namedWindow("mask0_inv", WINDOW_NORMAL);
//	imshow("mask0_inv", mask0_inv);
//	waitKey();
//	namedWindow("border_inv", WINDOW_NORMAL);
//	imshow("border_inv", thresh0_rect1_inv(rect2));
//	waitKey();
//
//	//Calc A, B
//	Point2i A, B;
//	A.x = rect1.x + rect2.x + int(rect2.width / 2);
//	A.y = rect1.y + rect2.y + rect2.height;
//	B.x = rect1.x + rect2.x;
//	B.y = rect1.y + rect2.y + int(rect2.height / 2);
//	Mat img0_display;
//	img0.copyTo(img0_display);
//	circle(img0_display, A, 50, Scalar(0, 0, 255), -1);
//	circle(img0_display, B, 50, Scalar(0, 0, 255), -1);
//	namedWindow("img0_display", WINDOW_NORMAL);
//	imshow("img0_display", img0_display);
//	waitKey();
//	imwrite("./AB.bmp", img0_display);
//	//缺点和需要进一步改进的地方：拍照得到的样品是歪的，这种情况得到的AB位置会不太准确
//
//	return 0;
//}