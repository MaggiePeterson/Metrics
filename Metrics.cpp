//
//  Metrics.cpp
//  Metrics
//
//  Created by Margaret Peterson on 4/19/19.
//  Copyright © 2019 Margaret Peterson. All rights reserved.
//

#include "Metrics.hpp"

struct x_target {
   int start =0;
   int end =0;
};

struct y_target {
   int start =0;
   int end =0;
};

struct target {
   y_target y;
   x_target x;
};

//constructor
Metrics::Metrics(int frameWidth, int degree){
   this->frameW = frameWidth;
   this->FOV = degree;
   vector<float>radiusX = {0};
   vector<float>distY = {0};
}

Mat Metrics:: calculate(Mat *img){

   int haswhite = 0;
   int prevhaswhite = 0;
   vector<x_target> xt;
   vector<y_target> yt;
   struct x_target x;
   struct y_target y;
   vector<target> targets;
   target vision_target;


   //vertical
   for(int i =0; i < img->cols; i++){

      haswhite =0;
      for(int j =0; j< img->rows; j++){ //each column

         if(img->data[j* img->cols *img->channels() + i*img->channels()] == 255){
            haswhite++;
         }
      }

      if((haswhite>=15) && !prevhaswhite){   //if current is white but prev is black
         line(*img, Point(i, 0), Point(i,480), Scalar(250,250,250),1, LINE_8, 0); //draw line
         prevhaswhite =1;
         x.start = i;
      }
      else if ((haswhite<=15) && prevhaswhite){
         line(*img, Point(i, 0), Point(i,480), Scalar(250,250,250),1, LINE_8, 0); //draw line
         prevhaswhite =0;

         x.end = i;
         xt.push_back(x);

      }
   }

   prevhaswhite = 0;

   //horizontal
   for(int j =0; j< img->rows; j++){
      haswhite =0;

      for(int i =0; i < img->cols; i++){

         if(img->data[j* img->cols *img->channels() + i*img->channels()] == 255){  //if at least one is white
            haswhite++;
         }
      }

      if((haswhite>=5) && !prevhaswhite){   //if current is white but prev is black
         line(*img, Point(0, j), Point(640,j), Scalar(250,250,250),1, LINE_8, 0); //draw line
         prevhaswhite =1;

         y.start = j;

      }
      else if ((haswhite<=5) && prevhaswhite){
         line(*img, Point(0, j), Point(640,j), Scalar(250,250,250),1, LINE_8, 0); //draw line
         prevhaswhite =0;

         y.end = j;
         yt.push_back(y);
         yt.push_back(y);
      }
   }

   for(int i = 0; i<xt.size(); i++){
      cout<<i+1 <<" x target: "<<xt[i].end<<" "<<xt[i].start<<endl;
      cout<<i+1 <<" y target: "<<yt[i].end<<" "<<yt[i].start<<endl; //only 1

      vision_target.x = xt[i];
      vision_target.y = yt[i];

      cout<<i+1 <<" x Vtarget: "<<vision_target.x.end<<" "<<vision_target.x.start<<endl;
      cout<<i+1 <<" y Vtarget: "<<vision_target.y.end<<" "<<vision_target.y.start<<endl; //only 1

   }


   return *img;
}

void Metrics:: calibrateZero(Mat *img, float dist){
   cout<<"METRICS: Calibrating to Zero; dist: "<<dist<<endl;
   //  drawBoundingBox(img);
   this->radiusX.push_back(radius1);
   this->radiusX.push_back(radius2);
   this->distY.push_back(0);
   this->distY.push_back(0);

   this->zeroDist = dist;
}

void Metrics::configValues(Mat *img, float dist){
   cout<<"METRICS: Config; dist: "<<dist<<endl;
   //   drawBoundingBox(img);
   this->radiusX.push_back(radius1);
   this->radiusX.push_back(radius2);
   this->distY.push_back(dist);
   this->distY.push_back(dist);
}

float Metrics:: lineOfRegression(float rad1, float rad2){

   if(this->radiusX.size() != this->distY.size()){
      cerr<< "ERROR: Vectors are not the same size"<<endl;
   }
   float n = this->radiusX.size();

   float avgX = accumulate(this->radiusX.begin(), this->radiusX.end(), 0.0) / n;
   float avgY = accumulate(this->distY.begin(), this->distY.end(), 0.0) / n;

   float numerator = 0.0;
   float denominator = 0.0;

   for(int i=0; i<n; ++i){
      numerator += (this->radiusX[i] - avgX) * (this->radiusX[i] - avgY);
      denominator += (this->radiusX[i] - avgX) * (this->radiusX[i] - avgX);
   }

   if(denominator==0){
      cerr << "ERROR: Undefined slope"<<endl;
   }

   float radius = (rad1+rad2)/2; //average radius
   float slope = numerator / denominator;
   float dist = slope*radius + this->zeroDist;
   return dist;
}

float Metrics::distance(){

   return lineOfRegression(radius1, radius2);
}

int Metrics:: angle(){
   return this->angl;
}

void Metrics:: writeMetrics(string textFile){

   ofstream myFile;
   myFile.open(textFile);

   myFile << radiusX.size() <<endl;
   for(int i =0; i< this->radiusX.size(); i++){
      myFile<< this->radiusX[i] <<endl<<  this->distY[i] << endl;
   }
   myFile.close();

}

bool Metrics:: readMetrics(string textFile){

   ifstream inFile;
   inFile.open(textFile);
   int size;
   if(inFile>> size){
      radiusX.resize(size);
      distY.resize(size);
      for(int i=0; i< size; i++){
         inFile >> this->radiusX[i] >> this->distY[i];
      }
      return true;
   }
   return false;
}



