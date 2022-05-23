#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>
#include "treemap.h"
#include "hashmap.h"
#include "list.h"
#include "ctype.h"
#include <math.h>

typedef struct
{
  char  Id [1000];
  char titulo[1000];
  float cant;
  TreeMap* mapaPalabras;
}libro;

typedef struct
{
  float frecuencia;
  char palabra[100];
  float cant;
  float cantidadDoc;
  float relevancia;
}palabra;

int lower_than_string(void* key1, void* key2);
char * _strdup(const char * str);
void guardarPalabras(libro *, const char *);
//void GuardarListaBloqueo(List *);
void listFiles(const char* dirname,TreeMap*,TreeMap*);
void CargarDocumentos(TreeMap*,List*, TreeMap*);
void ImportarProductos();
int is_equal_string(void * , void * );
void agregarLibros(TreeMap*, char*,TreeMap*);
void analisisLibro(char*);
void obtenerNombre(char*,TreeMap*,TreeMap*);
void leerPalabras(char*,libro*);
char *get_csv_field (char * , int );
void BuscarporPalabra(TreeMap *);
void MostrarDocumentosOrdenados(TreeMap *);
void leerListaBloqueo(Map * );
void PalabrasMayorFrecuencia(TreeMap *);
void CalcularFrecuencia(TreeMap * );
void CalcularCantDoc(TreeMap * , libro *);
void PalabrasRelevantes(TreeMap*,TreeMap*);
void masRelevantes(TreeMap *,palabra*);
void BuscarLibroporTitulo(Map *);