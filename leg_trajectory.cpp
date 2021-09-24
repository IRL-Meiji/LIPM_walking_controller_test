#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

//関数のプロトタイプ宣言

//遊脚が矩形波の軌道をたどる時、各時刻での位置を求める関数
template <typename T>
T swing_rectangle_x(T zankle, T px_in, T px_fin, T Tsup, T t);

template <typename T>
T swing_rectangle_z(T zankle, T px_in, T px_fin, T Tsup, T t);

//遊脚が正弦波の軌道をたどる時、各時刻での位置を求める関数
//遊脚がサイクロイドの軌道をたどる時、各時刻での位置を求める関数。
int main()
{
  double px_rev[10];                           //修正された(実際の)足踏み位置
  double zankle = 0.1;                         //足裏から足首までの長さ。ロボットのパラメータ
  double xswing_r;                             //各時刻での右足首のx位置
  double xswing_l;                             //各時刻での左足首のx位置
  double zswing_r;                             //各時刻での右足首のz位置
  double zswing_l;                             //各時刻での左足首のz位置
  double Tsup = 0.8;                           //一歩にかかる時間
  double total_t = 0.0;

  //値を取得
  ifstream fin("foot_pattern.csv");
  int n = 0;
  double dummy;                               //  使わない値をここに捨てる
  char split;
  while(1){
    fin >> dummy >> split >> px_rev[n] >> split >> dummy >> split >> dummy;
    if(fin.eof()) break;
    n++;
  }
  int Nmax = n-1;

  ofstream ofs;
  ofs.open("leg_trajectory.csv");

  //一歩目だけ別
  for(double t=0.0;t<=Tsup;t+=0.001){
    xswing_r = px_rev[0];
    zswing_r = zankle;

    xswing_l = swing_rectangle_x(zankle,px_rev[0],px_rev[1],Tsup,t);
    zswing_l = swing_rectangle_z(zankle,px_rev[0],px_rev[1],Tsup,t);

    if(xswing_l > 0.0){
      xswing_l = -1*xswing_l;                    //一歩目だけ後ろに下がるのでこれが必用
    }

    total_t += 0.001;

    ofs << total_t << "," << xswing_r << "," << zswing_r << "," << xswing_l << "," << zswing_l << endl;
  }

  for(int n=2;n<=Nmax;n++){
    if(n%2 == 1){
      for(double t=0.0;t<=Tsup;t+=0.001){
        xswing_r = px_rev[n-1];
        zswing_r = zankle;

        xswing_l = swing_rectangle_x(zankle,px_rev[n-2],px_rev[n],Tsup,t);
        zswing_l = swing_rectangle_z(zankle,px_rev[n-2],px_rev[n],Tsup,t);

        total_t += 0.001;

        ofs << total_t << "," << xswing_r << "," << zswing_r << "," << xswing_l << "," << zswing_l << endl;
      }
    }
    else{
      for(double t=0.0;t<=Tsup;t+=0.001){
        xswing_r = swing_rectangle_x(zankle,px_rev[n-2],px_rev[n],Tsup,t);
        zswing_r = swing_rectangle_z(zankle,px_rev[n-2],px_rev[n],Tsup,t);

        xswing_l = px_rev[n-1];
        zswing_l = zankle;

        total_t += 0.001;

        ofs << total_t << "," << xswing_r << "," << zswing_r << "," << xswing_l << "," << zswing_l << endl;
      }

    }
  }
  return 0;
}

template <typename T>
T swing_rectangle_x(T zankle, T px_in, T px_fin, T Tsup, T t){
  //田崎勇一氏の論文から。
  double vconst;
  double zmax = 0.2;
  vconst = (2*(zmax - zankle) + abs(px_fin - px_in))/Tsup;

  if(0<=t && t < (zmax-zankle)/vconst){
    return px_in;
  }
  else if((zmax-zankle)/vconst<=t && t<(zmax - zankle + abs(px_fin - px_in))/vconst){
    return px_in + vconst*(t-(zmax-zankle)/vconst);
  }
  else{
    return px_fin;
  }
}

template <typename T>
T swing_rectangle_z(T zankle, T px_in, T px_fin, T Tsup, T t){
  //田崎勇一氏の論文から。
  double vconst;
  double zmax = 0.2;
  vconst = (2*(zmax - zankle) + abs(px_fin - px_in))/Tsup;

  if(0<=t && t < (zmax-zankle)/vconst){
    return zankle + vconst*t;
  }
  else if((zmax-zankle)/vconst<=t && t<(zmax - zankle + abs(px_fin - px_in))/vconst){
    return zmax;
  }
  else{
    return zmax - vconst*(t - (zmax - zankle + abs(px_fin - px_in))/vconst);//ここだけおかしい
  }
}
