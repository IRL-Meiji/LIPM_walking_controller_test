#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

//本プログラムは「ヒューマノイドロボット第2版」p152~156のアルゴリズムに従う。
//本プログラムは両足支持期ナシの矢状面に拘束したLIPMの歩行パターン生成
int main()
{
  //定数
  double pi = 3.141592;
  double Tsup = 0.8;                                             //一歩の時間
  double px0 = 0.0;                                              //初期脚位置
  double zc = 0.75;                                               //LIPMの重心高さ
  double g = 9.81;                                               //重力加速度
  double Tc = sqrt(zc/g);
  double c = cosh(Tsup/Tc);
  double s = sinh(Tsup/Tc);
  int Nmax = 5;                                                 //足を踏み出す回数。何歩まで歩くか。
  double Sx[Nmax + 1] = {0.0, 0.15, 0.15, 0.15, 0.0, 0.0};         //歩幅。必要があれば、途中変えることも可能。

  //変数
  double px[Nmax + 1];                                          //着地位置
  double x_bar;                                                 //歩幅を変えるなら、これも配列にする。下も同様。
  double v_bar;                                                 //歩行素片の終端速度
  double xd[Nmax + 1];                                          //n歩目の重心の目標終端位置
  double vd[Nmax + 1];                                          //n歩目の重心の目標終端速度
  double xi[Nmax + 1];                                          //n歩目の重心の初期位置
  double vi[Nmax + 1];                                          //n歩目の重心の初速度
  double xf[Nmax + 1];                                          //n歩目の終端位置
  double vf[Nmax + 1];                                          //n歩目の終端速度
  double a = 10.0;                                              //教科書p155の式4.47の重み係数
  double b = 1.0;                                               //同上。これらをいじると歩行パターンが変わる。
  double d = a * pow(c-1,2) + b * pow(s/Tc,2);
  double px_rev[Nmax + 1];                                      //revising point.修正された着地位置

  ofstream ofs;
  ofs.open("foot_pattern.csv");                                 //歩行パターンをcsvで出力

  //初期化
  px[0] = px0;
  xi[0] = 0.0;
  vi[0] = 0.0;
  xd[0] = 0.0;
  vd[0] = 0.0;
  px_rev[0] = 0.0;
  xf[0] = c*xi[0] + Tc*s*vi[0] + (1-c)*px_rev[0];               //教科書p155の式4.45を参照。初期
  vf[0] = s/Tc*xi[0] + c*vi[0] - s/Tc*px_rev[0];

  ofs << 0 << "," << px_rev[0] << "," << xi[0] << "," << vi[0] << endl;

  for(int n = 1;n <= Nmax; n++){
    px[n] = px[n-1] + Sx[n-1];                                          //教科書p153の式4.39
    xi[n] = xf[n-1];                                                    //n-1歩目の終端位置はn歩目の開始位置
    vi[n] = vf[n-1];                                                    //速度についても同上.ただしこれらは両足支持期が無い時のみ。

    x_bar = Sx[n]/2;                                                    //教科書p153の式4.40
    v_bar = (c + 1)/(Tc * s)*x_bar;                                     //教科書p153の式4.41

    xd[n] = px[n] + x_bar;                                              //教科書p155の式4.46
    vd[n] = v_bar;                                                      //教科書p155の式4.46

    double px_rev_1 = a*(c-1)/d * (xd[n] - c*xi[n] - Tc*s*vi[n]);       //教科書p155の式4.48の上の行
    double px_rev_2 = b*s/(Tc*d) * (vd[n] - s/Tc*xi[n] - c*vi[n]);      //教科書p155の式4.48の中の行

    px_rev[n]= -1*px_rev_1 - px_rev_2;                                  //教科書p155の式4.48全体

    xf[n] = c*xi[n] + Tc*s*vi[n] + (1-c)*px_rev[n];                     //教科書p155の式4.45
    vf[n] = s/Tc*xi[n] + c*vi[n] - s/Tc*px_rev[n];                      //教科書p155の式4.45

    ofs << n << "," << px_rev[n] << "," << xi[n] << "," << vi[n] << endl;//元の着地点と修正された着地点をファイルに書き出す。
  }
}
