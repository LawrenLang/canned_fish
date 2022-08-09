#include "deal.h"
#include <QDir>
#include <QDebug>
#include <direct.h>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <QProgressDialog>
#include <QApplication>
#include <QMessageBox>
using namespace std;
using namespace cv;

const int maxn = 4e3 + 5, inf = 2e9;
const double eps = 1e-7, pi = acosl(-1);
int dcmp(double x) { return (x > eps) - (x < -eps); }
ofstream out;

struct mVec
{
    int x, y;
    mVec operator-(const mVec& t) const { mVec rec; rec.x = x - t.x, rec.y = y - t.y;  return rec; }
    mVec operator+(const mVec& t) const { mVec rec; rec.x = x + t.x, rec.y = y + t.y;  return rec; }
    mVec() {}
    mVec(int xx, int yy) : x(xx), y(yy) {}
}P[maxn * maxn], CP[maxn * 4];



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
        while (t > 1 && dcmp(cro(cp[t] - cp[t - 1], p[i] - cp[t - 1])) <= 0)
            t--;
        cp[++t] = p[i];
    }

    int st = t;
    for (int i = sum - 1; i >= 1; i--)
    {
        while (t > st && dcmp(cro(cp[t] - cp[t - 1], p[i] - cp[t - 1])) <= 0)
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
        while (dcmp(cro(cp[i + 1] - cp[i], cp[j + 1] - cp[i]) - cro(cp[i + 1] - cp[i], cp[j] - cp[i])) > 0)
            j = j < cnt ? j + 1 : (l = 1);

        while (dcmp(dot(cp[i + 1] - cp[i], cp[k + 1] - cp[i]) - dot(cp[i + 1] - cp[i], cp[k] - cp[i])) >= 0)
            k = k < cnt ? k + 1 : 1;

        l = max(l, j);
        while (dcmp(dot(cp[i + 1] - cp[i], cp[l + 1] - cp[i]) - dot(cp[i + 1] - cp[i], cp[l] - cp[i])) <= 0)
            l = l < cnt ? l + 1 : 1;

        mVec base = CP[i + 1] - CP[i];
        double base_len = len(base);
        double h = (double) cro(base, CP[j] - CP[i]) / base_len;
        double w = (double) (dot(base, CP[k] - CP[i]) - dot(base, CP[l] - CP[i])) / base_len;
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



int N = 0, M = 0;

void add_point(int x, int y)
{
    N++;
    P[N].x = x;
    P[N].y = y;
}
cv::Mat img;
void read_data(string filePath)
{
    img = imread(filePath, IMREAD_UNCHANGED);
    for (int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            int alpha = img.at<Vec4b>(i, j)[3];
            if(alpha)
            {
                add_point(i, j);
            }
        }
    }
}

void cropRotateImage(Mat &src,Mat &dst,float angle)
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

void init()
{
    for(int i = 0; i <= N; i++)
    {
        P[i].x = P[i].y = 0;
        CP[i].x = CP[i].y = 0;
    }
    N = 0, M = 0;
}

string Q2std(QString qstring)
{
    return string((const char *)qstring.toLocal8Bit().constData());
}

int solve(QString src_path, QString dst_path)
{
    out.open(Q2std(dst_path) + "/ans.txt");
    out << fixed << setprecision(6);

    vector<string> vec;
    vector<QString> qvec;
    QDir dir = (src_path);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (auto fileInfo, fileInfoList)
    {
        if(fileInfo.isFile())
        {
            string filename = Q2std(fileInfo.fileName());

            if(filename.length() > 4 && filename.substr(filename.length() - 4) == ".png")
            {
                cout << filename << endl;
                vec.push_back(filename);
                qvec.push_back(fileInfo.fileName());
            }
        }
    }



    QProgressDialog progress;
    progress.setFixedSize(620, 150);
    progress.setWindowTitle(QString::fromStdString("正在转化中🐟..."));
    progress.setLabelText(QString::fromStdString("正在转化中..."));
    progress.setCancelButtonText(QString::fromStdString("取消"));
    progress.setRange(0, vec.size());//设置范围
    progress.setModal(true);//设置为模态对话框
    progress.setStyleSheet("QProgressBar{"
                               "border:1px solid rgb(0, 173, 47);"   // 进度框
                               "height:20;"
                               "background: #d9d9d9;"     // 背景条
                               "text-align:center;"
                               "color:white;"    // 字体颜色
                               "border-radius:10px;"    // 弧度
                               "font-size:14px;"        //
                           "}"
                           "QProgressBar::chunk{"      // 进度条样式
                               "border-radius:10px;"    // 斑马线圆角
                               "background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,"
                               "stop:0 #00ffff, stop:1 #70DB93);"
                           "}");
    progress.show();
    int cnt = 0;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        string filename = vec[i];

        progress.setLabelText(QString::fromStdString("正在处理：") + qvec[i]);

        string srcPath = Q2std(src_path) + '/' + filename;
        string dstPath = Q2std(dst_path) + '/' + filename;
        read_data(srcPath);
        M = ConvexHull(P, CP, N);
        CP[0] = CP[M];
        double ans = Area_CP(CP, M);
        out << filename << ":" << ans << endl;

        Mat dst;
        cropRotateImage(img, dst, -ans);
        imwrite(dstPath, dst);
        init();

        progress.setValue(i);
        cnt++;

        //用户取消的话则中止
        if (progress.wasCanceled())
        {
            break;
        }

        //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        QCoreApplication::processEvents();
    }
    QApplication::restoreOverrideCursor();
    progress.close();
    out.close();
    return cnt;
}

