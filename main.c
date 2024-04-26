#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define FALSE 0
#define TRUE  1

#define MAX_DATABLK_SIZE		100
#define RND_VALUE_UPPER			50
#define MAX_WINDOW_DIVISOR		10
#define MIN_WINDOW              3
#define SIN_DIVISOR             4.0     /* this reduces the cycles per data blk */


int LiveDataBuffer[MAX_DATABLK_SIZE];
int SmoothDataBuffer[MAX_DATABLK_SIZE];


bool GenerateLiveData()
{
    int DataCount;
    
    for (DataCount = 0; DataCount < MAX_DATABLK_SIZE; DataCount++)
    {
        LiveDataBuffer[DataCount] = (int)((rand() / (RAND_MAX / RND_VALUE_UPPER)) * sin((double)(DataCount/ SIN_DIVISOR)));
    }
    
    return TRUE;
}
 
bool BoxcarSmoothData(int *pWindowBuffer, int *fsum, int Window)
{
    register int DataIndex, WindowIndex;

    WindowIndex = 0;
    DataIndex = 0;

    for (DataIndex = 0; DataIndex < MAX_DATABLK_SIZE; DataIndex++)
    {
        *fsum = *fsum - pWindowBuffer[WindowIndex] + LiveDataBuffer[DataIndex];     /* update sum */
        pWindowBuffer[WindowIndex] = LiveDataBuffer[DataIndex];
        SmoothDataBuffer[DataIndex] = *fsum / Window;
        WindowIndex++;
        if (WindowIndex >= Window)                      /* reset window buffer index back to start */
            WindowIndex = 0;
    }
    
    return TRUE;
}

main(int argc, char** argv)
{
    int DataCount,Window, MaxWindow;
    int* pWindowBuffer = NULL;            
    int fsum = 0;
 

    MaxWindow = MAX_DATABLK_SIZE / MAX_WINDOW_DIVISOR;
    Window = 3;

    if (argc > 1)
        Window = (atoi(argv[1]) > MaxWindow ? MaxWindow : atoi(argv[1]));

    /* pWindowBuffer must be zero intialised */
    if ((pWindowBuffer = calloc(1, Window * sizeof(int))) == NULL)
        exit(2);

    srand(1);

    if (!GenerateLiveData())
        exit(3);

    printf("Window=%d Cycles=%d\n", Window, (int)(MAX_DATABLK_SIZE / (6.2 * SIN_DIVISOR)));

       
    for (DataCount = 0; DataCount < MAX_DATABLK_SIZE; DataCount++)
    {
        printf("%d ", LiveDataBuffer[DataCount]);
    }
   
    if (!BoxcarSmoothData(pWindowBuffer, &fsum, Window))
        exit(4);

    printf("\n\n");

    for (DataCount = 0; DataCount < MAX_DATABLK_SIZE; DataCount++)
    {	
        printf("%d ", SmoothDataBuffer[DataCount]);
    }

}