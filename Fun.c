#include "Fun.h"

void leerListaBloqueo(Map * MapaBloqueo)
{
  FILE *archivo = fopen("listabloqueo.txt", "r"); // Modo lectura
  char bufer[15];         // Aquí vamos a ir almacenando cada línea
  while (fgets(bufer,15, archivo))
  {
        strtok(bufer, "\n");
        insertMap(MapaBloqueo,strdup(bufer),bufer);//insertamos en mapa
  }
}


void listFiles(const char* dirname,TreeMap* mapaLibro, TreeMap* mapaLibroTitulos) {
  //esta funcion se  accede a la carpeta donde estan los archivos
    
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    struct dirent* entity;

    entity = readdir(dir);
    while (entity != NULL) { 
        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFiles(path,mapaLibro,mapaLibroTitulos);//esto
        }
        agregarLibros(mapaLibro,entity->d_name,mapaLibroTitulos);
        entity = readdir(dir);
    }

    closedir(dir);
}


void leerPalabras(char* carpeta,libro* lib)
{
  Map * MapaBloqueo = createMap(is_equal_string);
  leerListaBloqueo(MapaBloqueo);
  FILE *fp = fopen (carpeta, "r");
  int cont = 0;
  char linea[200];
  int i = 0;
  int cont2 = 0;
  bool flag = true;
  palabra* aux2 = (palabra*)malloc(sizeof(palabra));
  
  while (fgets (linea, 200, fp) != NULL) // Se lee la linea
  { 
        
          char* aux = get_csv_field(linea, i); // Se obtiene el nombre
          while (aux != NULL)
          {
            char* aux3 = (char*)malloc(sizeof(char));
            aux3 = searchMap(MapaBloqueo,aux);
            if (aux3 == NULL)
            {
              if (aux != NULL) 
              {
                aux2 = searchTreeMap(lib->mapaPalabras, aux);
                if(aux2 == NULL)
                {
                  palabra* aux2 = (palabra*)malloc(sizeof(palabra));
                  strcpy(aux2->palabra,strdup(aux));
                  aux2->cant++;
                  insertTreeMap(lib->mapaPalabras,strdup(aux),aux2);

                }
                else aux2->cant++;
              }
            }
            i++;
            aux = get_csv_field(linea, i);
          }
        cont++;
    i=0;

  }
}


void CargarDocumentos(TreeMap* mapa, TreeMap * mapaLibroTitulo)
{
  listFiles("./Libros",mapa,mapaLibroTitulo);
}

void obtenerNombre(char *key,TreeMap* mapaLibros, TreeMap* mapaLibroTitulos)
{
  int  i = 0;
  int cont = 0;
  libro* lib = (libro*)malloc(sizeof(libro));
  lib->mapaPalabras = createTreeMap(lower_than_string);
  strcpy(lib->Id,key);
  
  char carpeta[100000] = "./Libros/";
  
  if (strcmp(key,".") != 0 && strcmp(key,"..")!= 0) 
  {
    strcat(carpeta, key);
    FILE *pf = fopen (carpeta, "r");
	  char letra;
    char str[1000];
    
    while(feof(pf) == 0)
    {
      letra = fgetc(pf);
      if(cont > 33) 
      {   
        if(letra == ',')
        {
          break;
        }
        if(letra == '\n')
        {
          break;
        }
        lib->titulo[i] = letra;
        i++;
      }
      cont++;
    }
    printf("%s\n", lib->titulo);
    insertTreeMap(mapaLibros,lib->Id,lib);
    insertTreeMap(mapaLibroTitulos,lib->titulo,lib);
    leerPalabras(carpeta,lib);
  }

}





void agregarLibros (TreeMap* mapaLibros,char* key, TreeMap* mapaLibroTitulo )
{//obtenemos los nombres le qutamos el autor
  libro * aux = (libro*)malloc(sizeof(libro));
  aux = (libro*)searchTreeMap(mapaLibros, key);
  
  if(aux == NULL)
  {
    obtenerNombre(key,mapaLibros,mapaLibroTitulo);
  
  }
  else exit(1);
  
}
char *get_csv_field (char * tmp, int k) 
{//leemos palabras
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if((open_mark || tmp[i]!= ' ') && (isalpha(tmp[i])) != 0) {
            if(k==j) ret[i-ini_i] = tolower(tmp[i]);
            i++;
            continue;
        }

        if(tmp[i]== ' '){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
      ret[i-ini_i] = 0;
      return ret;
    }
    return NULL;
}

void MostrarDocumentosOrdenados(TreeMap * mapaDesordenado)//mostramos documentos ordenados
{
  libro* aux = (libro *)firstTreeMap(mapaDesordenado);

  while(aux!=NULL)
  {
    printf("Id : %s\n",aux->Id);
    printf("Titulo : %s\n",aux->titulo);
    printf("\n");
    aux = (libro *)nextTreeMap(mapaDesordenado);
  }
}

void CalcularCantDoc (TreeMap * mapaLibros, libro * aux)
{
  /*se calcula la cantidad de documentos en los que aprece la palabra (aux)*/
  libro * auxLibro = (libro*)malloc(sizeof(libro));
  palabra* auxPalabra = (palabra*)malloc(sizeof(palabra));
  palabra* auxPalabra2 = (palabra*)malloc(sizeof(palabra));
  float cont = 0;

  auxLibro = firstTreeMap(mapaLibros);
  auxPalabra = firstTreeMap(aux->mapaPalabras);
  // va recorriendo todos los libros buscando la palabra
    while (auxPalabra != NULL)
    {
      auxLibro = firstTreeMap(mapaLibros);
      cont=0;
      while (auxLibro != NULL)
      {
        auxPalabra2 = searchTreeMap(auxLibro->mapaPalabras,auxPalabra->palabra);
        if (auxPalabra2 != NULL) 
        {
          cont++; // si la encuentra se aumenta el contador
        }
        auxLibro = nextTreeMap(mapaLibros);
      }
      auxPalabra->cantidadDoc = cont; //asignamos el contador a la variable cantidadDoc de cada palabra
       auxPalabra = nextTreeMap(aux->mapaPalabras); //seguimos con  la siguiente palabra
    }
  
}

void masRelevantes(TreeMap* mapaLibros,palabra* palabrarelev)
{
  //se ordenar de mayor a menor las 10 palabras con mayor relevancia
  char key[1000];
  libro * aux2 =  (libro*)malloc(sizeof(libro));
  palabra* pal  = (palabra*)malloc(sizeof(palabra));
  
   printf("Ingrese el ID de el texto\n");
  printf("Por favor incluya .txt después de el número\n");
  scanf("%s", key);
  getchar();
  aux2 =  (libro *)searchTreeMap(mapaLibros,key);
  pal = (palabra *)firstTreeMap(aux2->mapaPalabras);
  
  int i = 0;
  if(aux2 == NULL)
  {
    printf("Lo sentimos, el libro ingresado no pertenece a esta carpeta\n");
  }
  else
  {
  for(int j = 0; j < 10 ; j++)
  {
    bool esta = false;
    //se van ordenando en un arreglo segun relevancia
   while(pal != NULL)
    {
      if(pal->relevancia > palabrarelev[j].relevancia)
      {
        for(int i = 0; i < 10 ; i++)
        {
          if(strcmp(palabrarelev[i].palabra,pal->palabra)==0) //se verifica si la palabra ya se encuentra 
           {
            esta = false;
             break;
           }
          else
            esta = true;
            
        }
        if(esta == true)
        {  
          //se agrega la palabra a la posicion del arreglo
          strcpy(palabrarelev[j].palabra, pal->palabra);
          palabrarelev[j].relevancia = pal->relevancia;
        }
      }
      pal = nextTreeMap(aux2->mapaPalabras);
   
     }
    pal = firstTreeMap(aux2->mapaPalabras);
    
    }
  }
  printf("palabras con mayor relevancia : \n");
  //se printean las palabras
  for(int j = 0 ; j < 10; j++)
  {
    printf("%s %f\n", palabrarelev[j].palabra,  palabrarelev[j].relevancia);
  }
  
}

/*void calcularRelevancia(palabra* palabra, libro* libro, TreeMap* mapaLibro)
{
  esta funcion iba a ser implementada para mostrar los textos en buscar por palabra, por orden de relevancia 
  CalcularCantDoc(mapaLibro, libro);
  palabra->relevancia = (palabra->cant/libro->cant)*(log10(100/palabra->cantidadDoc));
  printf("%s %f\n",palabra->palabra, palabra->relevancia);
  //if (palabraaux != NULL)
  //{
       // printf("palabra : %s |||| relevancia : %f\n",palabraaux->palabra ,palabraaux->relevancia); 
  //}
  //palabraaux = nextTreeMap(aux->mapaPalabras);
} */

void BuscarporPalabra(TreeMap * mapaLibro)
{
  //se buscan los libros que contengan la palabra dada
  char pal[100];
  
  printf("ingresa una palabra : ");
  scanf("%s",pal);
  getchar();
  palabra * aux = (palabra *)malloc(sizeof(palabra));
  //palabra * aux2 = (palabra *)malloc(sizeof(palabra));
  //libro *Libro = (libro* )firstTreeMap(mapaLibro);
  libro*  Libro2 = (libro *)firstTreeMap(mapaLibro);

  //Libro = (libro *)firstTreeMap(mapaLibro);
 // aux=searchTreeMap(Libro->mapaPalabras,pal);
  
  while(Libro2 !=NULL)
  {
    //printf("%s\n", Libro->titulo );
    aux=searchTreeMap(Libro2->mapaPalabras,pal);

    if(aux != NULL)
    {
      printf("%s\n", Libro2->titulo );
    }
    //Libro = nextTreeMap(mapaLibro); 
    Libro2 = nextTreeMap(mapaLibro); 

  }
 
  
}

void PalabrasRelevantes(TreeMap* mapaLibro, TreeMap* mapaLibroTitulos)
{
  //CalcularCantDoc(mapaLibroTitulos);
  //Calcula la relevancia de las palabras
 palabra* palabrarelev = (palabra*)calloc(9,sizeof(palabra));
  
  libro * aux = (libro*) malloc(sizeof(libro));
  char titulo[1000];
  char id[1000];
  printf("Ingrese el id del texto\n");
  scanf("%s", id);
  getchar();
  
  aux = searchTreeMap(mapaLibro,id);
  if (aux != NULL)
  {
    CalcularCantDoc(mapaLibro, aux);
  }
  
  
  palabra* palabraaux = (palabra*)malloc(sizeof(palabra));  
  palabraaux = firstTreeMap(aux->mapaPalabras);

  //si se encuentra la palabra en el mapa, se calcula su relevancia segun formula
  while (palabraaux != NULL )
  { 
     
      palabraaux->relevancia = (palabraaux->cant/aux->cant)*(log10(100/palabraaux->cantidadDoc));
      palabraaux = nextTreeMap(aux->mapaPalabras);
  }  
  masRelevantes(mapaLibro,palabrarelev);
  
}


void CalcularFrecuencia(TreeMap * mapaLibro)
{
  //Se saca el primer libro de el mapa
  libro* libro = firstTreeMap(mapaLibro);
  //se obtiene la primera palabra de el Mapa.
  palabra* pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  char auxpal[1000];
  int cont;
  float fre ;
  
  while(libro != NULL || pal != NULL)
  {
    // se inicializa la cant total de palabras en el libro
    libro->cant = 0;
    while(pal != NULL)
    {
      //avanzamos en el mapa palabras y se actualiza la cantidad
      libro->cant = libro->cant + pal->cant;
      strcpy(auxpal,pal->palabra);
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }
  libro = firstTreeMap(mapaLibro);
  pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  cont = 0;
  
  while(libro != NULL || pal != NULL)
  {

    while(pal != NULL)
    {
      //se calcula la frecuencia por palabra
      pal->frecuencia = ((pal->cant) / (libro->cant)); 
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }
  libro = firstTreeMap(mapaLibro);
  pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  cont = 0;

  while(libro != NULL || pal != NULL)
  {

    while(pal != NULL)
    {
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }  
  
}

void PalabrasMayorFrecuencia(TreeMap *mapaLibros)
{
  //buscaremos las 10 palabras con mayor frecuencia, usamos el metodo del arreglo
  libro * aux =  (libro*)malloc(sizeof(libro));
  aux = (libro *)firstTreeMap(mapaLibros);
  char key[1000];
  int i = 0;
  palabra* palabrafrec = (palabra*)calloc(9,sizeof(palabra));
  
  
  float frecuenciaMayor;
  
  printf("Ingrese el ID de el texto\n");
  printf("Por favor incluya .txt después de el número\n");
  scanf("%s", key);
  getchar();
  aux =  (libro *)searchTreeMap(mapaLibros,key);
  palabra* pal = (palabra *)firstTreeMap(aux->mapaPalabras);
  
  if(aux == NULL)
  {
    printf("Lo sentimos, el libro ingresado no pertenece a esta carpeta\n");
  }
  else //a traves de ciclos y comparaciones vamos ordenando en el arreglo las palabras
  {
  for(int j = 0; j < 10 ; j++)
  {
    bool esta = false;
   while(pal != NULL)
    {
      if(pal->frecuencia > palabrafrec[j].frecuencia)
      {
        for(int i = 0; i < 10 ; i++)
        {
          if(strcmp(palabrafrec[i].palabra,pal->palabra)==0) 
           {
              esta = false;
             break;
           }
          else
            esta = true;
            
        }
        if(esta == true)
        {
          strcpy(palabrafrec[j].palabra, pal->palabra);
          palabrafrec[j].frecuencia = pal->frecuencia;
        }
      }
      pal = nextTreeMap(aux->mapaPalabras);
   
     }
    pal = firstTreeMap(aux->mapaPalabras);
    
    }
  }
   printf("palabras con mayor frecuencia : \n");
  for(int j = 0 ; j < 10; j++) //imprimimos 10 palabras con mayor frecuencia
  {
    printf("%s %f\n", palabrafrec[j].palabra,  palabrafrec[j].frecuencia);
  }
}

//INTENTO DE FUNCION MOSTRAR CONTEXTO
/*void MostrarContexto()
{
  char id[10];
  char palabra;
  char titulo[1000];
  char contexto[1024];
  char palabra2;
  printf("Ingrese el id de el libro\n");
  printf("Incluya el .txt\n");
  scanf("%s", id);
  getchar();
  printf("Ingrese el id de el libro\n");
  scanf("%s", titulo);
  getchar();
  printf("Ingrese la palabra a buscar\n");
  scanf("%s", palabra);
  getchar();
  FILE *f = fopen("id.txt","r");
  libro * auxLibro =  (libro*)malloc(sizeof(libro));
  palabra * auxPal = (palabra*)malloc(sizeof(palabra));
  auxLibro = searchTreeMap(mapaLibro, titulo);
  if(auxLibro == NULL)
  { 
    printf("Lo sentimos este libro no se encuentra en la carpeta");
  }
  auxPal = searchTreeMap(auxLibro->mapaPalabras, palabra);
  if(auxPal == NULL)
  {
    printf("Lo sentimos, esta palabra no se encuentra en este libro\n");
  }
  while (fscanf(f, "%[^\n ] ", palabra2) != EOF)
  {
    int pos = ftell(f);
    if(strcmp(palabra, palabra2) == 0)
    {
      if(fscanf(f, " %1023s", contexto) == 1)
      {
        printf("CONTEXTO : %s", contexto);
        
      }
    }
  }
  
}*/

//No funciona el scanf, no sabemos porque sigue pidiendo datos.
void BuscarLibroporTitulo(TreeMap * mapaLibro)
{
  char lineapalabras[80];
  int cont = 0;
  int i = 0 ;
  char palabras[20];
  libro * lib =  (libro*)malloc(sizeof(libro));
  palabra * pal =  (palabra*)malloc(sizeof(palabra));
  int isspace ;
  
  printf("Ingrese las palabras\n");
  fflush(stdin);
  scanf("%80[^\n]s", lineapalabras);
  fflush(stdin);
  List* listaPalabras = createList();

  //Ciclo para sacar las palabras, guardar en lista y aumentar contador;
  char *token = strtok(lineapalabras, " ");
  while (token != NULL)
  {
    pushBack(listaPalabras, token);
    cont++;
    token = strtok(NULL, " ");
  }  
  lib = (libro *) firstTreeMap(mapaLibro);
  //se recorren todos los libros
  while(lib != NULL)
  {
    //se saca la primera palabra de la lista para ver comenzar la busqueda
    strcpy(palabras, firstList(listaPalabras));
    int j = 0;
    bool todas = false;
    //el ciclo se procesa por la cant de palabras ingresadas por el usuario
    while(j <= cont)
    {
      pal = (palabra *)searchTreeMap(lib->mapaPalabras, palabras);
      //si no se encuentra, ya no debe procesarse el libro
      if(pal == NULL)
      {
        todas = false;
        break;
      }
      else todas = true;
      //se pasa a analizar la siguiente palabra
      strcpy(palabras, nextList(listaPalabras));
      j++;
    }
    //Si en el texto estaban todas las palabras, se imprime el libro
    if(todas == true)
    {
      printf("%s\n", lib->titulo);
    }
  }

}