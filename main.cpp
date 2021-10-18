#include<iostream>
#include<cstring>
#include<fstream>
#include<vector>
#include<stdio.h>
#include<algorithm> //使用sort函数
#include<iomanip>
using namespace std;
int flag=0;
string adminpasseword="0";
/*密码为string类，账号为int型*/
static int tnumber=0,snumber=0; //初始化教师数量，学生数量，教师的课程学生数量
//整型变量统一首字母大写

class account  //账号虚基类,以此基础派生出学生、教师以及管理员类
{ 
public:
  account(int a,char *b) {Account=a; if(b!=NULL){strcpy(Password,b);} }
  int Account; //账号（统一规定：学生为2xxx，教师为1xx，管理员为0）
  char Password[20];
  virtual void print() const=0;
};
class Course
{
public:
char course[20]="  "; //该科目的名称
  int  credit=-1; //科目的学分
  
  int tnum; //记录授课教师的账号
};
class grade:public Course  //成绩类，基类
{  
public:
    grade(float a=-1,char* b=NULL,int c=-1) {Grade=a; 
    
   if(b!=NULL) {strcpy(course,b);}
   credit=c;
   }
  float Grade=-1;
  char GPA[5]; //A-F

};
class saccount:public account
{
public:
  saccount(int a=0,char *b=nullptr,char* name1=NULL):account(a, b) {
    if(name1!=NULL){strcpy(name,name1);
        }  } 
        virtual void print() const;
     //== 运算符重载，判断是否相等
    bool operator ==(const saccount &p)
    { if(strcmp(name,p.name)==0&&Account==p.Account)
       {   return true;}
      else {return false;}
    }
      char name[9]="***"; 
    int Grade=-1;  char GPA[4]="N/A";//供teacher类使用，方便信息录入
};
void saccount::print() const
{cout<<"学号为"<<this->Account<<"名字为"<<this->name<<endl;}
bool stunumcompare(const saccount& a1,const saccount& a2)//自定义排序
 {
    return (a1.Account<=a2.Account);
}
bool stugradecompare(const saccount &a1,const saccount &a2)
{
  return (a1.Grade<=a2.Grade);
}

class taccount:public account,public Course
{
public:
  taccount(int a=0,char* b=nullptr,char* name1=NULL,char* name2=NULL,int c=0):account( a, b) 
  {if(name1!=NULL&&name2!=NULL)
        { strcpy(name,name1); strcpy(course,name2);}
        credit=c;
        }  
    virtual void print() const;
    char name[9]=" ";
   //运算符== 重载
   bool operator ==(const taccount &p)
    { if(strcmp(name,p.name)==0&&Account==p.Account)
       {   return true;}
      else {return false;}
    }
  
};
void taccount::print() const
{
  cout<<"账号为"<<this->Account<<"名字为"<<this->name<<endl;
}
class teacher:public taccount //教师类继承教师账号类，在此基础上增加学生类数组
{
  public:
teacher(int a=0,char* b=nullptr,char* name1=NULL,char* name2=NULL,int c=0):taccount(a,b,name1,name2,c){}
 //若有学生申请成绩复议，会先将其绩点改为GG显示 
  friend class saccount;
    void search();//查找
    void modify();
    void sorts();
    void fuyi();
  void insert();//插入
  void del(int acc) ;//删除,供学生退课使用。
  void display(); //打印学生信息： 可选择按照学号或成绩排序输出
  void passwordmodify();
  void read(string newfile); //读取学生信息到指定文件夹
  void stat(); //统计学生信息：总人数，绩点分布，平均分数
};
void teacher::sorts()
{  vector<saccount> va;
    saccount stu;
    char str[10];
    int num;
sprintf(str,"%d",teacher::Account);
strcat(str,".txt");
    fstream file(str,ios::in|ios::binary);//打开存取学生对象的文件
    file.seekg(0,ios::beg);
    while(file.peek()!=EOF)
    {
        file.read((char *)&stu, sizeof(saccount));//读取文件
        va.push_back(stu);//将学生对象存入容器
    }
        std::cout<<"请选择按学号大小或成绩高低进行排序"<<endl;
        std::cout<<"按1选择按照学号排序"<<endl;
        std::cout<<"按2选择按照成绩排序"<<endl;
        cin>>num;
        if(num==1)
       { 
     sort(va.begin(),va.end(),stunumcompare);}//使用sort进行排序}
     else if(num==2)
     {sort(va.begin(),va.end(),stugradecompare);}
     else {cout<<"暂无此选项，将返回上一级"<<endl;}
 fstream file1(str, ios::out|ios::trunc);
 file1.seekp(0,ios::beg);
    for (int i=0;i<va.size();i++)
    {
        file1.write((char *)&va[i], sizeof(saccount));//将排序好的写入文件
    }
    file.close();//关闭文件
    file1.close();
}
void teacher::passwordmodify()
{ 
  taccount tea;
  fstream file("teacher.txt",ios::in|ios::binary|ios::out|ios::binary);
  char pss1[15],pss2[15];
  while(!file.peek()==EOF)
  {
    file.read((char*)&tea,sizeof(taccount));
    if(teacher::Account==tea.Account)
    { std::cout<<"请输入新密码"<<endl;
      cin>>pss1;
      std::cout<<"请再次输入新密码"<<endl;
      cin>>pss2;
      if(!strcmp(pss1,pss2)) { cout<<"修改成功"<<endl;
      strcpy(tea.Password,pss2);
      file.seekp(-sizeof(taccount),ios::cur);
      file.write((char*)&tea,sizeof(taccount));
      } 
      else {cout<<"两次密码输入不一，请重新修改"<<endl;}      
       }
  }
}
void teacher::search()
{while(1)  
{std::cout<<"*\t按照账号查找请按1\t*"<<endl;
std::cout<<"*\t按照姓名查找请按2\t*"<<endl;
std::cout<<"*\t返回上一层请按其他\t*"<<endl;
char ch;
std::cin>>ch;
char huiche=getchar();
char str[10];
sprintf(str,"%d",teacher::Account);
strcat(str,".txt");
if(ch!='1'&&ch!='2') {break;}
else if(ch=='1')
{std::cout<<"请输入要查询的学生的学号"<<endl;
int num;
int flag=0;
std::cin>>num;

fstream file(str,ios::in|ios::binary);
while(file.peek()!=EOF)
{
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    if(num==tea.Account)
    { flag=1;
      std::cout<<"学生帐号    学生姓名    课程分数    课程绩点"<<endl;
      std::cout<<tea.Account<<"\t  "<<tea.name<<"\t  "<<tea.Grade<<"\t"<<tea.GPA<<endl;
      break;
    }
}
if(flag==0) {std::cout<<"查无此人"<<endl;}
file.close();
}
else if(ch=='2')
{std::cout<<"请输入要查询的学生的名字"<<endl;
char ming[13];
int flag=0;
std::cin>>ming;
fstream file(str,ios::in|ios::binary);
while(file.peek()!=EOF)
{
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    if(strcmp(tea.name,ming)==0)
    { flag=1;
      std::cout<<"学生帐号      学生姓名    课程分数      课程绩点"<<endl;
      std::cout<<tea.Account<<"\t  "<<tea.name<<"\t  "<<tea.Grade<<"\t"<<tea.GPA<<endl;
      break;
    }
    if(flag==0) {std::cout<<"查无此人"<<endl;}
    file.close();
}
}}
}
void teacher::insert() //登入成绩,并在输入错误时进行提醒。
{   
 if(flag!=3) 
 {cout<<"成绩录入系统未开放"<<endl;
 return;
 }
char str[10];
int      num;
int   flag=0;
sprintf(str,"%d",teacher::Account);
strcat(str,".txt");
while(1)  
{
  std::cout<<"*\t请输入学生的学号\t*"<<endl;
std::cout<<"学号："<<endl;
cin>>num;
int location=-1;
fstream file(str,ios::in|ios::binary|ios::out|ios::binary);
while(file.peek()!=EOF)
{   
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    location++;
    if(num==tea.Account)
    { flag=1;
      while(1)
      {std::cout<<"请输入分数"<<endl; 
       cin>>tea.Grade;
       if(tea.Grade>=0&&tea.Grade<=100)
        {break;}
        else{cout<<"百分制分数，输入错误，请重新输入"<<endl;
         continue;}
       }
      while(1)
      {std::cout<<"请输入绩点(A-F)"<<endl;
      cin>>tea.GPA;
      if(tea.GPA[0]=='A'||tea.GPA[0]=='B'||tea.GPA[0]=='C'||tea.GPA[0]=='D'||tea.GPA[0]=='F') 
      { if(strlen(tea.GPA)==2) 
        {if(tea.GPA[1]=='+'||tea.GPA[1]=='-')
         { break;}
          else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
        }
         else if(strlen(tea.GPA)==1)
          {break;}
          else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
      }
      else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
      }
        file.seekp(location*sizeof(saccount),ios::beg);
        file.write((char*)&tea,sizeof(saccount));
        file.close();
       std::cout<<"学生帐号      学生姓名    课程分数    课程绩点"<<endl;
      cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
       std::cout<<tea.Account<<"\t"<<tea.name<<"\t\t"<<tea.Grade<<"\t"<<tea.GPA<<endl;
      break;
    }
        if(flag==0) {cout<<"查无此人"<<endl; continue;}}
        cout<<"按0退出录入，按其他继续"<<endl;
         char t;
         cin>>t;
         cin.get();
         if(t=='0') {return;} 
        
        }}
void teacher::modify()
{ 
   if(flag!=3&&flag!=4) 
 {cout<<"成绩录入系统未开放"<<endl;
 return;
 }
  char str[10];
int     num;
int   flag=0;
sprintf(str,"%d",teacher::Account);
strcat(str,".txt");
while(1)  
{std::cout<<"*\t请输入学生的学号\t*"<<endl;
std::cout<<"学号："<<endl;
cin>>num;
 int location=-1;
fstream file(str,ios::in|ios::binary|ios::out|ios::binary);
while(file.peek()!=EOF)
{   location++;
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    if(num==tea.Account)
    { flag=1;
     while(1)
      {std::cout<<"请输入新的分数："<<endl; 
       cin>>tea.Grade;
       if(tea.Grade>=0&&tea.Grade<=100)
        {break;}
        else{cout<<"百分制分数，输入错误，请重新输入"<<endl;
         continue;}
       }
      while(1)
      {std::cout<<"请输入新的绩点(A-F)"<<endl;
      cin>>tea.GPA;
      if(tea.GPA[0]=='A'||tea.GPA[0]=='B'||tea.GPA[0]=='C'||tea.GPA[0]=='D'||tea.GPA[0]=='F') 
      { if(strlen(tea.GPA)==2) 
        {if(tea.GPA[1]=='+'||tea.GPA[1]=='-')
         { break;}
          else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
        }
         else if(strlen(tea.GPA)==1)
          {break;}
          else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
      }
      else {cout<<"输入错误，请重新输入"<<endl;
           continue;}
      }
      file.seekp(location*sizeof(saccount),ios::beg);
       file.write((char*)&tea,sizeof(saccount));
       std::cout<<"学生帐号    学生姓名    课程分数    课程绩点"<<endl;
      std::cout<<tea.Account<<"\t  "<<tea.name<<"\t  "<<tea.Grade<<"\t"<<tea.GPA<<endl;
      cout<<"退出修改请输入0，继续请按其他"<<endl;
      char c1;
      cin>>c1;
      char huiche=getchar();
      if(c1=='0') {
        cout<<"退出成功，正在返回上一级"<<endl;
        return;}
    }}
    if(flag==0) {cout<<"查无此人"<<endl; continue;}
}}
void teacher::display() //显示学生信息
{ int a=0;
  char str[10];
  sprintf(str,"%d",teacher::Account);
  strcat(str,".txt");
  fstream file(str,ios::in|ios::binary);
     std::cout<<"学生帐号      学生姓名      课程分数  学生绩点 "<<endl;
      while(file.peek()!=EOF)
      {saccount tea;
       file.read((char*)&tea,sizeof(saccount));
       a=1;
       cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
       std::cout<<tea.Account<<"\t"<<tea.name<<"\t\t"<<tea.Grade<<"\t"<<tea.GPA<<endl;
      }
    if(a==0) {std::cout<<"暂无学生信息录入"<<endl;} 
    file.close();}
void teacher::stat()
{
  int a=0;
  static int num[12];
  float Averagegpa=0;
  float Averagegrade=0;
  float best=0;
  float worst=100;
  char str[10];
  sprintf(str,"%d",teacher::Account);
  strcat(str,".txt");
  fstream file(str,ios::in|ios::binary);
      while(file.peek()!=EOF)
      {saccount tea;
       file.read((char*)&tea,sizeof(saccount));
       a++;
       Averagegrade+=tea.Grade;
       if(tea.Grade>best) {best=tea.Grade;}
       if(tea.Grade<worst) {worst=tea.Grade;}
      if(tea.GPA[0]=='A'||tea.GPA[0]=='a')
        {num[0]++;
         Averagegpa+=4;}
      else if(!strcmp("B+",tea.GPA)||!strcmp("b+",tea.GPA))
      {num[1]++;Averagegpa+=3.6;}
      else if(!strcmp("B",tea.GPA)||!strcmp("b",tea.GPA))  
      {num[2]++;Averagegpa+=3.3;}
      else if(!strcmp("B-",tea.GPA)||!strcmp("b-",tea.GPA))
      {num[3]++;Averagegpa+=3;}
      else if(!strcmp("C+",tea.GPA)||!strcmp("c+",tea.GPA))
      {num[4]++;Averagegpa+=2.6;}
      else if(!strcmp("C",tea.GPA)||!strcmp("c",tea.GPA))
      {num[5]++;Averagegpa+=2.3;}
      else if(!strcmp("C-",tea.GPA)||!strcmp("c-",tea.GPA))
      {num[6]++;Averagegpa+=2.0;}
      else if(!strcmp("D+",tea.GPA)||!strcmp("d+",tea.GPA))
      {num[7]++;Averagegpa+=1.7;}
      else if(!strcmp("D",tea.GPA)||!strcmp("d",tea.GPA))
      {num[8]++;Averagegpa+=1.3;}
      else if(!strcmp("F",tea.GPA)||!strcmp("f",tea.GPA))
      {num[9]++;Averagegpa+=0;}}
    Averagegpa=Averagegpa/float(a);
    Averagegrade=Averagegrade/float(a);
    cout<<"您的学生人数为："<<a<<"其中:"<<endl; 
  cout<<"A-及以人数为:"<<num[0]<<"B+人数为:"<<num[1]<<"B人数为:"<<num[2]<<"B-人数为:"<<num[3]<<endl;
      cout<<"C+人数为:"<<num[4]<<"C人数为:"<<num[5]<<"C-人数为"<<num[6]<<endl;
      cout<<"D+人数为:"<<num[7]<<"D人数为:"<<num[8]<<"F(不及格)人数为:"<<num[9]<<endl; 
    cout<<"最高分数为："<<best<<"最低分数为："<<worst<<endl;
    cout<<"平均分数为："<<Averagegrade<<"/平均绩点为："<<Averagegpa<<endl;
      }
void teacher::del(int acc)
{   vector<saccount> vt;
      saccount student;
      int flag=0;
      int num=acc;
       char str[10];
          sprintf(str,"%d",teacher::Account);
          strcat(str,".txt");
      fstream file(str,ios::in|ios::binary);
      while(file.peek()!=EOF)
      {
          file.read((char*)&student,sizeof(saccount));
          if(student.Account==num) 
          {   snumber--;
              flag=1;
              continue;
          }
          vt.push_back(student);
       }
       fstream file1(str,ios::out|ios::trunc);
       for(int i=0;i<vt.size();i++)
        {  
          file1.write((char*)&vt[i],sizeof(saccount)); }
       if(flag==1) {std::cout<<"退课成功"<<endl;
       }
       else {cout<<"您已退学"<<endl;}
        file.close();
        file1.close();  }
void teacher::read(string newfile)
{ char str[10];
sprintf(str,"%d",teacher::Account);
strcat(str,".txt");
  ifstream input(str,ios::in|ios::binary);
  if(input.fail())
  { 
    std::cout<<"信息未开始录入，读取失败"<<endl;
   return;
  }
   ofstream ob(newfile);
   if(ob.fail())
   {
       std::cout<<"目标文件未能成功创建，读取失败"<<endl;
       return;
   }
    input.unsetf(ios::skipws);
    char ch;
    while(input>>ch)
    {
        ob<<ch;
    }
    input.close();
    ob.close();
    std::cout<<"读取成功"<<endl;

}
void teacher::fuyi()
{ if(flag!=4) {cout<<"现在还不是成绩复议阶段"<<endl;
   return;}
  int  flahg=0;
  char str[10];
sprintf(str,"%d",teacher::Account);
  strcat(str,".txt");
  fstream file(str,ios::in|ios::binary);
      while(file.peek()!=EOF)
      {saccount tea;
       file.read((char*)&tea,sizeof(saccount));
       if(strcmp(tea.GPA,"GG")==0)
       {  flahg=1;
         cout<<"学号："<<tea.Account<<"姓名："<<tea.name<<"请求复议"<<endl;
       }
      }
      file.close();
      if(flahg==0) {cout<<"无人复议"<<endl;}
  
}
class student:public saccount,public grade
{public:
 void print(); //打印全科成绩与个人信息
 void passwordmodify(); //修改账号密码
 void fuyi();
 void tuike();
 void xuanke();
 grade gpa[20];
 friend class teacher;
 student(int a=0,char *b=NULL):saccount(a,b,"c")
 {;}
};
void student::fuyi()
{ if(flag!=4) {cout<<"现在还不是成绩复议阶段"<<endl;
   return;}
  char a[20];  
  cout<<"请输入申请复议的科目课程名称"<<endl;
    cin>>a;
     char huiche=getchar();
   int location=-1;
    for(int i=0;i<20;i++)
    {  if(gpa[i].course==NULL||strcmp(gpa[i].course,"N/A")==0)
      {continue;}
       else if(strcmp(gpa[i].course,a)==0)
       {  cout<<"该课程你的绩点为："<<gpa[i].GPA<<endl;
         cout<<"是否确定复议？--输入Y进行确定"<<endl;
         char b=getchar();
         char huiche=getchar();
         if(b=='Y') {cout<<"复议请求已提交，将于七个工作日内通知结果"<<endl;
         strcpy(this->gpa[i].GPA,"GG");
         char str[10];
         sprintf(str,"%d",gpa[i].tnum);
          strcat(str,".txt");
          fstream file(str,ios::out|ios::binary|ios::in);
          while(file.peek()!=EOF)
          {saccount tea;
            location++;
          file.read((char*)&tea,sizeof(saccount));
          if(this->Account==tea.Account)
          {
            strcpy(tea.GPA,"GG");
            file.seekp(location*sizeof(saccount),ios::beg);
       file.write((char*)&tea,sizeof(saccount));  }   }
          file.close();
         return;
         }
         else {cout<<"取消复议成功，退出"<<endl;
           return;}
        }

       }
       
    }
void student::tuike()
{  cout<<"你的课表为:"<<endl;
   cout<<"课程名称:"<<"\t\t"<<"课程学分："<<endl;

   for(int i=0;i<=19;i++)
   {
     if(this->gpa[i].credit!=-1)
  {
    cout<<setiosflags(ios::fixed)<<setprecision(12)<<setiosflags(ios::left);
    cout<<setw(18)<<this->gpa[i].course<<"\t"<<setw(16)<<this->gpa[i].credit<<endl;
  }
   }
  if(flag>=3) {cout<<"退课系统已关闭，不允许退课"<<endl;
   return;}
  int flag1=1;  
   char a[15];
   char b[15];
  if(flag1==1) {
    cout<<"请输入要退课的课程名称：(按2退出)"<<endl;
    cin>>a;
    int flag11=0;
     for(int i=0;i<19;i++)
  {  
     if(strcmp(gpa[i].course,a)==0)
     {    cout<<"请再次输入该课程名称确定退课:"<<endl;
          cin>>a;
          if(!strcmp(a,gpa[i].course))
          {
            teacher t1(gpa[i].tnum,"123","aa","aa");
             t1.del(student::Account);     //实例化，调用teacher类的del函数
              gpa[i].credit=-1;         
         flag11=1;
          cout<<"退课成功"<<endl;
          }
          
          break;}

} 
   if(flag11==0) {cout<<"操作失败：取消或课程信息错误"<<endl;}
  }

}
void student::passwordmodify()
{saccount tea;
int location=-1;
  fstream file("student.txt",ios::in|ios::out|ios::binary);
  char pss1[15],pss2[15];
  file.seekg(0,ios::beg);
  while(file.peek()!=EOF)
  {
    file.read((char*)&tea,sizeof(saccount));
    location++;
    if(this->Account==tea.Account)
    { std::cout<<"请输入新密码"<<endl;
      cin>>pss1;
      std::cout<<"请再次输入新密码"<<endl;
      cin>>pss2;
      if(!strcmp(pss1,pss2)) { cout<<"修改成功"<<endl;
      strcpy(tea.Password,pss2);
      file.seekp(location*sizeof(taccount),ios::beg);
      file.write((char*)&tea,sizeof(saccount));
      } 
      else {cout<<"两次密码输入不一，请重新修改"<<endl;}      
       }
  }
}

void student::print() //打印成绩
{   
  int num[12]={0};
  int a=0,i=0;
  float Averagegpa=0;
  float Averagegrade=0;
  char str[10];
  int totalcredit=0;
      
  for(int z=0;z<=19;z++)
   {
  if(this->gpa[i].Grade!=-1)
      {   
        totalcredit+=this->gpa[i].credit;
       Averagegrade+=this->gpa[i].Grade;
      if(student::gpa[i].GPA[0]=='A'||this->gpa[i].GPA[0]=='a')
        {num[0]++;
         Averagegpa+=4*this->gpa[i].credit;}
      else if(!strcmp("B+",this->gpa[i].GPA)||!strcmp("b+",this->gpa[i].GPA))
      {num[1]++;Averagegpa+=3.6*this->gpa[i].credit;}

      else if(!strcmp("B",this->gpa[i].GPA)||!strcmp("b",this->gpa[i].GPA))  
      {num[2]++;Averagegpa+=3.3*this->gpa[i].credit;}

      else if(!strcmp("B-",this->gpa[i].GPA)||!strcmp("b-",this->gpa[i].GPA))
      {num[3]++;Averagegpa+=3*this->gpa[i].credit;}

      else if(!strcmp("C+",this->gpa[i].GPA)||!strcmp("c+",this->gpa[i].GPA))
      {num[4]++;Averagegpa+=2.6*this->gpa[i].credit;}

      else if(!strcmp("C",this->gpa[i].GPA)||!strcmp("c",this->gpa[i].GPA))
      {num[5]++;Averagegpa+=2.3*this->gpa[i].credit;}

      else if(!strcmp("C-",this->gpa[i].GPA)||!strcmp("c-",this->gpa[i].GPA))
      {num[6]++;Averagegpa+=2.0*this->gpa[i].credit;}

      else if(!strcmp("D+",this->gpa[i].GPA)||!strcmp("d+",this->gpa[i].GPA))
      {num[7]++;Averagegpa+=1.7*this->gpa[i].credit;}

      else if(!strcmp("D",this->gpa[i].GPA)||!strcmp("d",this->gpa[i].GPA))
      {num[8]++;Averagegpa+=1.3*this->gpa[i].credit;}

      else if(!strcmp("F",this->gpa[i].GPA)||!strcmp("f",this->gpa[i].GPA))
      {num[9]++;Averagegpa+=0;}
       a++;
      i++;
      }
     
      }
    Averagegpa=Averagegpa/float(totalcredit);
    Averagegrade=Averagegrade/float(a);
   std::cout<<"你的成绩报告如下"<<endl;
   std::cout<<" 科目"<<"  "<<"分数"<<"  "<<"绩点"<<endl;
   for(int i=0;i<19;i++)
   { if(student::gpa[i].Grade>0&&student::gpa[i].GPA[0]!='N')
      {
          if(strcmp(student::gpa[i].GPA,"GG")==0)
          {
            cout<<student::gpa[i].course<<"   申请复议中...   "<<endl;
          }
          else{ std::cout<<student::gpa[i].course<<"\t"<<student::gpa[i].Grade<<"\t"<<student::gpa[i].GPA<<endl; }
      } }
       cout<<"A-及以上课程数为:"<<num[0]<<"B+课程数为:"<<num[1]<<"B课程数为:"<<num[2]<<"B-课程数为:"<<num[3]<<endl;
      cout<<"C+课程数为:"<<num[4]<<"C课程数为:"<<num[5]<<"C-课程数为"<<num[6]<<endl;
      cout<<"D+课程数为:"<<num[7]<<"D课程数为:"<<num[8]<<"F(不及格)课程数为:"<<num[9]<<endl;     
    cout<<"总学分为："<<totalcredit<<"/平均分数为："<<Averagegrade<<"/平均绩点为："<<Averagegpa<<endl;

}
void student::xuanke()
{
if(flag!=1) {cout<<"选课系统关闭，不允许选课"<<endl;
   return;}
 cout<<"你的课表为:"<<endl;
   cout<<"课程名称:"<<"\t\t"<<"课程学分："<<endl;

   for(int i=0;i<=19;i++)
   {
     if(this->gpa[i].credit!=-1)
  {
    cout<<setiosflags(ios::fixed)<<setprecision(12)<<setiosflags(ios::left);
    cout<<setw(18)<<this->gpa[i].course<<"\t"<<setw(16)<<this->gpa[i].credit<<endl;
  }
   }
   int num=0;
   int coursenum,coursenum2;
fstream file("teacher.txt",ios::in|ios::binary);
     std::cout<<"本学期开放选课的课程如下："<<endl;
     std::cout<<"课程序号    教师姓名    课程名称        课程学分"<<endl;
      while(file.peek()!=EOF)   //eof会出现多读一次的错误
      {taccount tea;
       file.read((char*)&tea,sizeof(taccount));
        num++;
      cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
      cout<<setw(12)<<num<<setw(12)<<tea.name<<setw(19)<<tea.course<<setw(18)<<tea.credit<<endl;
      }
      file.close();
      while(1)
      {
        fstream file1("teacher.txt",ios::in|ios::binary);
       cout<<"请输入课程序号进行报名:(按-1返回上一级)"<<endl;
       cin>>coursenum;
       if(coursenum==-1) { file1.close();return;}
       else if(coursenum>num)
       {
         cout<<"无此课程，请重新输入"<<endl;
         file1.close();
         continue;
       }
       cout<<"你将报名课程序号为"<<coursenum<<"的课程"<<endl;
       cout<<"请再次输入课程序号进行提交"<<endl;
       cin>>coursenum2;
       if(coursenum2!=coursenum) {cout<<"两次输入不一致，提交选课失败"<<endl; 
       file1.close();
       continue;}
       else {
      file1.seekg(0,ios::beg);
      int num2=0;
       while(file1.peek()!=EOF&&num2<=coursenum)   //eof会出现多读一次的错误
      {taccount tea;
       file1.read((char*)&tea,sizeof(taccount));
        num2++;
        if(num2==coursenum)
        { int flag3=0;  
        int i=0;
          for( i=0;i<=19;i++)
          {  
             if(!strcmp(tea.course,this->gpa[i].course)&&tea.Account==this->gpa[i].tnum)   //避免重复选课
              {flag3=1;}
          }
          i=0;
          while(this->gpa[i].credit!=-1&&i<=19)
          {
            i++;
          }
          if(flag3==1) {
            file1.close();
            cout<<"你已经报名该课程，请勿重复提交"<<endl;
            break;}
          cout<<"成功报名：";
          std::cout<<"课程序号:"<<num2<<"教师姓名:"<<tea.name<<"课程名称 "<<tea.course<<" 课程学分:"<<tea.credit<<"的课程"<<endl;
        this->gpa[i].tnum=tea.Account;
        this->gpa[i].credit=tea.credit;
        strcpy(this->gpa[i].course,tea.course);
        char str[10];
       sprintf(str,"%d",tea.Account);
      strcat(str,".txt");
      fstream file2(str,ios::out|ios::app|ios::binary);
      saccount stu(this->Account,"cccc",this->name);
      file2.write((char*)&stu,sizeof(saccount));
      file2.close();
        }     
      }
      file1.close();
       }

      }
}
class admin:public account //管理员账号，负责录入学生和教师信息
{   
public: 
  admin(int a=0,char* b=nullptr):account(a,b){
    } 
    int flag=0; //标志阶段：0-4，0不允许任何非管理员进行操作。1：开放学生选课系统；2：选课系统关闭，退课开始； 
    //3：退课系统关闭，教师登入成绩； 4：教师登入成绩结束，开始成绩复议阶段
   friend class taccount;
   friend class saccount;
 private:
   virtual void print()   const {;}

};
void operatestudentmenu()
{printf("*************************************************\n");
  printf("*\t\t请选择功能\t\t\t*\n");
  printf("*************************************************\n");
  printf("*\t\t1.录入学生信息\t\t\t*\n");
  printf("*\t\t2.打印学生信息\t\t\t*\n");
  printf("*\t\t3.保存学生信息\t\t\t*\n");
  printf("*\t\t4.读取学生信息\t\t\t*\n"); //即将信息读取到指定名称文本文件中
  printf("*\t\t5.统计所有学生人数\t\t*\n");
  printf("*\t\t6.查找学生信息\t\t\t*\n");
  printf("*\t\t7.修改学生信息\t\t\t*\n");
  printf("*\t\t8.删除学生信息\t\t\t*\n");
  printf("*\t\t0.退出,返回上一级\t\t\t*\n");
  printf("*************************************************\n");}
void operateteachermenu()
{

  printf("*************************************************\n");
  printf("*\t\t请选择功能\t\t\t*\n");
  printf("*************************************************\n");
  printf("*\t\t1.录入教师信息\t\t\t*\n");
  printf("*\t\t2.打印教师信息\t\t\t*\n");
  printf("*\t\t3.保存教师信息\t\t\t*\n");
  printf("*\t\t4.读取教师信息\t\t\t*\n");
  printf("*\t\t5.统计所有教师人数\t\t*\n");
  printf("*\t\t6.查找教师信息\t\t\t*\n");
  printf("*\t\t7.修改教师信息\t\t\t*\n");
  printf("*\t\t8.删除教师信息\t\t\t*\n");
    printf("*\t\t0.退出，返回上一级\t\t\t*\n");
  printf("*************************************************\n");
}
void teachermenu()
{  
  printf("*************************************************\n");
  printf("*\t\t请选择功能\t\t\t*\n");
  printf("*************************************************\n");
  printf("*\t\t1.录入学生成绩\t\t\t*\n");
  printf("*\t\t2.打印学生信息\t\t\t*\n");  //可选择按照学号或成绩排序输出
  printf("*\t\t3.读取学生信息\t\t\t*\n"); //即将信息读取到指定名称文本文件中
  printf("*\t\t4.统计学生信息\t\t\t*\n");   //输出学生总人数，平均成绩以及GPA分布
  printf("*\t\t5.查找学生信息\t\t\t*\n");
  printf("*\t\t6.排序学生信息\t\t\t*\n");
  printf("*\t\t7.修改学生信息\t\t\t*\n");
  printf("*\t\t8.更改账号密码\t\t\t*\n");
  printf("*\t\t9.查看复议请求\t\t\t*\n");
  printf("*\t\t0.退出,返回上一级\t\t*\n");
  printf("*************************************************\n");

}
void studentmenu()
{ printf("*********************************************\n");
  printf("*\t\t请选择功能\t\t\t*\n");
  printf("*********************************************\n");
  printf("*\t\t1.查看个人成绩\t\t\t*\n");
  printf("*\t\t2.进入选课系统\t\t\t*\n");   
  printf("*\t\t3.进入退课系统 \t\t\t*\n");
  printf("*\t\t4.申请成绩复议\t\t\t*\n");   
  printf("*\t\t5.更改个人密码\t\t\t*\n");
  printf("*\t\t  6.退出系统  \t\t\t*\n");}
void print(string File) //打印信息
{  int a=0;
    if(File=="teacher.txt")
    {fstream file("teacher.txt",ios::in|ios::binary);
        std::cout<<"教师帐号    教师密码    教师姓名    授课课程      课程学分"<<endl;
      while(file.peek()!=EOF)   //eof会出现多读一次的错误
      {taccount tea;
       file.read((char*)&tea,sizeof(taccount));
       a=1;
           cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
           cout<<setw(12)<<tea.Account<<setw(12)<<tea.Password<<setw(12)<<tea.name<<setw(12)<<tea.course<<"\t"<<tea.credit<<endl;
      }
    if(a==0) {std::cout<<"暂无教师信息录入"<<endl;} 
    file.close(); 
    }
    else if(File=="student.txt")
    {fstream file("student.txt",ios::in|ios::binary);
     std::cout<<"学生帐号\t学生密码\t学生姓名    "<<endl;
      while(file.peek()!=EOF)
      {saccount tea;
       file.read((char*)&tea,sizeof(saccount));
       a=1;
       std::cout<<tea.Account<<"\t"<<tea.Password<<"\t\t"<<tea.name<<endl;
      }
    if(a==0) {std::cout<<"暂无学生信息录入"<<endl;} 
    file.close(); }
}
void save()    //保存
{
  std::cout<<"*\t保存成功\t*"<<endl;

}
void read(char *newfile,string File) //将信息读取到指定名称文本文件中
{ ifstream input(File,ios::in|ios::binary);
  if(input.fail())
  { 
    std::cout<<"信息未开始录入，读取失败"<<endl;
   return;
  }
   ofstream ob(newfile);
   if(ob.fail())
   {
       std::cout<<"目标文件未能成功创建，读取失败"<<endl;
       return;
   }
    input.unsetf(ios::skipws);
    char ch;
    while(input>>ch)
    {
        ob<<ch;
    }
    input.close();
    ob.close();
    std::cout<<"读取成功"<<endl;
}
void insert(string File,int *b) //对配置文件进行写入，参数为文件名称与插入名单的人数变量的地址
{  int j=*b;
   int m;
 std::cout<<"请输入要增加的人数"<<endl;
 std::cin>>m;
 do
 { if(File=="teacher.txt")
    {   taccount tea;
        while(1){
        std::cout<<"请输入第"<<j-(*b)+1<<"个教师的账号"<<endl;
        std::cin>>tea.Account;
        if(tea.Account>9999&&tea.Account<100000) {break;}
        else {std::cout<<"错误，请重新设置"<<endl;}
        }
         std::cout<<"请输入第"<<j-(*b)+1<<"个教师的密码"<<endl;
        std::cin>>tea.Password;
        std::cout<<"请输入第"<<j-(*b)+1<<"个教师的姓名"<<endl;
        std::cin>>tea.name;
         std::cout<<"请输入第"<<j-(*b)+1<<"个教师的课程名称"<<endl;
        std::cin>>tea.course;
         std::cout<<"请输入第"<<j-(*b)+1<<"个教师的课程学分"<<endl;
        std::cin>>tea.credit;
        char qw=getchar();
         char str[10];
          sprintf(str,"%d",tea.Account);
          strcat(str,".txt");
          fstream file100(str); //创建教师账号.txt文件
       j++;
       fstream file("teacher.txt",ios::out|ios::app);
        file.write((char*)&tea,sizeof(taccount));
        file.close();
    }
  else if(File=="student.txt")
  {   saccount tea ;
       while(1)
       { std::cout<<"提示学生学号为8位：由入学年份与四位序号组成"<<endl;
         std::cout<<"请输入第"<<j-(*b)+1<<"个学生的账号"<<endl;
        std::cin>>tea.Account;
        if(tea.Account<100000000&&tea.Account>9999999) {break;}
        else { std::cout<<"错误，请重新设置"<<endl;continue;}
        } 
        std::cout<<"请输入第"<<j-(*b)+1<<"个学生的密码"<<endl;
        std::cin>>tea.Password;
        std::cout<<"请输入第"<<j-(*b)+1<<"个学生的姓名"<<endl;
        std::cin>>tea.name;
        char ch=getchar();
       j++;
       fstream file("student.txt",ios::out|ios::in|ios::app);
        file.write((char*)&tea,sizeof(saccount));
        file.close();
  }
   
 }while(j<*b+m);
 *b+=m;
 std::cout<<"信息添加完毕"<<endl;
}
void countnumber(string File) //统计总人数
{  
   if(File=="teacher.txt")
    {
        std::cout<<"当前教师总人数为"<<tnumber<<endl;
    }
  else 
  {
      std::cout<<"当前学生总人数为"<<snumber<<endl;
  }
  
}
void search(string File)
{  
  while(1)  
{std::cout<<"*\t按照账号查找请按1\t*"<<endl;
std::cout<<"*\t按照姓名查找请按2\t*"<<endl;
std::cout<<"*\t返回上一层请按其他\t*"<<endl;
char ch;
std::cin>>ch;
char huiche=getchar();
if(ch!='1'&&ch!='2') {break;}

if(File=="teacher.txt")
{if(ch=='1')
{std::cout<<"请输入要查询的教师的帐号"<<endl;
int num;
int flag=0;
std::cin>>num;
fstream file("teacher.txt",ios::in|ios::binary);
while(file.peek()!=EOF)
{
    taccount tea;
    file.read((char*)&tea,sizeof(taccount));
    if(num==tea.Account)
    { flag=1;
      std::cout<<"教师帐号    教师密码    教师姓名    授课课程      课程学分"<<endl;
      cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
           cout<<setw(12)<<tea.Account<<setw(12)<<tea.Password<<setw(12)<<tea.name<<setw(12)<<tea.course<<"\t"<<tea.credit<<endl;

      break;
    }
}
if(flag==0) {std::cout<<"查无此人"<<endl;}
file.close();
}
else if(ch=='2')
{std::cout<<"请输入要查询的教师的名字"<<endl;
char ming[13];
int flag=0;
std::cin>>ming;
fstream file("teacher.txt",ios::in|ios::binary|ios::out);
while(file.peek()!=EOF)
{
    taccount tea;
    file.read((char*)&tea,sizeof(taccount));
    if(strcmp(tea.name,ming)==0)
    { flag=1;
     std::cout<<"教师帐号    教师密码    教师姓名    授课课程      课程学分"<<endl;
      cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
           cout<<setw(12)<<tea.Account<<setw(12)<<tea.Password<<setw(12)<<tea.name<<setw(12)<<tea.course<<"\t"<<tea.credit<<endl;
          file.close();
      break;
    }
}
    if(flag==0) {std::cout<<"查无此人"<<endl;}
        file.close();
}}
else 
{
    if(ch=='1')
{std::cout<<"请输入要查询的学生的帐号"<<endl;
int num;
int flag=0;
std::cin>>num;
fstream file("student.txt",ios::in|ios::binary);
while(file.peek()!=EOF)
{
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    if(num==tea.Account)
    { flag=1;
      std::cout<<"学生帐号    学生密码    学生姓名  "<<endl;
      std::cout<<tea.Account<<"\t  "<<tea.Password<<"\t  "<<tea.name<<endl;
      break;
    }
}
if(flag==0) {std::cout<<"查无此人"<<endl;}
file.close();
}
else if(ch=='2')
{std::cout<<"请输入要查询学生的名字"<<endl;
char ming[13];
int flag=0;
std::cin>>ming;
char huiche=getchar();
fstream file("student.txt",ios::in|ios::binary);
while(file.peek()!=EOF)
{
    saccount tea;
    file.read((char*)&tea,sizeof(saccount));
    if(strcmp(tea.name,ming)==0)
    { flag=1;
      std::cout<<"学生帐号    学生密码    学生姓名   "<<endl;
      std::cout<<tea.Account<<"\t  "<<tea.Password<<"\t  "<<tea.name<<"\t"<<endl;
      break;
    }
    if(flag==0) {std::cout<<"查无此人"<<endl;}
    file.close();
}}
}
}
}
void fix(string File)
{  int flag=0;
   int location=-1;
   std::cout<<"请输入账号"<<endl;
   int n;
   std::cin>>n;
   char huiche=getchar();
   int acc;
   char pas[15];
   char nam[10],cou[10];
   saccount sea;
   taccount tea;
     if(File=="student.txt")
      { fstream file1("student.txt",ios::in|ios::binary);
        while(file1.peek()!=EOF&&flag==0)
         { location++;
           file1.read((char*)&sea,sizeof(saccount));
          if(sea.Account==n){ 
            flag=1;  
          std::cout<<"学生帐号    学生密码    学生姓名  "<<endl;
          cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
           cout<<setw(12)<<sea.Account<<setw(12)<<sea.Password<<setw(12)<<sea.name<<endl;
          break;}
    }
file1.close();
if(flag==0) {std::cout<<"查无此人"<<endl;

return ;
}
fstream file2("student.txt",ios::out|ios::binary|ios::in);
while(1)
{
std::cout<<"------------------"<<endl;
std::cout<<"    按1修改账号    "<<endl;
std::cout<<"    按2修改密码    "<<endl;
std::cout<<"    按3修改姓名    "<<endl;
std::cout<<"    按4退出菜单    "<<endl;
std::cout<<"------------------"<<endl;
int cr;
cin>>cr;
 if(cr==1) {std::cout<<"请输入新的账号"<<endl; 
     std::cin>>sea.Account;
     if(sea.Account<10000000||sea.Account>99999999)
     {
       cout<<"账号应为8位，修改失败"<<endl;
       continue;
     }
  }
    else if(cr==2) {std::cout<<"请输入新的密码"<<endl;
     std::cin>>pas;   
     strcpy(sea.Password,pas);
    }
    else if(cr==3){std::cout<<"请输入新的姓名"<<endl;
     std::cin>>nam;  
     strcpy(sea.name,nam); 
    }
    else if(cr==4){
    std::cout<<"修改完成"<<endl;
    file2.seekp(location*sizeof(saccount),ios::beg);
    file2.write((char*)&sea,sizeof(saccount));
    file2.close();
    return;
    }
    else{std::cout<<"请在1~4中选择"<<endl;}
}
}
      
    else 
      {   fstream file1("teacher.txt",ios::in|ios::binary|ios::out);
          while(file1.peek()!=EOF&&flag==0)
          {  location++;
          file1.read((char*)&tea,sizeof(taccount));
          if(tea.Account==n){ flag=1;  
          std::cout<<"教师帐号    教师密码    教师姓名  授课课程   课程学分"<<endl;
           cout<<setiosflags(ios::fixed)<<setprecision(6)<<setiosflags(ios::left);
           cout<<setw(12)<<tea.Account<<setw(12)<<tea.Password<<setw(12)<<tea.name<<setw(12)<<tea.course<<setw(12)<<tea.credit<<endl;
      break;}
    }
file1.close();
if(flag==0) {std::cout<<"查无此人"<<endl;
 return;
}
fstream file2("teacher.txt",ios::out|ios::binary|ios::in);
while(1)
{
std::cout<<"------------------"<<endl;
std::cout<<"    按1修改账号    "<<endl;
std::cout<<"    按2修改密码    "<<endl;
std::cout<<"    按3修改姓名    "<<endl;
std::cout<<"    按4修改课程    "<<endl;
std::cout<<"   按5修改课程学分 "<<endl;
std::cout<<"    按6退出菜单    "<<endl;
std::cout<<"------------------"<<endl;
int cr;
cin>>cr;
    if(cr==1) {std::cout<<"请输入新的账号"<<endl; 
     int acc=tea.Account;
     std::cin>>tea.Account;
     if(tea.Account<10000||tea.Account>99999)
     {cout<<"账号应为五位，修改失败"<<endl;
     tea.Account=acc;
     continue;
     }
}
    else if(cr==2) {std::cout<<"请输入新的密码"<<endl;
     std::cin>>pas;   
     strcpy(tea.Password,pas);
    
    }
    else if(cr==3){std::cout<<"请输入新的姓名"<<endl;
     std::cin>>nam;  
     strcpy(tea.name,nam); 
    }
    else if(cr==4) 
    {std::cout<<"请输入新的课程名"<<endl;
     std::cin>>nam;  
     strcpy(tea.name,nam); 

    }
    else if(cr==5)
    {std::cout<<"请输入新的课程学分"<<endl;
     std::cin>>tea.credit;  
    }
    else if(cr==6){
    std::cout<<"修改完成"<<endl;
    cin.get();
    file2.seekp(location*sizeof(taccount),ios::beg);
    file2.write((char*)&tea,sizeof(taccount));
    file2.close();
    return;
    }
    else{std::cout<<"请在1~6中选择"<<endl;}
}
}    }

void del(string File)
{   if(File=="student.txt")
    { vector<saccount> vt;
      saccount student;
      int flag=0;
      int num;
      char str[20];
      std::cout<<"请输入要删除的学生的学号"<<endl;
      std::cin>>num;
      char huiche=getchar();
      fstream file(File,ios::in|ios::binary);
      while(file.peek()!=EOF)
      {
          file.read((char*)&student,sizeof(saccount));
          if(student.Account==num) 
          {   snumber--;
              flag=1;
              continue;
          }
          vt.push_back(student);
       }
       fstream file1(File,ios::out|ios::trunc);
       for(int i=0;i<vt.size();i++)
        {  
          file1.write((char*)&vt[i],sizeof(saccount)); }
       if(flag==1) {std::cout<<"删除成功"<<endl;
       }
       else {cout<<"查无此人,删除失败"<<endl;}
        file.close();
        file1.close();  
    }
    else 
    {vector<taccount> vt;
      taccount student;
      int flag1;
      int num;
      char str[20];
      std::cout<<"请输入要删除的教师的帐号"<<endl;
      std::cin>>num;
      char huiche=getchar();
      fstream file(File,ios::in|ios::binary);
      while(file.peek()!=EOF)
      {
          file.read((char*)&student,sizeof(taccount));
          if(student.Account==num) 
          {   tnumber--;
              flag=1;
              continue;
          }
          vt.push_back(student);
       }
       fstream file1(File,ios::out|ios::trunc);
       for(int i=0;i<vt.size();i++)
        {  
          file1.write((char*)&vt[i],sizeof(taccount)); }
         if(flag==1) {std::cout<<"删除成功"<<endl;}
       else {cout<<"查无此人,删除失败"<<endl;}
        file.close();
        file1.close(); }
}
int login(int acc,char* pss) 
//登录失败返回0，登陆成功返回1或2
{   
    fstream tfile("teacher.txt",ios::in|ios::binary);
    tfile.seekg(0,ios::beg);
    taccount tea;
    saccount sea;
    if(acc<100000&&acc>9999)
    {
      while(tfile.peek()!=EOF)
{    tfile.read((char*)&tea,sizeof(taccount));
    if(acc==tea.Account&&(!strcmp(pss,tea.Password)))
    { tfile.close();
      cout<<"尊敬的"<<tea.name<<"老师，欢迎登录学生成绩管理系统"<<endl;
      return 1;
      break;
    }
    }
     tfile.close();
      return 0;
 }
    else if(acc<=99999999&&acc>9999999)
    {       fstream sfile("student.txt",ios::in|ios::binary);
       sfile.seekg(0,ios::beg);
       while(sfile.peek()!=EOF)
    {sfile.read((char*)&sea,sizeof(saccount));
    if(acc==sea.Account&&(!strcmp(pss,sea.Password)))
    {  cout<<"亲爱的"<<sea.name<<"同学，欢迎登录学生信息门户"<<endl;
      tfile.close();
    sfile.close();

      return 2;
      break;
    }
    }
       tfile.close();
    sfile.close();
      return 0;
    }
    else {  
      return 0;}
 }
void fileloader() //加载文件
{    fstream xuantuikefile("flag.txt",ios::in|ios::binary);
    if(xuantuikefile.peek()!=EOF) { 
      xuantuikefile>>flag;
         }
    fstream adminfile("admin.txt",ios::in|ios::binary);
    if(adminfile.peek()!=EOF) { 
      adminfile>>adminpasseword;
         }
    cout<<adminpasseword<<endl;
    
    fstream file1("teacher.txt",ios::in|ios::binary);
    fstream file2("student.txt",ios::in|ios::binary);
    saccount s1;
    taccount t1;     
    while(file1.peek()!=EOF)
    {
    
    file1.read((char*)&t1,sizeof(taccount));
    tnumber++;
    }
    while(file2.peek()!=EOF)
    {
    file2.read((char*)&s1,sizeof(saccount));  
    snumber++;
    }
    xuantuikefile.close();
    adminfile.close();
    file1.close();
    file2.close();  }
int main()
{   fileloader(); //
    int n;
    char s[20] ;
    std::cout<<"学生选课及成绩管理系统V1.0"<<endl;
    std::cout<<"----------------------"<<endl;
while(1)
{ std::cout<<"请输入账号："<<endl;
  std::cin>>n;
  char p=getchar();  //读掉回车符
  std::cout<<"请输入密码："<<endl;
  std::cin>>s;             
  char p1=getchar();//读掉回车符
  if(flag==0&&n!=0) {   std::cout<<"系统信息未录入，请等待管理员完成信息录入"<<endl;  continue;}
  else if(n==0)
  {  
  if(s!=adminpasseword)
    {
  std::cout<<"登录失败，账号或密码错误"<<endl;
  std::cout<<"       请重新登录      "<<endl;
  continue;
  }
  else{
  std::cout<<"~~~管理员，登陆成功~~~"<<endl;
  if(flag==0){std::cout<<"提示：信息尚未完成录入"<<endl;}
  while(1)
  { 
  std::cout<<"对教师信息进行操作请按1"<<endl;
  std::cout<<"对学生信息进行操作请按2"<<endl;
  if(flag==0)
  {std::cout<<"开放学生选课系统请按3"<<endl;}
  else if(flag==1)
    {std::cout<<"开始学生退课系统请按3"<<endl;}
    else if(flag==2)
    {std::cout<<"开放教师成绩录入请按3"<<endl;}
    else if(flag==3)
    {  std::cout<<"开始成绩复议阶段请按3"<<endl;}
    else if(flag==4) 
    {  std::cout<<"清空系统信息，开始新学期请按#"<<endl; 
    }
  std::cout<<"修改管理员账号密码请按4"<<endl;
  std::cout<<"~~~~退出系统请按5~~~~"<<endl;
  char ch = getchar();
  char huiche=getchar();
//管理员负责录入学生的名字，账号，密码以及科目，教师的姓名，账号密码，授课科目。
  if(ch=='1') {  
   while(1)
 {operateteachermenu();
  char a;
  cin>>a;
  char huiche=getchar();
  if(a=='1') 
  { 
  std::cout<<"提示：设置的教师账号为五位数"<<endl;
  insert("teacher.txt",&tnumber);
  }
  else if(a=='2')
  {
    
  print("teacher.txt");
  }
  else if(a=='3')
  {
  save();
  }
  else if(a=='4')
  {
  std::cout<<"请输入读取到的目标文本文件名称(不含格式后缀)"<<endl;
  char newfile[20];
  std::cin>>newfile;
  char cdhjici=getchar();
  strcat(newfile,".txt");
  read(newfile,"teacher.txt");
  }
  else if(a=='5')
 {
  countnumber("teacher.txt"); 
 } 
  else if(a=='6')
{
search("teacher.txt");  
}
  else if(a=='7')
{
  fix("teacher.txt");
}  
  else if(a=='8')
{
del("teacher.txt");  
}
 else if(a=='0')
 { 
 std::cout<<"返回上一级成功"<<endl;
 break;  
 } 
  else 
  {   
  std::cout<<"暂无此功能，请重新选择"<<endl;}
 }
 }
  else if(ch=='2')  {
  while(1) 
  {operatestudentmenu();
  char a;
  std::cin>>a;
  char bc=getchar();
  if(a=='1') 
  { 
  insert("student.txt",&snumber);
  }
  else if(a=='2')
  {
  print("student.txt");
  }
  else if(a=='3')
  {
  save();
  }
  else if(a=='4')
  {
  std::cout<<"请输入读取到的目标文本文件名称(不含格式后缀)"<<endl;
  char newfile[20];
  std::cin>>newfile;
  read(newfile,"student.txt");
  }
  else if(a=='5')
 {
  countnumber("student.txt"); 
 } 
  else if(a=='6')
{
search("student.txt");  
}
  else if(a=='7')
{
  fix("student.txt");
}  
  else if(a=='8')
{
del("student.txt");  
}
 else if(a=='0')
 { 
 std::cout<<"返回上一级成功"<<endl;
 break;  
 } 
  else 
  {   
  std::cout<<"暂无此功能，请重新选择"<<endl;}    
      
      
      
  } }
  else if(ch=='3'&&flag<=3) {
    flag++;
  fstream adminfile1("flag.txt",ios::out);
          adminfile1.seekp(0,ios::beg);
          adminfile1<<flag;
          adminfile1.close();
  }
  else if(ch=='4') {
  std::cout<<"请输入新密码:"<<endl;
  string newadminpassword2;
  std::cin>>newadminpassword2;
   char p=getchar();
  if(newadminpassword2==adminpasseword) { std::cout<<"与原密码相同，修改失败"<<endl;} 
    else {
        std::cout<<"请再次输入新密码："<<endl;
        string newadminpassword3;
        std::cin>>newadminpassword3;
        char p2=getchar(); 
        if(newadminpassword2!=newadminpassword3) { std::cout<<"两次密码不一致，修改失败"<<endl;}
        else {
          std::cout<<"修改成功"<<endl;
          fstream adminfile1("admin.txt",ios::out);
          adminfile1.seekp(0,ios::beg);
          adminfile1<<newadminpassword2;
          adminpasseword=newadminpassword2;
          adminfile1.close();
        }
        }
     }
     else if(ch=='5')  {printf("退出登录成功\n");
     break;}
     else if(ch=='#') 
     {
       flag=0;
       fstream adminfile1("flag.txt",ios::out);
          adminfile1.seekp(0,ios::beg);
          adminfile1<<flag;
          adminfile1.close();
       while(1)
       {
         std::cout<<"清空教师名单请按1"<<endl;
         std::cout<<"清空学生名单请按2"<<endl;
         std::cout<<"直接退出不清空请按其他"<<endl;
         char  a=getchar();
         char huiche=getchar();
         if(a=='1')
         { 
           FILE *fp;
           fp=fopen("teacher.txt","w");
           fclose(fp);
         }
         else if(a=='2')
         {FILE *fp;
           fp=fopen("student.txt","w");
            fclose(fp);
         }
         else {
          break;
         }
        }
     }
    else {std::cout<<"暂无此功能"<<endl;}
  }
  }}
  //教师负责录入学生名单，学生名单储存在“教师账号”.txt中
   else if(flag!=0&&n!=0) 
  {    
    int flag1=login(n,s);
    if(flag1==1)//教师操作
        {  cout<<"欢迎登录选课及成绩管理系统"<<endl; 
           teacher t1(n,s,"aa","bb");
           taccount tea;
           fstream file3("teacher.txt",ios::in|ios::binary);
           while(!file3.peek()==EOF)
          {file3.read((char*)&tea,sizeof(taccount));
          if(tea.Account=n)  
              {        
             strcpy(t1.course,tea.course);
             strcpy(t1.name,tea.name);
             t1.credit=tea.credit;
             break;//教师对象实例化}
      }}
         while(1)
         {
           teachermenu(); 
            string num;
            cin>>num;
            char huiche=getchar();
           if(num=="1")
           { 
            t1.insert();
           }
           else if(num=="2")
           {
             t1.display();
           }
           else if(num=="3")
           { 
            std::cout<<"请输入读取到的目标文本文件名称(不含格式后缀)"<<endl;
            char newfile[20];
            std::cin>>newfile;
            char cdhjici=getchar();
            strcat(newfile,".txt");
            t1.read(newfile);
           }
           else if(num=="4")
           {
             t1.stat();
           }
           else if(num=="5")
           {
             t1.search();
           }
           else if(num=="6")
           {
             t1.sorts();
           }
          else if(num=="7")
           {
             t1.modify();
           }
          else if(num=="8")
           {
             t1.passwordmodify();
           }
           else if(num=="9")
           {
             t1.fuyi();
           }
          else if(num=="0")
           {
             break;
           }
           else {cout<<"暂无此功能"<<endl;
          }
         }

        }
       //学生操作
       //学生的个人信息不再另外存档，在登录时从各个老师名单里搜刮出来进行对象实例化 
        else if(flag1==2){ 
          cout<<"欢迎登录选课及成绩管理系统"<<endl;
         taccount tea;
         int i=0;
          student xiaohua(n,s);
          fstream sfile("student.txt",ios::in|ios::binary);
          while(sfile.peek()!=EOF)  //小华的名字
          {
            saccount sea;
            sfile.read((char*)&sea,sizeof(saccount));
             if(sea.Account==n)
             {strcpy(xiaohua.name,sea.name);
             break;}
          }
          sfile.close();
           fstream file1("teacher.txt",ios::in|ios::binary);
           while(file1.peek()!=EOF)
           {  file1.read((char*)&tea,sizeof(taccount));
              char str[10];
          sprintf(str,"%d",tea.Account);
          strcat(str,".txt");
          fstream file2(str,ios::in|ios::out); //打开教师的课程学生文件
          file2.seekg(0,ios::beg);
          saccount sea;
          while(file2.peek()!=EOF)
          {     file2.read((char*)&sea,sizeof(saccount));
                if(sea.Account==n) { 
            xiaohua.gpa[i].Grade=sea.Grade;
            xiaohua.gpa[i].tnum=tea.Account;
            xiaohua.gpa[i].credit=tea.credit;
            strcpy(xiaohua.gpa[i].GPA,sea.GPA);
            strcpy(xiaohua.gpa[i].course,tea.course);
            i++;
            file2.close();
            break; 
                }
             }
           }
           file1.close();
       
  //学生操作界面菜单
    while(1)
    { studentmenu(); 
      string num;
      cin>>num;
      if(num=="1")
           { 
            xiaohua.print();
           }
          else if(num=="2")
           {
             xiaohua.xuanke();
           }
          else if(num=="3")
           {
             xiaohua.tuike();
           }
          else if(num=="4")
           {
            xiaohua.fuyi();
           }
           else if(num=="5")
           {
             xiaohua.passwordmodify();
           }
           else if(num=="6")
           {cout<<"退出成功"<<endl;
            break;}
            else {cout<<"暂无此功能"<<endl;}
    }
        
         } 
      else  { 
         std::cout<<"登录失败"<<endl;
         std::cout<<"请重新登录"<<endl; 
          continue;} //登录失败
     
  }
}
}
