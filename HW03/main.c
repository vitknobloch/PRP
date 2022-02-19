#include <stdio.h>
#include <stdlib.h>

//functions declaration
void draw_with_fence(int width, int height, int fence);
void draw_no_fence(int width, int height);
void draw_roof(int width);

/* The main program */
int main(int argc, char *argv[])
{
  //initialize properties
  int width;
  int height;
  int fence = -1;
  
  //catch error input
  if(scanf("%d %d", &width, &height) != 2){
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  //check for interval
  if(width < 3 || width > 69 || height < 3 || height > 69){
    fprintf(stderr, "Error: Vstup mimo interval!\n");
    return 101;
  }
  //check if width is odd
  if(width % 2 == 0){
    fprintf(stderr, "Error: Sirka neni liche cislo!\n");
    return 102;
  }

  //load fence size if witdth = height
  if(width == height){
    //check for valid input
    if(scanf("%d", &fence) != 1){
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    //check for interval
    if(fence < 1 || fence >= height){
      fprintf(stderr, "Error: Neplatna velikost plotu!\n");
      return 103;
    }

    draw_with_fence(width, height, fence);
    return 0;
  }
  draw_no_fence(width, height);
  return 0;
}

void draw_no_fence(int width, int height){
  //draw roof
  draw_roof(width);

  for(int h = 0; h<height; h++){
    for(int w = 0; w<width; w++){
      if(h==0 || h==height-1 || w==0 || w==width-1){
        printf("X");
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
  
}

void draw_roof(int width){
  for(int i = 0; i<width/2; i++){
    for(int x = 0; x<=width/2+i; x++){
      if(x == width/2 - i){
        printf("X");
      }
      else if(x == width/2+i){
        printf("X");
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
}

void draw_with_fence(int width, int height, int fence){
  //draw roof
  draw_roof(width);

  //draw house
  for(int h = 0; h<height; h++){
    for(int w = 0; w<width; w++){
      if(h==0 || h==height-1 || w==0 || w==width-1){//Draw full line at the top and bottom and left and right
        printf("X");
      }
      else{
        if((h+w)%2==0){ //fill the house with 'o' and '*'
          printf("o");
        }
        else{
          printf("*");
        }
      }
    }
    //draw fence if the row contains some
    if(h >= height - fence){ 
      for(int f=0; f<fence; f++){ //draw f chars per line

        //start fence with ' ','-' or '|' based on it's widht (so that it ends with '|')
        if(fence%2 == 0){
          //start with ' ','-'
          if(f%2==0){
            if(h == height - 1 || h == height - fence){ //draw '-' on first and last line of fence
              printf("-");
            }
            else{ //draw ' ' on the rest of the lines
              printf(" ");
            }
          }
          //continue with '|'
          else{
            printf("|");
          }
        }

        //start fence with '-' or '|' based on it's widht (so that it ends with '|')
        else{
          //start with '|'
          if(f%2==0){
            printf("|");
          }
          //continue with ' ', '-'
          else{
            if(h == height - 1 || h == height - fence){//draw '-' on first and last line of fence
              printf("-");
            }
            else{//draw ' ' on the rest of the lines
              printf(" ");
            }
          }
        }
      }
    }
    printf("\n");
  }
  
}

