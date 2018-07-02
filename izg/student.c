/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */
int ticksLast = 0;
float n = 0;

/* Materialy */
const S_Material    MAT_WHITE_AMBIENT  = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_DIFFUSE  = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_SPECULAR = { 1.0, 1.0, 1.0, 1.0 };

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;



/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    /* napr. renderer->base.releaseFunc = studrenRelease; */
    /* ??? */
    renderer->base.releaseFunc = studrenRelease;
    renderer->base.projectTriangleFunc = studrenProjectTriangle;

    /* inicializace nove pridanych casti */
    /* ??? */
    
    renderer->texture = loadBitmap(TEXTURE_FILENAME, &renderer->textureX, &renderer->textureY);

    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
        /* ??? */
        if(renderer->texture) {
        
            free(renderer->texture);
        }
        
        /* fce default rendereru */
        renRelease(ppRenderer);
    }

}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer, S_Triangle *triangle,
                        double *wTriangle,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3
                         )
{
    /* zaklad fce zkopirujte z render.c */
    /* ??? */
    int         minx, miny, maxx, maxy;
    int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int         s1, s2, s3;
    int         x, y, e1, e2, e3;
    double      alpha, beta, gamma, w1, w2, w3, z;
    S_RGBA      col1, col2, col3, color;

    IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

    /* vypocet barev ve vrcholech */
    col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
    col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
    col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

    /* obalka trojuhleniku */
    minx = MIN(x1, MIN(x2, x3));
    maxx = MAX(x1, MAX(x2, x3));
    miny = MIN(y1, MIN(y2, y3));
    maxy = MAX(y1, MAX(y2, y3));

    /* oriznuti podle rozmeru okna */
    miny = MAX(miny, 0);
    maxy = MIN(maxy, pRenderer->frame_h - 1);
    minx = MAX(minx, 0);
    maxx = MIN(maxx, pRenderer->frame_w - 1);

    /* Pineduv alg. rasterizace troj.
       hranova fce je obecna rovnice primky Ax + By + C = 0
       primku prochazejici body (x1, y1) a (x2, y2) urcime jako
       (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

    /* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
    a1 = y1 - y2;
    a2 = y2 - y3;
    a3 = y3 - y1;
    b1 = x2 - x1;
    b2 = x3 - x2;
    b3 = x1 - x3;

    /* koeficient C */
    c1 = x1 * y2 - x2 * y1;
    c2 = x2 * y3 - x3 * y2;
    c3 = x3 * y1 - x1 * y3;

    /* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
    s1 = a1 * x3 + b1 * y3 + c1;
    s2 = a2 * x1 + b2 * y1 + c2;
    s3 = a3 * x2 + b3 * y2 + c3;

    if ( !s1 || !s2 || !s3 )
    {
        return;
    }

    /* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
    if( s1 < 0 )
    {
        a1 *= -1;
        b1 *= -1;
        c1 *= -1;
    }
    if( s2 < 0 )
    {
        a2 *= -1;
        b2 *= -1;
        c2 *= -1;
    }
    if( s3 < 0 )
    {
        a3 *= -1;
        b3 *= -1;
        c3 *= -1;
    }

    /* koeficienty pro barycentricke souradnice */
    alpha = 1.0 / ABS(s2);
    beta = 1.0 / ABS(s3);
    gamma = 1.0 / ABS(s1);

    /* vyplnovani... */
    for( y = miny; y <= maxy; ++y )
    {
        /* inicilizace hranove fce v bode (minx, y) */
        e1 = a1 * minx + b1 * y + c1;
        e2 = a2 * minx + b2 * y + c2;
        e3 = a3 * minx + b3 * y + c3;

        for( x = minx; x <= maxx; ++x )
        {
            if( e1 >= 0 && e2 >= 0 && e3 >= 0 )
            {
                /* interpolace pomoci barycentrickych souradnic
                   e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
                w1 = alpha * e2;
                w2 = beta * e3;
                w3 = gamma * e1;

                /* interpolace z-souradnice */
                z = w1 * v1->z + w2 * v2->z + w3 * v3->z;

                /// Povodny vypocet
                /// double u = triangle->t[0].x * w1 + triangle->t[1].x * w2 + triangle->t[2].x * w3;
                /// double v = triangle->t[0].y * w1 + triangle->t[1].y * w2 + triangle->t[2].y * w3;
                //
                
                double u = (triangle->t[0].x * w1 / wTriangle[0] + 
                    triangle->t[1].x * w2 / wTriangle[1] + 
                    triangle->t[2].x * w3 / wTriangle[2]) / (
                    w1 / wTriangle[0] + 
                    w2 / wTriangle[1] + 
                    w3 / wTriangle[2]);

                double v = (triangle->t[0].y * w1 / wTriangle[0] + 
                    triangle->t[1].y * w2 / wTriangle[1] + 
                    triangle->t[2].y * w3 / wTriangle[2]) / (
                    w1 / wTriangle[0] + 
                    w2 / wTriangle[1] + 
                    w3 / wTriangle[2]);

                S_RGBA color2 = studrenTextureValue((S_StudentRenderer*) pRenderer, u, v);
               
                /* interpolace barvy */
                color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red);
                color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green);
                color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue);
                color.alpha = 255;

                color.red = color.red * color2.red / 255;
                color.green = color.green * color2.green / 255;
                color.blue = color.blue * color2.blue / 255;

                /* vykresleni bodu */
                if( z < DEPTH(pRenderer, x, y) )
                {
                    PIXEL(pRenderer, x, y) = color;
                    DEPTH(pRenderer, x, y) = z;
                }
            }

            /* hranova fce o pixel vedle */
            e1 += a1;
            e2 += a2;
            e3 += a3;
        }
    }
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
    /* zaklad fce zkopirujte z render.c */
    /* ??? */
    S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
    S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
    S_Coords    nn;                     /* normala trojuhelniku po transformaci */
    int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
    S_Triangle  * triangle;
    int         vertexOffset, vertexOffset2, normalOffset, normalOffset2; /* offset pro vrcholy a normalove vektory trojuhelniku */
    int         i0, i1, i2, in, i20, i21, i22, i2n; /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
    double wTriangle[3];

    IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0 );

    /* z modelu si vytahneme i-ty trojuhelnik */
    triangle = trivecGetPtr(pModel->triangles, i);

    /* ziskame offset pro vrcholy n-teho snimku */
    vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n-teho snimku */
    normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;

    /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    i0 = triangle->v[ 0 ] + vertexOffset;
    i1 = triangle->v[ 1 ] + vertexOffset;
    i2 = triangle->v[ 2 ] + vertexOffset;

    /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    in = triangle->n + normalOffset;

    //////////////// MOOOOOOOJEEEEE /////////////////////
    
    /* ziskame offset pro vrcholy n-teho snimku */
    vertexOffset2 = (((int)n + 1) % pModel->frames) * pModel->verticesPerFrame;

    /* ziskame offset pro normaly trojuhelniku n-teho snimku */
    normalOffset2 = (((int)n + 1) % pModel->frames) * pModel->triangles->size;

    /* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    i20 = triangle->v[ 0 ] + vertexOffset2;
    i21 = triangle->v[ 1 ] + vertexOffset2;
    i22 = triangle->v[ 2 ] + vertexOffset2;

    /* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
    i2n = triangle->n + normalOffset2;

    n -= (int) n;

    S_Coords newCoords;
    S_Coords *origCoords = cvecGetPtr(pModel->vertices, i0);
    S_Coords *origCoords2 = cvecGetPtr(pModel->vertices, i20);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    //////////////// MOOOOOOOJEEEEE /////////////////////
    
    /* transformace vrcholu matici model */
    trTransformVertex(&aa, &newCoords);

    origCoords = cvecGetPtr(pModel->vertices, i1);
    origCoords2 = cvecGetPtr(pModel->vertices, i21);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVertex(&bb, &newCoords);

    origCoords = cvecGetPtr(pModel->vertices, i2);
    origCoords2 = cvecGetPtr(pModel->vertices, i22);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVertex(&cc, &newCoords);

    /* promitneme vrcholy trojuhelniku na obrazovku */
    wTriangle[0] = trProjectVertex(&u1, &v1, &aa);
    wTriangle[1] = trProjectVertex(&u2, &v2, &bb);
    wTriangle[2] = trProjectVertex(&u3, &v3, &cc);

    /* pro osvetlovaci model transformujeme take normaly ve vrcholech */

    origCoords = cvecGetPtr(pModel->normals, i0);
    origCoords2 = cvecGetPtr(pModel->normals, i20);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVector(&naa, &newCoords);

    origCoords = cvecGetPtr(pModel->normals, i1);
    origCoords2 = cvecGetPtr(pModel->normals, i21);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVector(&nbb, &newCoords);

    origCoords = cvecGetPtr(pModel->normals, i2);
    origCoords2 = cvecGetPtr(pModel->normals, i22);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVector(&ncc, &newCoords);

    /* normalizace normal */
    coordsNormalize(&naa);
    coordsNormalize(&nbb);
    coordsNormalize(&ncc);

    /* transformace normaly trojuhelniku matici model */

    origCoords = cvecGetPtr(pModel->trinormals, in);
    origCoords2 = cvecGetPtr(pModel->trinormals, i2n);
    newCoords.x = origCoords->x + (origCoords2->x - origCoords->x) * n;
    newCoords.y = origCoords->y + (origCoords2->y - origCoords->y) * n;
    newCoords.z = origCoords->z + (origCoords2->z - origCoords->z) * n;

    trTransformVector(&nn, &newCoords);
    
    /* normalizace normaly */
    coordsNormalize(&nn);

    /* je troj. privraceny ke kamere, tudiz viditelny? */
    if( !renCalcVisibility(pRenderer, &aa, &nn) )
    {
        /* odvracene troj. vubec nekreslime */
        return;
    }

    /// wTriangle[0] = trProjectVertex(&u1, &v1, &triangle->t[0]);
    /// wTriangle[1] = trProjectVertex(&u2, &v2, &triangle->t[0]);
    /// wTriangle[2] = trProjectVertex(&u3, &v3, &triangle->t[0]);

    /* rasterizace trojuhelniku */
    studrenDrawTriangle(pRenderer, triangle, wTriangle,
                    &aa, &bb, &cc,
                    &naa, &nbb, &ncc,
                    u1, v1, u2, v2, u3, v3
                    );
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
    /* ??? */
    // unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    // return makeColor( c, 255 - c, 0 );
    
    /** 
     * x,y
     * ROUND(x),y
     * x,ROUND(Y)
     * ROUND(X),ROUND(Y)
     */

    double posX = u * pRenderer->textureX;
    double posY = v * pRenderer->textureY;

    S_RGBA pixelX1 = pRenderer->texture[(int)posX*pRenderer->textureY + (int)posY];
    S_RGBA pixelX2 = pRenderer->texture[ROUND(posX)*pRenderer->textureY + (int)posY];
    S_RGBA pixelY1 = pRenderer->texture[(int)posX*pRenderer->textureY + ROUND(posY)];
    S_RGBA pixelY2 = pRenderer->texture[ROUND(posX)*pRenderer->textureY + ROUND(posY)];

    u -= (int) u;
    v -= (int) v;
    
    pixelX1.red = (1 - v) * ((1-u) * pixelX1.red + u * pixelX2.red) + v * ((1-u) * pixelY1.red + u * pixelY2.red);
    pixelX1.green = (1 - v) * ((1-u) * pixelX1.green + u * pixelX2.green) + v * ((1-u) * pixelY1.green + u * pixelY2.green);
    pixelX1.blue = (1 - v) * ((1-u) * pixelX1.blue + u * pixelX2.blue) + v * ((1-u) * pixelY1.blue + u * pixelY2.blue);

    return pixelX1;
}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
    /* zaklad fce zkopirujte z main.c */
    /* ??? */
    /* test existence frame bufferu a modelu */
    IZG_ASSERT(pModel && pRenderer);

    /* nastavit projekcni matici */
    trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

    /* vycistit model matici */
    trLoadIdentity();

    /* nejprve nastavime posuv cele sceny od/ke kamere */
    trTranslate(0.0, 0.0, pRenderer->scene_move_z);

    /* nejprve nastavime posuv cele sceny v rovine XY */
    trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

    /* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
    trRotateX(pRenderer->scene_rot_x);
    trRotateY(pRenderer->scene_rot_y);

    /* nastavime material */
    renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
    renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
    renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

    /* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
    renderModel(pRenderer, pModel, n);
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku
     * a pro interpolaci mezi snimky */
    /* ??? */
    
    n += (float)(ticks-ticksLast)/100.0;
    ticksLast = ticks;
}

/*****************************************************************************
 *****************************************************************************/
