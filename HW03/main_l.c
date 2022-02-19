#include <stdio.h>

int main()
{
    int width_bad = 0;
    int height_bad = 0;
    int width;
    int height;
    int fence;
    int pattern = 0;

    if (scanf("%d %d", &width, &height) != 2)
        {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
        }
    if (width < 3 || width > 69)
        width_bad = 1;

    if (height < 3 || height > 69)
        height_bad = 1;

    if (height_bad || width_bad)
    {
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return 101;
    }

    if(width % 2 == 0)
    {
        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
        return 102;
    }
    int gap = width/2;
    int mid_gap = -1;

    if (width == height)
    {
        if (scanf("%d", &fence) != 1)
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
        if (fence > height - 1 || fence < 1)
        {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return 103;
        }
    }
    
    
    
    for (int k = 0; k < width/2; k++)                        //střecha      
    {
        for (int l = 0; l < width; l++)
        {
            if ((l == gap) || (l == gap + mid_gap + 1))
                printf("X");
            else if (l < gap + mid_gap + 2)
                printf(" ");
        }
        printf("\n");
        gap -= 1;
        mid_gap += 2;
    }
    for (int i = 0; i < height; i++)                              //domeček
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == height-1 || j == 0 || j == width-1)              
                printf("X");
            else if(height == width)
            {
                if (pattern % 2 == 0)
                    printf("o");
                else
                    printf("*");
            }
            else                   
                printf(" ");

            pattern += 1;
        }

        if (height == width && (i == (height - fence) || i == height - 1)) 
        {
            for(int m = 0; m < fence; m++)
            {
                if (fence % 2 == 1)
                {
                    if(m % 2 == 0)
                        printf("|");
                    else
                        printf("-");
                }
                else
                {
                    if(m % 2 == 0)
                        printf("-");
                    else
                        printf("|");
                }  
            } 
        }

        if (height == width && i > height - fence && i != height - 1)
            {
                for(int n = 0; n < fence; n++)
                {
                    if (fence % 2 == 1)
                    {
                        if(n % 2 == 0)
                            printf("|");
                        else
                            printf(" ");
                    }
                    else
                    {
                        if(n % 2 == 0)
                            printf(" ");
                        else
                            printf("|");
                    }  
                }
            }
        printf("\n");
    }
    return 0;
}
