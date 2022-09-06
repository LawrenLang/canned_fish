#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <direct.h>
#include <bits/stdc++.h>

#include <opencv2/opencv.hpp>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTextCodec>

using namespace std;
using namespace cv;

const int inf = 2e9;

const double pi = acosl(-1);

struct mVec
{
    int x, y;
    mVec operator-(const mVec& t) const { mVec rec; rec.x = x - t.x, rec.y = y - t.y;  return rec; }
    mVec operator+(const mVec& t) const { mVec rec; rec.x = x + t.x, rec.y = y + t.y;  return rec; }
    mVec() {}
    mVec(int xx, int yy) : x(xx), y(yy) {}
};

double len(mVec a);

int dot(mVec a, mVec b);

int cro(mVec a, mVec b);

bool cmpxy(mVec a, mVec b);

int ConvexHull(mVec* p, mVec* cp, int sum);

double Area_CP(mVec* cp, int cnt);

void add_point(int x, int y);

void read_data(string filePath);

void cropRotateImage(Mat &src, Mat &dst,float angle);

void init();

string Q2std(QString qstring);

#endif // ALGORITHM_H
