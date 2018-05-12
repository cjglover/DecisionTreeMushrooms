#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;
class tree {
public:
  tree() {
isleaf=false;
isroot=false;
fid=-2;
p=0;
n=0;
par=NULL;
parval= 1;

  }
bool isleaf;
bool isroot;
int fid;
vector<string> path;
vector<tree* > kids;
tree* par;
bool parval;
int p;
int n;

};

pair<int, int> getpn(vector<string>& path, const vector<pair<string, int> > & mExamples) {
  int siz=path.size();
  int p=0;
  int n=0;
  for(int i=0;i<mExamples.size();++i) {
    bool isval=true;
    for(int j=0;j<siz;++j) {
      if((path[j][0]!='-')&&(path[j][0]!=mExamples[i].first[j])) {
        isval=false;
        break;
      }

    }
    if(isval) {
      if(mExamples[i].second==1)
        p++;
      else
      n++;

    }
  }
  return make_pair (p,n);


}

float getfloat(float p, float n) {
  float f=0;
  if(p!=0)  {

      f+= p/(n+p)*log2(p/(n+p));
  }
  if(n!=0) {

    f+= n/(n+p)* log2(n/(n+p));

  }

  return -f;




}

float getentropy(tree* mytree){
  vector<float> myvec;
  myvec.resize(mytree->kids.size());
  float sum=0;
  for(int i=0;i<mytree->kids.size();++i) {
    tree* kid= mytree->kids[i];
    sum+=kid->p;
    sum+=kid->n;
  }

  for(int i=0;i<mytree->kids.size();++i) {
      tree* kid= mytree->kids[i];
      if(kid->p==0||kid->n==0) {
        kid->isleaf=true;

      }
      myvec[i]= (kid->p+kid->n)/sum*getfloat(kid->p, kid->n);


  }
  sum=0;
  for(int i=0;i<myvec.size();++i) {
    sum+=myvec[i];


  }
return sum;

}


void maketree(tree* mytree, const vector<string>& mFeatures, const vector< vector<string> > & mAttributes, int currlevel, int maxlevel, const vector<pair<string, int> > & mExamples, bool depthlim)
{
  if(mytree->isleaf){
    return;
  }

  /*if(currlevel==maxlevel) {
    mytree->isleaf=true;

    int siz=mFeatures.size()-1;
    for(int i=0;i<mExamples.size(),++i) {
      bool isval=true;
      for(int j=0;j<siz;++j) {
        if((mytree->path[j].length()!=0)&&(mytree->path[j][0]!=mExamples[i].first[j])) {
          isval=false;
          break;
        }

      }
      if(isval) {
        if(mExamples[i].second==1)
          ++(mytree->p);
        else
          ++(mytree->n);

      }

    }
    return;
  }*/

  vector<float> entropyval;
  entropyval.resize(mFeatures.size()-1);
  for(int i=0;i<entropyval.size();++i) {

    entropyval[i]= 5.0f;
  }

  vector<tree*> candid;
//cout<<mytree->path.size();
  for(int i=0;i<mytree->path.size();++i) {
    if(mytree->path[i][0]!='-')
    continue;
    tree* tr= new tree();
    tr->fid=i;
    tr->path=mytree->path;
    tr->p= mytree->p;
    tr->n= mytree->n;

    candid.push_back(tr);
    vector<string> atts= mAttributes[tr->fid+1];
    for(int h=0;h<atts.size();++h) {
      tree * child= new tree();
      tr->kids.push_back(child);
      child->path= tr->path;
      child->path[i]=atts[h];
      pair<int, int> mypair= getpn(child->path,mExamples);
      child->p=mypair.first;
      child->n = mypair.second;
      if(depthlim&&currlevel+1>=maxlevel)
      child->isleaf=true;
      if(child->p==0||child->n==0) {
        child->isleaf=true;
        if(child->n==0&&child->p==0) {
          child->parval= ((tr->p)>=(tr->n));
        }
      }

    }
    float val= getentropy(tr);
    entropyval[tr->fid]=val;





  }
  if(candid.empty()) {

    mytree->isleaf=true;
    return;
  }
  int minfid= candid[0]->fid;
  int ind=0;
  float minent= entropyval[candid[0]->fid];
  for(int i=0;i<candid.size();++i) {
    if(entropyval[candid[i]->fid]<minent) {
      minent=entropyval[candid[i]->fid];
      minfid=candid[i]->fid;
      ind=i;
    }


  }
  mytree->fid=minfid;
  mytree->kids=candid[ind]->kids;
  for(ind=0;ind<candid.size();++ind){
  delete candid[ind];}

  for(int i=0;i<mytree->kids.size();++i) {
    maketree(mytree->kids[i],  mFeatures,  mAttributes, currlevel+1, maxlevel, mExamples, depthlim);

  }







return;



}

void print(tree* mytree, int level=0) {
  cout<<"||level "<<level<<" id: "<<mytree->fid<<" p: "<<mytree->p<<" n: "<<mytree->n<<" path: " ;
  for(int i=0;i<mytree->path.size();++i) {
    if(mytree->path[i].length()==0)
    cout<<"-";
    else
    cout<<mytree->path[i];

  }
  cout<<" islef: "<<mytree->isleaf<<endl;
  if(mytree->isleaf)
  return;
  else{
    for(int i=0;i<mytree->kids.size();++i)
    print(mytree->kids[i], level+1);


  }


}
int randomint(){

    return(rand() % 5);
}
int randomint2(){

    return(rand() % 4);
}

int main (int argc , char** argv) {


    if(argc<2) {
        cout<<"Please include filename";

        return 0;
    }

    ifstream infile;
    infile.open(argv[1]);
    if(infile.fail()) {
    cout<<"Unable to open file";

        return 0;
    }
    bool depthlim=0;
    int mexdepth=5;
    srand(time(0));
    string fline;
    std::getline(infile, fline);
    vector<string> mFeatures;
    {
      string delim=",";
      size_t pos=0;
      string token;
      while((pos=fline.find(delim))!=string::npos) {
        token=fline.substr(0, pos);
        mFeatures.push_back(token);
        fline.erase(0, pos+delim.length());

      }
    }
    mFeatures.push_back(fline);
    vector< vector<string> > mAttributes;
    mAttributes.resize(mFeatures.size());
    for(int m=0;m<mAttributes.size();++m){
      mAttributes[m].resize(0);


    }
    string currline;
    vector< pair<string, int > > mExamples;
    int g=0;
    while(std::getline(infile, currline)) {
      ++g;
      string examplekey="";
      string delim=",";
      size_t pos=0;
      string token;
    unsigned  int i=0;
      while( ((pos=currline.find(delim))!=string::npos) && (i<mFeatures.size()) ) {

        token=currline.substr(0, pos);
        examplekey+=token;
        bool ispres=false;

        for(int j=0;j<mAttributes[i].size();++j) {

          if(mAttributes[i][j][0]==token[0])
            ispres=true;

        }

        if(!ispres)
          mAttributes[i].push_back(token);

        currline.erase(0, pos+delim.length());
        ++i;
      }

      bool isp=false;
      for(int j=0;j<mAttributes[i].size();++j) {
        if(mAttributes[i][j][0]==currline[0])
          isp=true;
      }

      if(!isp)
        mAttributes[i].push_back(currline);

      examplekey+=currline;
      int ispos= examplekey[0]-'0';
      examplekey.erase(0,1);
      mExamples.push_back(make_pair(examplekey, ispos));




    }



  /*  for(int i=0;i<mAttributes.size();++i) {
      for(int j=0;j<mAttributes[i].size();++j) {
        cout<<mAttributes[i][j]<<" ";
      }
      cout<<endl;

    }
    for(int i=0;i<mExamples.size();++i){
    cout<<mExamples[i].first<<" "<<mExamples[i].second<<endl;
  }*/
  //cout<<mExamples.size()<<endl;
  vector<pair<string, int> > trainingset;
  vector<pair<string, int> > testingset;


  for(int i=0;i<mExamples.size();i+=5) {
    int test= randomint();


    for(int j=0;(j<5)&&((i+j)<mExamples.size());++j) {
      if(j==test){
      testingset.push_back(mExamples[i+j]);
      }
      else{
      trainingset.push_back(mExamples[i+j]);
      }
    }


  }
  vector<pair<string, int> > trainingset2;
  vector<pair<string, int> > validset2;

  for(int i=0;i<trainingset.size();i+=4 ) {
    int test= randomint2();
    for(int j=0;(j<4)&&((i+j)<trainingset.size());++j) {
      if(j==test){
    validset2.push_back(trainingset[i+j]);
      }
      else{
      trainingset2.push_back(trainingset[i+j]);
      }
    }


  }
//  cout<<testingset.size()<<" "<<trainingset.size()<<" "<<mExamples.size()<<endl;
//  cout<<trainingset2.size()<<" "<<validset2.size()<<endl;
    {
  tree* root= new tree();
  root->kids.push_back(new tree());
  tree* start=root->kids[0];
  vector<string> sPath;
  for(int i=0;i<mFeatures.size()-1; ++i){
    sPath.push_back("-");
  }
  start->path=sPath;
  pair<int, int> mypair=getpn(sPath, trainingset);
  start->p= mypair.first;
  start->n= mypair.second;
  if(start->p==0||start->n==0) {
    start->isleaf=true;
  }
  if(depthlim&&mexdepth<=1) {
    start->isleaf=true;
  }
  //cout<<start->isleaf<<endl;
 maketree(start, mFeatures, mAttributes, 1, mexdepth,  trainingset, depthlim );
  int correct=0;
  for(int i=0;i<testingset.size();++i) {

    tree* curr= start;
    while(!curr->isleaf) {
      int fid=curr->fid;
      char mchar= testingset[i].first[fid];

      int ind=-1;
      for(int j=0;j<mAttributes[fid+1].size();++j)
      {
        if(mAttributes[fid+1][j][0]==mchar){
            ind=j;
            break;
        }


      }
      if(ind==-1){
        cerr << "ERROR" << '\n';
        return 0;
      }
      curr=curr->kids[ind];


    }
    if(((curr->p)>=(curr->n))==(testingset[i].second==1)) {
      ++correct;
    }
  }
  cout<<"For decision tree of unbounded depth, the accuracy is: ";
  cout<<correct*(1.0)/testingset.size()<<endl;
}

vector< float> resultsval;
vector< float> resultstran;

depthlim=1;

  for(int d=1;d<=15;++d)  {
    mexdepth=d;
    tree* root= new tree();
    root->kids.push_back(new tree());
    tree* start=root->kids[0];
    vector<string> sPath;
    for(int i=0;i<mFeatures.size()-1; ++i){
      sPath.push_back("-");
    }
    start->path=sPath;
    pair<int, int> mypair=getpn(sPath, trainingset2);
    start->p= mypair.first;
    start->n= mypair.second;
    if(start->p==0||start->n==0) {
      start->isleaf=true;
    }
    if(depthlim&&mexdepth<=1) {
      start->isleaf=true;
    }
    //cout<<start->isleaf<<endl;
   maketree(start, mFeatures, mAttributes, 1, mexdepth,  trainingset2, depthlim );
   {
     int correct=0;
     for(int i=0;i<validset2.size();++i) {

       tree* curr= start;
       while(!curr->isleaf) {
         int fid=curr->fid;
         char mchar= validset2[i].first[fid];

        int ind=-1;
        for(int j=0;j<mAttributes[fid+1].size();++j)
        {
          if(mAttributes[fid+1][j][0]==mchar){
              ind=j;
              break;
          }


        }
        if(ind==-1){
          cerr << "ERROR" << '\n';
          return 0;
        }
        curr=curr->kids[ind];


      }
      if(((curr->p)>=(curr->n))==(validset2[i].second==1)) {
        ++correct;
      }
    }
    resultsval.push_back(correct*(1.0)/validset2.size());
  }
  {
    int correct=0;
    for(int i=0;i<trainingset2.size();++i) {

      tree* curr= start;
      while(!curr->isleaf) {
        int fid=curr->fid;
        char mchar= trainingset2[i].first[fid];

       int ind=-1;
       for(int j=0;j<mAttributes[fid+1].size();++j)
       {
         if(mAttributes[fid+1][j][0]==mchar){
             ind=j;
             break;
         }


       }
       if(ind==-1){
         cerr << "ERROR" << '\n';
         return 0;
       }
       curr=curr->kids[ind];


     }
     if(((curr->p)>=(curr->n))==(trainingset2[i].second==1)) {
       ++correct;
     }
   }
   resultstran.push_back(correct*(1.0)/trainingset2.size());



  }


  }
  cout<<"depth  train%  valid%\n";
for(int i=0;i<15;++i) {
cout<<i+1<<"     "<<resultstran[i]<<"   "<<resultsval[i]<<endl;

}
float max=0;
int index=0;
for(int i=0;i<15;++i) {
  if(resultsval[i]>max) {
      max= resultsval[i];
      index=i;

  }

}
cout<<endl;
cout<<"The depth with the highest accuracy is depth: "<<index+1<<" with accuracy of "<<max<<endl;
cout<<endl;
{
  depthlim=1;
  mexdepth=index+1;
tree* root= new tree();
root->kids.push_back(new tree());
tree* start=root->kids[0];
vector<string> sPath;
for(int i=0;i<mFeatures.size()-1; ++i){
  sPath.push_back("-");
}
start->path=sPath;
pair<int, int> mypair=getpn(sPath, trainingset);
start->p= mypair.first;
start->n= mypair.second;
if(start->p==0||start->n==0) {
  start->isleaf=true;
}
if(depthlim&&mexdepth<=1) {
  start->isleaf=true;
}
//cout<<start->isleaf<<endl;
maketree(start, mFeatures, mAttributes, 1, mexdepth,  trainingset, depthlim );
int correct=0;
for(int i=0;i<testingset.size();++i) {

  tree* curr= start;
  while(!curr->isleaf) {
    int fid=curr->fid;
    char mchar= testingset[i].first[fid];

    int ind=-1;
    for(int j=0;j<mAttributes[fid+1].size();++j)
    {
      if(mAttributes[fid+1][j][0]==mchar){
          ind=j;
          break;
      }


    }
    if(ind==-1){
      cerr << "ERROR" << '\n';
      return 0;
    }
    curr=curr->kids[ind];


  }
  if(((curr->p)>=(curr->n))==(testingset[i].second==1)) {
    ++correct;
  }
}
cout<<"Measured against the testing set, at this depth, the accuracy is: ";
cout<<correct*(1.0)/testingset.size()<<endl;
}



  //print(start);
  //cout<<(start->p)<<" "<<(start->n)<<endl;

//cout<<getfloat(start->p, start->n);


    return 0;



}
