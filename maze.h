#ifndef MAZE_H
#define MAZE_H

#include <iostream>
using namespace std;

class Maze{
private:
  int dimension;
  bool** matrix;
public:
  Maze(int dim){
    dimension = dim;
    matrix = new bool*[dimension];
    for (int i = 0; i < dimension; i++){
      matrix[i] = new bool[dimension];
    }
  }
  
  ~Maze(){
    for (int i = 0; i < dimension; i++){
      delete[] matrix[i];
    }
    
    delete[] matrix;
   }
  
  void setAt(int i, int j, bool b){
    matrix[i][j] = b;
  }
  
  bool getAt(int i, int j){
    return matrix[i][j];
  }
};

#endif