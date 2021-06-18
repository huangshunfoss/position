#include "back.h"
#include "common.h"

int getBackCorner(int num)
{
	//num��Ӧ����ĵ�������ͼ����ţ���Ӧ��ͬ��num������ļ���Ҳ��ͬ
	//Load raw image
	const char* path = "D:\\huangshun\\Documents\\Projects\\border positioning\\images\\back\\back cover 1.bmp";
	Mat img0 = imread(path, IMREAD_GRAYSCALE); //Default IMREAD_UNCHANGED, Do Not Use it, will cause unpredicted trouble and trap because of alpha channel is introduced to Mat.
	if (img0.empty())
	{
		cout << "!!!Empty image!!!" << endl;
		return -1;
	}
	//namedWindow("raw image", WINDOW_NORMAL);
	//imshow("raw image", img0);
	//waitKey();

	//Thresh image
	Mat thresh0;
	int back_cover_1_thresh = 200;
	threshold(img0, thresh0, back_cover_1_thresh, 255, THRESH_BINARY);
	//namedWindow("thresh", WINDOW_NORMAL);
	//imshow("thresh", thresh0);
	//waitKey();

	//Canny border detection
	Mat canny0;
	Canny(thresh0, canny0, 0, 10, 3);
	//namedWindow("canny", WINDOW_NORMAL);
	//imshow("canny", canny0);
	//waitKey();

	//Find biggest bright area's contour and mask
	vector<vector<Point> > contours0;
	vector<Vec4i> hier0;
	findContours(thresh0, contours0, hier0, RETR_TREE, CHAIN_APPROX_NONE);
	int id_contour0 = -1;
	double area_max0 = 0;
	for (int i = 0; i < contours0.size(); i++)
	{
		double area0 = contourArea(contours0[i]);
		if (area0 > area_max0)
		{
			area_max0 = area0;
			id_contour0 = i;
		}
	}
	//Fill the contour area
	Mat mask0 = Mat::zeros(img0.size(), CV_8UC1);
	drawContours(mask0, contours0, id_contour0, Scalar::all(255), -1);
	Rect rect1 = boundingRect(mask0);
	////RotatedRect rrect1 = minAreaRect(contours0[id_contour0]);
	////namedWindow("mask0", WINDOW_NORMAL);
	////imshow("mask0", mask0);
	////waitKey();
	////namedWindow("border", WINDOW_NORMAL);
	////imshow("border", img0(rect1));
	////waitKey();

	//Invert img0(rect1) to get the inner border
	Mat thresh0_rect1_inv;
	threshold(img0(rect1), thresh0_rect1_inv, back_cover_1_thresh, 255, THRESH_BINARY_INV);
	////namedWindow("img0_rect1_inv", WINDOW_NORMAL);
	////imshow("img0_rect1_inv", thresh0_rect1_inv);
	////waitKey();
	vector<vector<Point> > contours0_inv;
	vector<Vec4i> hier0_inv;
	findContours(thresh0_rect1_inv, contours0_inv, hier0_inv, RETR_TREE, CHAIN_APPROX_NONE);
	int id_contour0_inv = -1;
	double area_max0_inv = 0;
	for (int i = 0; i < contours0_inv.size(); i++)
	{
		double area0 = contourArea(contours0_inv[i]);
		if (area0 > area_max0_inv)
		{
			area_max0_inv = area0;
			id_contour0_inv = i;
		}
	}
	Mat mask0_inv = Mat::zeros(img0(rect1).size(), CV_8UC1);;
	drawContours(mask0_inv, contours0_inv, id_contour0_inv, Scalar::all(255), -1);
	Rect rect2 = boundingRect(mask0_inv);
	//namedWindow("mask0_inv", WINDOW_NORMAL);
	//imshow("mask0_inv", mask0_inv);
	//waitKey();
	//namedWindow("border_inv", WINDOW_NORMAL);
	//imshow("border_inv", thresh0_rect1_inv(rect2));
	//waitKey();

	////Calc A, B
	//Point2i A, B;
	//A.x = rect1.x + rect2.x + int(rect2.width / 2);
	//A.y = rect1.y + rect2.y + rect2.height;
	//B.x = rect1.x + rect2.x;
	//B.y = rect1.y + rect2.y + int(rect2.height / 2);
	//Mat img0_display;
	//img0.copyTo(img0_display);
	//circle(img0_display, A, 50, Scalar(0, 0, 255), -1);
	//circle(img0_display, B, 50, Scalar(0, 0, 255), -1);
	//namedWindow("img0_display", WINDOW_NORMAL);
	//imshow("img0_display", img0_display);
	//waitKey();
	//imwrite("./AB.bmp", img0_display);
	////��AB��ķ�����ȱ�����Ҫ��һ���Ľ��ĵط���������յõ�����Ʒ����ģ���������õ���ABλ�û᲻׼ȷ

	//Hough Line Transform
	//���ڵ����⣺�õ����߿��ܲ�ֹ2�������º����Ľ���������³���Բ�ǿ��
	vector<Vec4i> lines0, linesf; // will hold the results of the detection
	HoughLinesP(canny0, lines0, 1, CV_PI / 180, 100, 500, 100);
	// Draw the lines
	Mat img0_display = img0.clone();
	for (size_t i = 0; i < lines0.size(); i++)
	{
		if (lines0[i][0] < rect1.x + rect2.x || lines0[i][2] < rect1.x + rect2.x || lines0[i][1] > rect1.y + rect2.y + rect2.height || lines0[i][3] > rect1.y + rect2.y + rect2.height)
		{
			continue;
		}
		line(img0_display, Point(lines0[i][0], lines0[i][1]), Point(lines0[i][2], lines0[i][3]), Scalar(50), 40, LINE_AA);
		linesf.push_back(Vec4i(lines0[i][0], lines0[i][1], lines0[i][2], lines0[i][3]));
	}
	//get cross point
	Point2f corner = getCrossPoint(linesf[0], linesf[1]);
	cout << corner << endl;
	circle(img0_display, corner, 40, Scalar(50), -1);
	//add text
	string corner_x(to_string(int(corner.x)));
	string corner_y(to_string(int(corner.y)));
	string text = "(" + corner_x + "," + " " + corner_y + ")";
	Point2i org = Point2i(int(corner.x), int(corner.y)+100);
	putText(img0_display, text, org, FONT_HERSHEY_COMPLEX, 2, Scalar(50), 5);
	//display result
	namedWindow("Result", WINDOW_NORMAL);
	imshow("Result", img0_display);
	waitKey();
	imwrite("./back corner.bmp", img0_display);

	getchar();
	destroyAllWindows();
	return 0;
}
