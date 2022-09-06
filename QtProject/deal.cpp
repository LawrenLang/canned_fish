#include "deal.h"

const int maxn = 4e3 + 5;

ofstream out, re_out;

int N = 0, M = 0;

cv::Mat img;

mVec P[maxn * maxn], CP[maxn * 4];

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

void init()
{
    for(int i = 0; i <= N; i++)
    {
        P[i].x = P[i].y = 0;
        CP[i].x = CP[i].y = 0;
    }
    N = 0, M = 0;
}

void add_point(int x, int y)
{
    N++;
    P[N].x = x;
    P[N].y = y;
}

int solve(QString src_path, QString dst_path)
{
    out.open(Q2std(src_path) + "/ans.txt");
    re_out.open(Q2std(src_path) + "/re_ans.txt");

    out << fixed << setprecision(6);
    re_out << fixed << setprecision(6);

    vector<string> vec;
    vector<QString> qvec;
    QDir dir = src_path + "/images";
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (auto fileInfo, fileInfoList)
    {
        if(fileInfo.isFile())
        {
            string filename = Q2std(fileInfo.fileName());

            if(filename.length() > 4 && filename.substr(filename.length() - 4) == ".png")
            {
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
    out << "{\n";
    re_out << "{\n";

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        string filename = vec[i];

        progress.setLabelText(QString::fromStdString("正在处理：") + qvec[i]);

        string srcPath = Q2std(src_path) + "/images/" + filename;
        read_data(srcPath);
        M = ConvexHull(P, CP, N);
        CP[0] = CP[M];
        double ans = Area_CP(CP, M);
        string temp = filename.substr(0, filename.size() - 4);
        out << "\"" << temp << "\"" << ":"
            << "\"" << temp << " #r" << ans << "\"";
        if(i != vec.size() - 1)
            out << ",";
        out << endl;

        re_out << "\"" << temp << " #r" << ans << "\"" << ":"
               << "\"" << temp << "\"";
        if(i != vec.size() - 1)
            re_out << ",";
        re_out << endl;

        if(dst_path != "")
        {
            string dstPath = Q2std(dst_path) + '/' + filename;
            Mat dst;
            cropRotateImage(img, dst, -ans);
            imwrite(dstPath, dst);
        }

        init();

        progress.setValue(i);
        cnt++;

        //用户取消的话则中止
        if (progress.wasCanceled())
        {
            out << "}";
            re_out << "}";
            break;
        }

        //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        QCoreApplication::processEvents();
    }
    out << "}";
    re_out << "}";
    QApplication::restoreOverrideCursor();
    progress.close();
    out.close();
    re_out.close();
    return cnt;
}


