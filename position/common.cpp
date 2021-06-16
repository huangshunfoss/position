#include "common.h"

Point2f getCrossPoint(Vec4i LineA, Vec4i LineB)
{
	/*函数功能：求两条直线交点*/
	/*输入：两条Vec4i类型直线*/
	/*返回：Point2f类型的点*/
	double ka, kb, ba, bb;
	Point2f crossPoint = Point2f(0, 0);
	if (LineA[2] == LineA[0])
	{
		if (LineB[2] == LineB[0])
		{
			cout << "Parallel lines." << endl;
			return crossPoint;
		}
		else
		{
			kb = (double)(LineB[3] - LineB[1]) / (double)(LineB[2] - LineB[0]);
			bb = (LineB[0] * LineB[3] - LineB[2] * LineB[1]) / (LineB[0] - LineB[2]);

			crossPoint.x = LineA[0];
			crossPoint.y = kb * LineA[0] + bb;
			return crossPoint;
		}
	}
	else if (LineB[2] == LineB[0])
	{
		ka = (double)(LineA[3] - LineA[1]) / (double)(LineA[2] - LineA[0]);
		ba = (LineA[0] * LineA[3] - LineA[2] * LineA[1]) / (LineA[0] - LineA[2]);

		crossPoint.x = LineB[0];
		crossPoint.y = ka * LineB[0] + ba;
		return crossPoint;
	}
	else
	{
		ka = (double)(LineA[3] - LineA[1]) / (double)(LineA[2] - LineA[0]);
		kb = (double)(LineB[3] - LineB[1]) / (double)(LineB[2] - LineB[0]);
		ba = (LineA[0] * LineA[3] - LineA[2] * LineA[1]) / (LineA[0] - LineA[2]);
		bb = (LineB[0] * LineB[3] - LineB[2] * LineB[1]) / (LineB[0] - LineB[2]);
		crossPoint.x = (bb - ba) / (ka - kb);
		crossPoint.y = (ka*bb - ba * kb) / (ka - kb);
		return crossPoint;
	}
}
