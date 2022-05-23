#include "treemap.h"
#include "Fun.h"
#include "hashmap.h"
#include "list.h"

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

int lower_than_string(void* key1, void* key2){
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k1,k2)<0) return 1;
    return 0;
}

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

int main(void) 
{
  TreeMap* mapaLibro = createTreeMap(lower_than_string);
  TreeMap* mapaLibroTitulos = createTreeMap(lower_than_string);
  List * listaBloqueo = createList();
 // GuardarListaBloqueo(listaBloqueo);
  

  //MENU
  int op;
  printf("MENÚ\n");
  printf("1.-  Cargar documentos\n");
  printf("2.-  Mostrar documentos Ordenados\n");
  printf("3.-  Buscar un libro por título\n");
  printf("4.-  Palabras con mayor frecuencia\n");
  printf("5.-  Palabras más relevantes.\n");
  printf("6.-  Buscar por palabra\n");
  printf("7.-  Mostrar palabra en su contexto dentro del libro\n");
  printf("¿Que opción desea realizar?\n\n");
  scanf("%d", &op);
  if(op < 1 && op > 7)
  {
    printf("Por favor ingrese una operacion válida\n");
    scanf("%d", &op);
    if(op < 1 && op > 7)
    {
      printf("Operacion no existente\n");
      return 1;
    }
  }
  while(op >= 1 && op <= 7)
  {
    switch(op)
    {
    case 1:
    {
      CargarDocumentos(mapaLibro,mapaLibroTitulos);
      break;
      
    }
    case 2:
    {
      MostrarDocumentosOrdenados(mapaLibro);
      break;
    }
    case 3:
    {
      BuscarLibroporTitulo(mapaLibro);
      break;
    
    }
    case 4:
    {
      CalcularFrecuencia(mapaLibro);
      PalabrasMayorFrecuencia(mapaLibro);
      break;
    }
    case 5:
    {
      CalcularFrecuencia(mapaLibro);
      PalabrasRelevantes(mapaLibro,mapaLibroTitulos);
      break;
      
    }
    case 6:
    {
      CalcularFrecuencia(mapaLibro);
      BuscarporPalabra(mapaLibro);
      break;
    }
    case 7:
    {
    //MostrarContexto();
    }
    
    }
    printf("¿Desea realizar otra operación?\n");
    printf("1 -> SI | 2 -> NO\n");
    int rep;
    scanf("%d", &rep);
    getchar();
  
    while(rep != 1 && rep != 2)
    {
      printf("¿Desea realizar otra operación?\n");
      scanf("%d", &rep);  
      getchar();
    }
    
    if(rep == 2) return 0;  
    printf("\n¿Qué operación desea realizar?\n");
    scanf("%d", &op);  
    getchar();
  }
  
  
    
  return 0;
} 
