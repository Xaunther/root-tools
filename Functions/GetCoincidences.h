#ifndef GetCoincidences_h
#define GetCoincidences_h

//Template function
template <class numtype>
int GetCoincidences(numtype* arr1, int N1, numtype* arr2, int N2)
{
  int bingo = 0;
  for(int i=0;i<N1;i++)
    {
      for(int j=0;j<N2;j++)
	{
	  if(arr1[i]==arr2[j])
	    {
	      bingo++;
	    }
	}
    }
  return bingo;
}

#endif
