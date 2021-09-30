#include<cnoid/SimpleController>
#include<vector>
#include<fstream>
using namespace cnoid;
using namespace std;

//bipedを動かす
//関節軌道はCSVで与える。
//初期化時にファイルから値を予め読み込んでおく。

class BipedController : public SimpleController
{
  BodyPtr ioBody;
  int time;
  //double theta1_l[10000];
  vector<double> theta1_l{0};
  //double theta2_l[10000];
  vector<double> theta2_l{0};
  //double theta3_l[10000];
  vector<double> theta3_l{0};
  //double theta1_r[10000];
  vector<double> theta1_r{0};
  //double theta2_r[10000];
  vector<double> theta2_r{0};
  //double theta3_r[10000];
  vector<double> theta3_r{0};

public:
  virtual bool initialize(SimpleControllerIO* io) override
  {
    ioBody = io->body();

    time = 0;

    for(int i=0; i < ioBody->numJoints(); ++i){
      Link* joint = ioBody->joint(i);
      joint->setActuationMode(Link::JointAngle);
      io->enableIO(joint);
    }

    //ファイルから値を予め取得
    ifstream fin("/home/yossi-mark1/choreonoid/my_test_folder/biped/angle_trajectory.csv");
    char split;
    int i = 0;
    while(1){
      fin >> theta1_l[i] >> split >> theta2_l[i] >> split >> theta3_l[i] >> split >> theta1_r[i] >> split >> theta2_r[i] >> split >> theta3_r[i];
      if(fin.eof()) break;
      i++;
      theta1_l.push_back(0);
      theta2_l.push_back(0);
      theta3_l.push_back(0);
      theta1_r.push_back(0);
      theta2_r.push_back(0);
      theta3_r.push_back(0);
    }
    fin.close();

    return true;
  }
  virtual bool control() override
  {
    Link* joint[6];

    for(int i=0; i < ioBody->numJoints(); ++i){
      joint[i] = ioBody->joint(i);
    }

    joint[0]->q_target() = theta1_l[time];
    joint[1]->q_target() = theta2_l[time];
    joint[2]->q_target() = theta3_l[time];
    joint[3]->q_target() = theta1_r[time];
    joint[4]->q_target() = theta2_r[time];
    joint[5]->q_target() = theta3_r[time];

    if(time<theta1_l.size()-2){
      time++;
    }
    else{
      time = theta1_l.size()-2;
    }

    return true;
  }
};

CNOID_IMPLEMENT_SIMPLE_CONTROLLER_FACTORY(BipedController)
