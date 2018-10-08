#include <libxml/parser.h>
#define COASTLINE 1
#define WATERWAY 2
#define LEISURE 3
#define LANDUSE 4
#define HIGHWAY 5
#define WATER 6
#define BUILDING 9
#define TOURISM 8
#define BOUNDARY 7
#define RAILWAY 10

// #include "affichage.h"
typedef struct NOEUD{
int id;
double lat; // latitude
double lon; // longuitude
int visible;
} NOEUD;

typedef struct way{
 int id;
 int nbNoeuds;
 char * name;
 int type;
 char* v;
 int* noeuds;
 
}way;

typedef struct{
    double maxLat;
    double maxLon;
    double minLat;
    double minLon;
}bounds;

typedef struct node
{
    NOEUD n1;
    struct node *left;
    struct node *right;
} node ;

void getReference(xmlDocPtr doc, xmlNodePtr cur,way* a,node** n,int** r,int*i);

void parseDoc(char *docname);

bounds getBounds(xmlDocPtr doc);

void addNode(node **tree, NOEUD n);

int estPresent(int*m,int id,int size);
