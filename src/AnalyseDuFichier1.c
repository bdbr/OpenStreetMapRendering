#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdint.h>
#include <libxml/xpath.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include "../header/affichage.h"



                    /************************************************************************************************/
                                /**fonction getBounds prend en argument un pointeur vers un document
                                            et retourne une variable de type bounds**/
                    /************************************************************************************************/

bounds getBounds(xmlDocPtr doc)
    {
        bounds b;
        xmlNodePtr cur = xmlDocGetRootElement(doc);
        cur = cur->xmlChildrenNode;

        while(cur != NULL)
            {
                if((!xmlStrcmp(cur->name, (const xmlChar *)"bounds")))
                    {
                        xmlChar* maxlat = xmlGetProp(cur, (const xmlChar *)"maxlat");
                        b.maxLat =atof((char *)maxlat);
                        xmlChar* minlat = xmlGetProp(cur, (const xmlChar *)"minlat");
                        b.minLat =atof((char *)minlat);
                        xmlChar* minlon = xmlGetProp(cur, (const xmlChar *)"minlon");
                        b.minLon =atof((char *)minlon);
                        xmlChar* maxlon = xmlGetProp(cur, (const xmlChar *)"maxlon");
                        b.maxLon =atof((char *)maxlon);
                        break;
                    }
                cur = cur->next;
            }
        return b;

    }

                    /************************************************************************************************/
                                /**fonction addNode utilisée pour l'insertion d'un noeuds
                                            dans un arbre binaire de recherche**/
                    /************************************************************************************************/

void addNode(node **tree, NOEUD n)
    {
        node *tmpNode;
        node *tmpTree = *tree;

        node *elem = malloc(sizeof(node));
        elem->n1 = n;
        elem->left = NULL;
        elem->right = NULL;

        if(tmpTree)
            do
                {
                    tmpNode = tmpTree;

                    if(n.id > tmpTree->n1.id )
                        {
                            tmpTree = tmpTree->right;

                                if(!tmpTree) tmpNode->right = elem;
                        }
                    else
                        {
                            tmpTree = tmpTree->left;
                            if(!tmpTree) tmpNode->left = elem;
                        }
                }
            while(tmpTree);

        else  *tree = elem;
    }



                    /************************************************************************************************/
                                        /**fonction getReference le moteur de l'application**/
                    /************************************************************************************************/




void getReference(xmlDocPtr doc, xmlNodePtr cur,way*ways,node**nodetreee,int**rel,int*t)
    {

        int* tmp=malloc(sizeof(int));
        xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
        cur = cur->xmlChildrenNode;
        node *nodetree=NULL;
        int y=0,q=0;


        while (cur != NULL)
            {

                        /************************************************************************************************/
                                            /**chercher tous les noeuds et les mettre dans un arbre**/
                        /************************************************************************************************/


                if((!xmlStrcmp(cur->name, (const xmlChar *)"node")))
                    {
                        NOEUD n;
                        xmlChar* id = xmlGetProp(cur, (const xmlChar *)"id");
                        int id1 =atoi((char *)id);

                        xmlChar* lat = xmlGetProp(cur, (const xmlChar *)"lat");
                        double lat1 =atof((char *)lat);

                        xmlChar* lon = xmlGetProp(cur, (const xmlChar *)"lon");
                        double lon1 =atof((char *)lon);

                        n.id=id1;
                        n.lat=lat1;
                        n.lon=lon1;

                        addNode(&nodetree, n);

                    }

                     /************************************************************************************************/
                                    /**chercher tous les chemain  et les mettre dans un tableau**/
                    /************************************************************************************************/

                     else if((!xmlStrcmp(cur->name, (const xmlChar *)"way")))
                        {
                            xmlChar* id = xmlGetProp(cur, (const xmlChar *)"id");
                            ways[y].id=atoi((char *)id);

                            xmlXPathSetContextNode(cur,ctxt);
                            xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST "tag[@k='name']/@v", ctxt);
                            ways[y].name=(char*)xmlXPathCastToString(xpathRes);
                            char * tag = "tag[@k='waterway' or @k='building' or @k='highway'or @k='natural' or @k='landuse' or @k='leisure' or @k='tourism'or @k='boundary']/@k";
                            char * tag2 = "tag[@k='waterway' or @k='building' or @k='highway'or @k='natural' or @k='landuse' or @k='leisure' or @k='tourism'or @k='boundary']/@v";
                            xpathRes = xmlXPathEvalExpression(BAD_CAST tag, ctxt);
                            xmlXPathObjectPtr  xpathRes2 = xmlXPathEvalExpression(BAD_CAST tag2, ctxt);
                            char*type=(char*)xmlXPathCastToString(xpathRes);


                        /************************************************************************************************/
                                            /**gestion des types et des sous-types de  "way"**/
                        /************************************************************************************************/

                            if(strcmp(type,"highway")==0)
                                {
                                    ways[y].type = HIGHWAY;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"tourism")==0)
                                {
                                    ways[y].type = TOURISM;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"building")==0)
                                {
                                    ways[y].type = BUILDING;

                                }

                            else if(strcmp(type,"natural")==0)
                                {
                                   xmlXPathObjectPtr  xpathRes2 = xmlXPathEvalExpression(BAD_CAST "tag[@k='natural']/@v", ctxt);
                                   char*nat=(char*)xmlXPathCastToString(xpathRes2);

                                   if(strcmp(nat,"water")==0)
                                       {
                                           ways[y].type = WATER;
                                       }
                                   else
                                       {
                                            ways[y].type = COASTLINE;
                                       }

                                }

                            else if(strcmp(type,"leisure")==0)
                                {
                                    ways[y].type =  LEISURE;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"landuse")==0)
                                {
                                    ways[y].type =  LANDUSE;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"waterway")==0)
                                {
                                    ways[y].type = WATERWAY;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"boundary")==0)
                                {
                                    ways[y].type = BOUNDARY;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else if(strcmp(type,"railway")==0)
                                {
                                    ways[y].type = RAILWAY;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                            else
                                {
                                    ways[y].type = 12;
                                    ways[y].v = (char*)xmlXPathCastToString(xpathRes2);
                                }

                /************************************************************************************************/
                                            /**récupérer les noeuds de chaque way**/
                /************************************************************************************************/

                            xpathRes = xmlXPathEvalExpression(BAD_CAST "nd", ctxt);

                            if (xpathRes->type == XPATH_NODESET)
                                {

                                    int j;
                                    int k =(xpathRes->nodesetval->nodeNr);
                                    ways[y].nbNoeuds=k;
                                    ways[y].noeuds = malloc(k*sizeof(NOEUD));
                                   // printf("ID: %d nom: %s\n",ways.id,ways.name);
                                    for (j = 0; j < xpathRes->nodesetval->nodeNr; j++)
                                        {
                                                xmlNodePtr n = xpathRes->nodesetval->nodeTab[j];
                                                xmlChar* ref = xmlGetProp(n, (const xmlChar *)"ref");
                                                ways[y].noeuds[j]= atoi((char*)ref);

                                        }

                                }
                            y=y+1;
                            xmlFree(id);
                            xmlXPathFreeObject(xpathRes);


                        }

                /************************************************************************************************/
                                            /**récupérer les relations qu'il y a entre les "way"**/
                /************************************************************************************************/


                    else if((!xmlStrcmp(cur->name, (const xmlChar *)"relation")))
                        {
                            xmlXPathSetContextNode(cur,ctxt);
                            xmlXPathObjectPtr  xpathRes = xmlXPathEvalExpression(BAD_CAST "tag[@k='type']/@v", ctxt);
                            char*pol=(char*)xmlXPathCastToString(xpathRes);

                            if(strcmp(pol,"multipolygon")==0)
                                {
                                    xmlNodePtr enf = cur->xmlChildrenNode;

                                    while(enf!=NULL)
                                        {
                                            if((!xmlStrcmp(enf->name, (const xmlChar *)"member")))
                                                {
                                                    char*role=(char*)xmlGetProp(enf, (const xmlChar *)"role");

                                                    if(strcmp(role,"inner")==0)
                                                        {
                                                            tmp[q]=atoi((char*)xmlGetProp(enf, (const xmlChar *)"ref"));
                                                            q++;
                                                            tmp = realloc(tmp,(q+1)*sizeof(int));
                                                        }

                                                 }
                                            enf = enf->next;
                                        }

                                }

                        }
                cur = cur->next;

            }
        *rel = tmp;
        *t=q;
        *nodetreee = nodetree;

        xmlXPathFreeContext(ctxt);
        return;

    }



                        /**la fonction estPresent retourne 1 si le way represente une cours interieure et 0 sinon **/

int estPresent(int*m,int id,int size)
    {
        int i;

        for(i=0;i<size;i++)
            {
                if(m[i]==id)
                    {
                        return 1;
                    }
            }
        return 0;
    }


                                /** verification du fichier osm**/
void parseDoc(char *docname)
    {

        xmlDocPtr doc;//declarer un pointeur qui pointe vers le document
        xmlNodePtr cur;//declarer un pointeur de noeud
        doc = xmlParseFile(docname);//pour parser un fichier XML, elle prend en argument le nom du document

                                /*au cas d'une erreur */

        if (doc == NULL )
            {
                fprintf(stderr,"Document not parsed successfully. \n");
                return;
            }

        cur = xmlDocGetRootElement(doc);// recuperer l'element racine du document

                            /*s'il y a pas un element racine */
        if (cur == NULL)
            {
                fprintf(stderr,"empty document\n");
                xmlFreeDoc(doc);
                return;
            }

                        /*si l'element racine est different de "osm"*/

        if (xmlStrcmp(cur->name, (const xmlChar *) "osm"))
            {
                fprintf(stderr,"document of the wrong type, root node != osm");
                xmlFreeDoc(doc);
                return;
            }
                    /************************************************************************************************/
                            /** remplir l'arbre des noeuds et le tableau de way afin d'afficher la carte **/
                    /************************************************************************************************/

        xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
        xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST "count(//relation/tag[@v='multipolygon'])", ctxt);
        int j = xmlXPathCastToNumber(xpathRes);
        int* r = NULL;
        xpathRes = xmlXPathEvalExpression(BAD_CAST "count(//way)", ctxt);
        int i = xmlXPathCastToNumber(xpathRes);
        way *w = malloc(i*sizeof(way));

         xmlXPathFreeObject(xpathRes);
         xmlXPathFreeContext(ctxt);
         bounds b = getBounds(doc);
        node * n =  NULL;

        getReference(doc, cur,w,&n,&r,&j);
        int v;

        for(v=0;v<i;v++)
            {
                 if(estPresent(r,w[v].id,j))
                     {
                        if(w[v].type>10)
                            {
                                w[v].type = 11;
                             }
                     }
             }

        affichage(w,n,b,i);

}


