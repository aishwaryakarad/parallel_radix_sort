#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

#include "sort.h"
#include "edgelist.h"
// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)

void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges){

    
    int i;
    int key;
    int pos;
    int expo;

    // auxiliary arrays, allocated at the start up of the program
   int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for Counting Sort

    
    for(i = 0; i < numVertices; ++i) {
        vertex_cnt[i] = 0;
    }

    // count occurrence of key: id of a source vertex
       for(i = 0; i < numEdges; ++i) {
        key = ((edges[i].src)/expo)%10;
        
        vertex_cnt[key]++;
    }
     
    // transform to cumulative sum
    for(i = 1; i < numVertices; ++i) {
        vertex_cnt[i] += vertex_cnt[i - 1];
    }

    // fill-in the sorted array of edges
    
    for(i = numEdges - 1; i >= 0; --i) {
        key = ((edges[i].src)/expo)%10;
        pos = vertex_cnt[key] - 1;
        edges_sorted[pos] = edges[i];
        vertex_cnt[key]--;
    }


   // free(vertex_cnt);

}

   int getMax(struct Edge *edges, int numEdges) {
	int max=0;
        int i;
	for( i=0; i<numEdges;i++){

		if(edges[i].src>max)
			max=edges[i].src;
	
	}
	printf("%d",max);
	return max;
}


 void radixSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
	int max= getMax(edges, numEdges);
	int expo;
	
	int vertex_cnt[8][10];		//no of rows=1,2,4,8,16
	int threadcnt;

	for ( expo= 1; max/expo > 0; expo *= 10)
	{ 
 		printf("exp:%d\n",expo);
		
    // auxiliary arrays, allocated at the start up of the program
    		//int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for Counting Sort

	    omp_set_num_threads(8);	//1,2,4,8,16
		#pragma omp parallel		
        { 
			int i,j;
			
   			int key, pos, tid; 
 			tid=omp_get_thread_num();
			int tid_start, tid_end;
			threadcnt= omp_get_num_threads();
			tid_start= ((int)(numEdges/threadcnt) *tid) ;
			if(tid< threadcnt-1)
				tid_end=tid_start +(int)(numEdges/threadcnt)-1;
			else if(tid == threadcnt-1)
				tid_end=numEdges-1;			
    		for(i = 0; i <=9; i++) 
			{
		    	vertex_cnt[tid][i] = 0;
	    	}
			printf("the number of threads are:\t%d\t\n",threadcnt);

    // count occurrence of key: id of a source vertex
   
       		for(j = tid_start; j<= tid_end ;j++)
			{
        		key = ((edges[j].src)/expo)%10;
            	vertex_cnt[tid][key]=vertex_cnt[tid][key]+1;
    		}

			//#pragma omp barrier 
		} 
			int base, i, j;
			base=0;
    // transform to cumulative sum
      		for(i = 0; i < 10; i++)
			{
				for(j=0; j<8;j++) //j<1,2,4,8,16
				{
					base= base + vertex_cnt[j][i];
		        		vertex_cnt[j][i] =base;
    			}
			}

			    // fill-in the sorted array of edges
			
	        omp_set_num_threads(8);			//1,2,4,8,16
   			#pragma omp parallel
			{
			int i,j, tid;
            printf("Here\n");
			threadcnt=omp_get_num_threads();
			tid= omp_get_thread_num();
			int base;
   			int key, pos;
			printf("%d threadcount",threadcnt);
			int tid_start, tid_end;

			tid_start= ((int)(numEdges/threadcnt) *tid) ;
			if(tid< threadcnt-1)
				tid_end=tid_start +(int)(numEdges/threadcnt)-1;
			else if(tid == threadcnt-1)
				tid_end=numEdges-1;
    			for(i = tid_end; i >= tid_start ; i--)
		 	    {
        			key = ((edges[i].src)/expo)%10;
        			pos = vertex_cnt[tid][key] - 1;
                    if(pos < 0) printf("wtf\n");
		       		edges_sorted[pos] = edges[i];
        			vertex_cnt[tid][key]=vertex_cnt[tid][key]-1;
    			}
			
			}
			
            #pragma omp parallel for
			for(i=0;i<numEdges;i++)
			{
				edges[i]=edges_sorted[i];
			}
   		}
	}


//SERIAL CODE
void serialradixSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) 
{
	int max= getMax(edges, numEdges);
	
		int i;
    int key;
    int pos;
    int expo;

    // auxiliary arrays, allocated at the start up of the program
   //int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for Counting Sort
	int vertex_cnt[10];
    
	
	for(expo=1; max/expo>0; expo*=10)
{
	
    for(i = 0; i < 10; ++i) {
        vertex_cnt[i] = 0;
    }

    // count occurrence of key: id of a source vertex
       for(i = 0; i < numEdges; ++i) {
        key = ((edges[i].src)/expo)%10;
        
        vertex_cnt[key]++;
    }
     
    // transform to cumulative sum
    for(i = 1; i < numVertices; ++i) {
        vertex_cnt[i] += vertex_cnt[i - 1];
    }

    // fill-in the sorted array of edges
    
    for(i = numEdges - 1; i >= 0; --i) {
        key = ((edges[i].src)/expo)%10;
        pos = vertex_cnt[key] - 1;
        edges_sorted[pos] = edges[i];
        vertex_cnt[key]--;
    }
	for(i=0;i<numEdges;i++)
	{
	edges[i]=edges_sorted[i];
	}
}
}


   // free(vertex_cnt);



