#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int x,y;
} Pereche;

typedef struct
{
    Pereche p;
    char c;
} Mutare;

void afis ( char **m , int n )
{
    int i, j;
    for (i=0 ; i <n; i++)
    {
        for(j =0 ; j < n; j++)
            printf( "%c ", m[i][j] );
        printf ("\n");
    }
}

char** alocare( char **board, int n)
{
    int i;
    board = (char **) malloc (sizeof(char*)*(n+1));
    for (i=0; i<n; i++)
        board[i] = (char*) malloc (n+1);
    return board;
}

void init ( char **m, int n)
{
    int i, j;
    for ( i = 0 ; i < n; i++)
        for (j = 0; j < n ; j++)
            m[i][j] = '-';
}

void eliberare (char** board, int n)
{
    int i;
    for (i =0 ; i < n; i++)
        free(board[i]);
    free(board);
}

Pereche verif_ddp(char **board , int n, int i)
{
    int j;
    Pereche p;

    p.x = p.y = -1;
    for (j = 0 ; j<=n-i; j++)
        if ( board [j][j+i] == '-')
        {
            p.x = j;
            p.y = i+j;
            return p;
        }
    return p;
}

Pereche verif_sdp(char **board , int n, int i)
{
    int j;
    Pereche p;

    p.x = p.y = -1;
    for (j = 0 ; j<=n-i; j++)
        if ( board [j+i][j] == '-')
        {
            p.x = j+i;
            p.y = j;
            return p;
        }
    return p;
}

Pereche verif_dp (char **board, int n)
{
    int i;
    Pereche p;
    p.x = p.y = -1;

    for( i=0 ; i< n && p.x == -1 ; i++)
        if( board[i][i] == '-')
        {
            p.x = p.y = i;
            return p;
        }
    return p;
}

void verificare( char **board, int n , int *a, int *b,  char c )
{
    int i ,j;
    Pereche p, pp ;
    if( board[*a][*b] == '-')
        board[*a][*b] = c;
    else
    {
        printf ("NOT AN EMPTY CELL\n");
        p = verif_dp (board, n);
        for ( i = 1; i < n && p.x == -1 ; i++)
        {
            p = verif_ddp(board ,n, i);
            if( p.x == -1)
                p = verif_sdp (board, n, i);
        }
        if ( p.x == -1 )
            printf("FULL BOARD\n");
        else board[p.x][p.y] = c;
    }
}

char castigator ( char **board, int n , int xx, int yy)
{
    int i, j;
    _Bool bb = true;
    xx = xx * n;
    yy = yy * n;
    char ch = board[xx][yy] ;
    /// verificare diag principala
    for ( i = 0 ; i < n && bb ; i++)
        if( board[i+xx][i+yy] != ch )
            bb = false;
    if ( bb )
        return ch;
    /// verificare pe linii
    for ( i = 0 ; i < n ; i ++)
    {
        ch = board[xx+i][yy];
        bb = true;
        for ( j = 0 ; j < n && bb ; j++)
            if( board[xx+i][yy+j] != ch)
                bb = false;
        if ( bb )
            return ch;
    }
    /// verificare  pe coloane
    for ( i = 0 ; i < n ; i ++)
    {
        ch = board[i+xx][yy];
        bb = true;
        for ( j = 0 ; j < n && bb ; j++)
            if( board[j+xx][i+yy] != ch)
                bb = false;
        if ( bb )
            return ch;
    }
    /// verificare pe diag secundara
    bb = true;
    ch = board[xx][yy+n-1];
    for ( i = 0 ; i < n && bb ; i++)
        if(board[i+xx][n-1-i+yy] != ch )
            bb = false;


    if (bb)
        return ch;
    return '-';
}

void mutari(char **board, int n , int m, char** macro)
{
    Mutare w;
    int i, px, py;
    char ch = '0';

    for (i = 0; i< m ; i++)
    {
        scanf ( " %c%d%d", &(w.c), &(w.p.x), &(w.p.y)  );
        //printf( "%c%d%d\n", w.c , w.p.x, w.p.y);
        if( (w.p.x) < 0 || (w.p.x) >= n*n || (w.p.y) < 0 || (w.p.y) >= n*n )
            printf("INVALID INDEX\n");
        else
        {
            if ( w.c == ch)
                printf("NOT YOUR TURN\n");
            else
            {
                verificare( board, n ,&(w.p.x), &(w.p.y), w.c );
                if( w.p.x != -1)
                {
                    px = w.p.x / n;
                    py = w.p.y / n;
                    if( macro [px][py] == '-')
                        macro [px][py] = castigator( board, n , px, py);
                }
                else printf ("FULL BOARD\n");
            }
        }
        ch = w.c;
    }
}

int nr_x (char **board ,int n)
{
    int i, j, a = 0;
    for ( i =0 ; i < n; i++)
        for ( j = 0; j<n; j++)
            if( board[i][j] == 'X')
                a++;
    return a;
}

int nr_0 ( char **board ,int n)
{
    int i, j,a = 0;
    for ( i =0 ; i < n; i++)
        for ( j = 0; j< n; j++)
            if( board[i][j] == '0')
                a++;
    return a;
}

int main()
{
    char **board, **macro, ch;
    int i, j, n, m;
    float x, o;
    int xx ,oo;

    scanf("%d %d", &n, &m);
    ///test citire dimensiuni
    // printf("%d %d", n, m);

    /// alocarea dinamica matricei tabla - matricea mare
    board = alocare ( board, n*n);

    ///alocare dinamica a matricei macro
    macro = alocare(macro, n);

    /// initializare matrice cu '-'
    init( board, n*n );
    init (macro, n);

    /// citirea mutarilor ,verificarea lor si punerea in matrice
    mutari ( board ,n, m, macro);

    /// test de afisare a matricei mare
//    afis( board , n*n);
//    printf("\n");
//    afis( macro, n);

    x = nr_x (board ,n*n);
    o = nr_0 (board ,n*n);
    xx = nr_x (macro ,n);
    oo = nr_0 (macro ,n);

    x = xx/x;
    o = oo/o;

    ch = castigator(macro, n, 0, 0);
    if( ch == '-')
        printf("Draw again! Let's play darts!\n");
    else printf("%c", ch);

    printf( "X %f \n0 %f ", x, o);

    ///eliberare memorie
    eliberare(board, n*n);
    eliberare(macro , n);

    return 0;
}
