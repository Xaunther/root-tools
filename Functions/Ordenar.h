#ifndef Ordenar_h
#define Ordenar_h

//Template function
template <class numtype>
numtype* Ordenar(int N , numtype* array)
{
  numtype supp;
  for(int i=0;i<N;i++)
    {
      for(int j=i;j<N;j++)
	{
	  if(array[j] < array[i])
	    {
	      supp = array[i];
	      array[i] = array[j];
	      array[j] = supp;
	    }
	}
    }
  return array;
}
//Order 2-D array
template <class numtype>
numtype** Ordenar(int N1, int N2, int Norder, numtype** array)
{
  numtype supp;
  for(int i=0;i<N1;i++)
    {
      for(int j=i;j<N1;j++)
	{
	  if(array[j][Norder] < array[i][Norder])
	    {
	      for(int k=0;k<N2;k++)
		{
		  supp = array[i][k];
		  array[i][k] = array[j][k];
		  array[j][k] = supp;
		}
	    }
	}
    }
  return array;
}


#endif
