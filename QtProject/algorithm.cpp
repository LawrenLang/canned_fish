#include "algorithm.h"

double len(mVec a) { return sqrt(a.x * a.x + a.y * a.y); }
int dot(mVec a, mVec b) { return a.x * b.x + a.y * b.y; }
int cro(mVec a, mVec b) { return a.x * b.y - a.y * b.x; }

bool cmpxy(mVec a, mVec b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }	//笛卡尔坐标排序（配合凸包）

int ConvexHull(mVec* p, mVec* cp, int sum)		//凸包（返回逆时针）
{
    sort(p + 1, p + 1 + sum, cmpxy);

    int t = 0;
    for (int i = 1; i <= sum; i++)
    {
        while (t > 1 && cro(cp[t] - cp[t - 1], p[i] - cp[t - 1]) <= 0)
            t--;
        cp[++t] = p[i];
    }

    int st = t;
    for (int i = sum - 1; i >= 1; i--)
    {
        while (t > st && cro(cp[t] - cp[t - 1], p[i] - cp[t - 1]) <= 0)
            t--;
        cp[++t] = p[i];
    }
    return t - 1;
}

double Area_CP(mVec* cp, int cnt)			//旋转卡壳
{
    cp[cnt + 1] = cp[1];
    double ansArea = inf, ansArc = 0;
    for (int i = 1, j = 3, k = 2, l = 2; i <= cnt; i++)
    {
        while (cro(cp[i + 1] - cp[i], cp[j + 1] - cp[i]) - cro(cp[i + 1] - cp[i], cp[j] - cp[i]) > 0)
            j = j < cnt ? j + 1 : (l = 1);

        while (dot(cp[i + 1] - cp[i], cp[k + 1] - cp[i]) - dot(cp[i + 1] - cp[i], cp[k] - cp[i]) >= 0)
            k = k < cnt ? k + 1 : 1;

        l = max(l, j);
        while (dot(cp[i + 1] - cp[i], cp[l + 1] - cp[i]) - dot(cp[i + 1] - cp[i], cp[l] - cp[i]) <= 0)
            l = l < cnt ? l + 1 : 1;

        mVec base = cp[i + 1] - cp[i];
        double base_len = len(base);
        double h = (double) cro(base, cp[j] - cp[i]) / base_len;
        double w = (double) (dot(base, cp[k] - cp[i]) - dot(base, cp[l] - cp[i])) / base_len;
        double tmp = h * w;
        if(tmp < ansArea)
        {
            ansArea = tmp;
            ansArc = atan2(base.y, base.x);
            ansArc = ansArc / pi * 180;
            if(ansArc > 90) ansArc -= 90;
            while (ansArc < 0) ansArc += 90;
            if(ansArc > 45) ansArc -= 90;
        }
    }
    return ansArc;
}



void cropRotateImage(Mat &src, Mat &dst,float angle)
{
    int maxBorder =(int) (max(src.cols, src.rows) * 1.5 );
    int dx = (maxBorder - src.cols) / 2;
    int dy = (maxBorder - src.rows) / 2;
    Mat tmp;
    copyMakeBorder(src, tmp, dy, dy, dx, dx, BORDER_CONSTANT);

    Point2f center( (float)(tmp.cols/2) , (float) (tmp.rows/2));
    Mat affine_matrix = getRotationMatrix2D( center, angle, 1.0 );
    warpAffine(tmp, tmp, affine_matrix, tmp.size());

    int minx, miny, maxx, maxy;
    minx = miny = inf;
    maxx = maxy = 0;
    for(int i = 0; i < tmp.rows; i++)
    {
        for(int j = 0; j < tmp.cols; j++)
        {
            int alpha = tmp.at<Vec4b>(i, j)[3];
            if(alpha)
            {
                minx = min(minx, i);
                miny = min(miny, j);
                maxx = max(maxx, i);
                maxy = max(maxy, j);
            }
        }
    }
    Rect rect(miny, minx, maxy - miny + 1, maxx - minx + 1);
    Mat(tmp,rect).copyTo(dst);
}



string Q2std(QString qstring)
{
    return string((const char *)qstring.toLocal8Bit().constData());
}
