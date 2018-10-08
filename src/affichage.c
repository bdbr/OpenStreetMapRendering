#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <math.h>
#include "../header/affichage.h"



                /************************************************************************************************/
                                /**chercher un noeud dont on connais l'identifiant dans un arbre **/
                /************************************************************************************************/


int searchNode(node *tree,  int id,NOEUD* n)
{
    while(tree)
    {
        if(id == tree->n1.id)
            {
                *n = tree->n1;
                //printf("%d %.7f %.7f ",tree->n1.id, tree->n1.lat, tree->n1.lon);
                return 1;

            }

        if(id > tree->n1.id )
            tree = tree->right;
        else
            tree = tree->left;
    }
    return 0;

}


                /************************************************************************************************/
                                /**la fonction compare utilisée pour le trie de tableau de way**/
                /************************************************************************************************/

int compare(const void *elem1, const void *elem2)
    {
        int y1 = ((const struct way*)elem1)->type;
        int y2 = ((const struct way*)elem2)->type;
        if ( y1 < y2)
            return -1;

       else if (y1 >y2)
          return 1;

       else
          return 0;
    }




                /************************************************************************************************/
                                                    /**l'affihage**/
                /************************************************************************************************/


int affichage(way* ways,node* nodes,bounds b,int size)
    {
                                        /**en cas d'erreur d'initialisation de la SDL**/
         if(SDL_Init(SDL_INIT_VIDEO) < 0)
            {
               printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
               return -1;
            }

        SDL_Window* window = SDL_CreateWindow("carte",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED ,WIDTH,HEIGHT,0);

                                        /**en cas d'erreur de creation de la fenetre**/

         if(window == NULL)
            {
                printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
                return EXIT_FAILURE;
            }


        SDL_Renderer* renderer = SDL_CreateRenderer( window,-1,SDL_RENDERER_ACCELERATED);

                                        /**en cas d'erreur de creation d'un renderer**/

        if(renderer == NULL)
            {
                printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
                return -1;
            }

        qsort(ways,size, sizeof(way), compare);// trier le tableau de way

        SDL_SetRenderDrawColor( renderer, 223, 218,217, 255 );
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0,0, 255 );


         int i;

         for(i=0;i<size;i++)
            {
                int j;

                SDL_Point *rue=malloc(ways[i].nbNoeuds*sizeof(SDL_Point));;
                Sint16 *rX = malloc(ways[i].nbNoeuds*sizeof(Sint16));
                Sint16 *rY =malloc(ways[i].nbNoeuds*sizeof(Sint16));;

                    for(j=0;j<ways[i].nbNoeuds;j++)
                        {
                            NOEUD n;
                            searchNode(nodes,ways[i].noeuds[j],&n);
                            rue[j].x= (WIDTH*(n.lon - b.minLon)/(b.maxLon - b.minLon));
                            rue[j].y=HEIGHT-(HEIGHT*(n.lat - b.minLat)/(b.maxLat - b.minLat ));
                            rX[j]=(WIDTH*(n.lon - b.minLon)/(b.maxLon - b.minLon));
                            rY[j]=HEIGHT-(HEIGHT*(n.lat - b.minLat)/(b.maxLat - b.minLat));

                        }
                if(ways[i].type==HIGHWAY)
                    {
                        if(strcmp(ways[i].v,"service")&&strcmp(ways[i].v,"footway")&&strcmp(ways[i].v,"cycleway"))
                            {
                                SDL_SetRenderDrawColor( renderer, 54,54,58, 255 );
                                SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                            }

                        double m;
                        int f=6;

                        if(strcmp(ways[i].v,"primary")==0)
                            {
                                f=8;
                                SDL_SetRenderDrawColor( renderer, 219, 23,23, 255 );
                            }

                        else if(strcmp(ways[i].v,"motorway")==0)
                            {

                                SDL_SetRenderDrawColor( renderer, 53, 122,183, 255 );

                            }

                        else if(strcmp(ways[i].v,"service")==0)
                            {
                                f=2;
                            }

                        else if(strcmp(ways[i].v,"secondary")==0)
                            {
                                SDL_SetRenderDrawColor( renderer, 255, 189,79, 255 );
                            }

                        else if(strcmp(ways[i].v,"footway")==0)
                            {
                                f=3;
                                SDL_SetRenderDrawColor(renderer,255,66,85,255);
                            }

                        else if(strcmp(ways[i].v,"tertiary")==0)
                            {
                                SDL_SetRenderDrawColor(renderer,240,208,143,255);
                            }

                        else if(strcmp(ways[i].v,"cycleway")==0)
                            {
                                f=3;
                                SDL_SetRenderDrawColor(renderer,0,204,204,255);
                            }

                        else if(strcmp(ways[i].v,"trunk")==0)
                            {
                                SDL_SetRenderDrawColor( renderer, 0, 155,0, 255 );
                            }

                        else
                            {
                                SDL_SetRenderDrawColor( renderer, 255, 255,255, 255 );
                            }

                        for(m =1;m<f;m+=1)
                            {
                                for(j=0;j<ways[i].nbNoeuds;j+=1)
                                    {
                                        rue[j].x   +=1 ;
                                    }

                                SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);

                            }

                        if(strcmp(ways[i].v,"footway") && strcmp(ways[i].v,"cycleway")&&strcmp(ways[i].v,"service"))
                            {
                                SDL_SetRenderDrawColor( renderer, 54, 54,58, 255 );
                                SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                            }

                        SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);

                    }

                else if(ways[i].type==TOURISM)
                    {
                        filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,234,204,242,255);
                        polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,0,0,0,255);
                    }

                else if(ways[i].type==BUILDING)
                    {
                        filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,192,191,169,255);
                        polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,0,0,0,255);
                         //stringRGBA(renderer,rX[ways[i].nbNoeuds/2],rY[ways[i].nbNoeuds/2],ways[i].name,15,15,15,255);
                    }

                 else if(ways[i].type==WATER)
                    {

                        filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,171,200,226,255);

                    }

                else if(ways[i].type==BOUNDARY)
                    {
                        SDL_SetRenderDrawColor(renderer,154,113,210,255);
                        SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                        SDL_SetRenderDrawColor(renderer,0,0,0,255);
                    }

                else if(ways[i].type==COASTLINE)
                    {

                    }

                else if(ways[i].type==LEISURE)
                    {
                        filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,103,182,100,255);
                        polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,0,0,0,255);
                    }

                else if(ways[i].type==11)
                    {
                        filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,223, 218,217,255);
                        polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,0, 0,0,247);
                    }

                else if(ways[i].type==LANDUSE)
                    {
                        if(strcmp(ways[i].v,"residential")==0)
                            {
                                filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,191,212,180,255);
                                polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,211, 232,200,255);
                            }
                        else
                            {
                                filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,10,255,10,100);
                                polygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,5, 130,5,255);
                            }

                    }

                else if(ways[i].type==WATERWAY)
                    {
                        if(strcmp(ways[i].v,"riverbank")==0)
                            {
                                filledPolygonRGBA(renderer,rX,rY,ways[i].nbNoeuds,136,173,186,200);
                            }
                        else
                            {
                            int m;
                            SDL_SetRenderDrawColor( renderer, 100, 50,255, 100 );

                            for(m =0;m<3;m+=1)
                                {
                                    for(j=0;j<ways[i].nbNoeuds;j++)
                                        {
                                            rue[j].x+=1;
                                        }

                                    SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                                }

                            SDL_SetRenderDrawColor( renderer, 0, 0,0, 255 );
                            }


                     //SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                    }

                else if(ways[i].type==RAILWAY)
                    {
                        if(strcmp(ways[i].v,"rail")==0||strcmp(ways[i].v,"funicular")==0)
                            {
                                SDL_SetRenderDrawColor( renderer, 173, 162,133, 255 );
                                SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                                SDL_SetRenderDrawColor( renderer, 0, 0,0, 255 );
                            }
                        else
                            {
                                
                                //SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                                //SDL_SetRenderDrawColor( renderer, 0, 0,0, 255 );

                            }

                    }
                else
                    {
                        //SDL_SetRenderDrawColor( renderer, 0, 0,0, 255 );
                        //SDL_RenderDrawLines(renderer,rue,ways[i].nbNoeuds);
                    }
                free(rX);
                free(rY);
                free(rue);

        }
        
        SDL_RenderPresent(renderer);
        SDL_Event event;
        int continuer=1;

        while(continuer)
            {
                SDL_WaitEvent(&event);

                switch (event.type)
                    {
                        case SDL_WINDOWEVENT:
                            if(event.window.event ==  SDL_WINDOWEVENT_CLOSE)
                                {
                                    continuer = 0;
                                }
                        break;

                    }
            }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;

    }

