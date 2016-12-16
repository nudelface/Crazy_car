/*
 * interp.c
 *
 *  Created on: 16.12.2016
 *      Author: Sebo
 */



int Values_left[21]={};




float interp(int Arraybit[], int Arrayval[], int bitval,int length)
{
int bitmin=interpsearchindex(bitval, Arraybit, length);
float value=(float)Arrayval[bitmin]+ (  (float)(Arrayval[bitmin+1]-Arrayval[bitmin])  /  (float)(Arraybit[bitmin+1]-Arraybit[bitmin])  )  *  (float)(bitval-Arraybit[bitmin]);
return value;

}


int interpsearchindex(int interp1, int Array[], int length)
{

 int left=0;
 int right=length-1;
 int found=0;
 int x;
 int count=0;


 while(found==0)
 {

   x =(int)((float)(interp1-Array[left])/(float)(Array[right]-Array[left])*(float)(right-left)+(float)left);

   if((left<=right)&&(count<=10))
    {
      if(x==left)
       {
         count++;
         right--;
         if(Array[x+1]==interp1)
          {
            found=1;
            return x+1;
          }
        }

       if (Array[x]>interp1)
        right=x;
       else if(Array[x]<interp1)
        left=x;

       if(right-left<=1)
       {
        found=1;
        return x;
       }

    }
    else
        return 0;
 }
}