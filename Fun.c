  #include "Fun.h"

void leerListaBloqueo(Map * MapaBloqueo)
{
  FILE *archivo = fopen("listabloqueo.txt", "r"); // Modo lectura
  char bufer[15];         // Aquí vamos a ir almacenando cada línea
  while (fgets(bufer,15, archivo))
  {
      // Aquí, justo ahora, tenemos ya la línea. Le vamos a remover el salto
        strtok(bufer, "\n");
        // La imprimimos, pero realmente podríamos hacer cualquier otra cosa
        //printf("La línea es: '%s'\n", bufer);
        insertMap(MapaBloqueo,strdup(bufer),bufer);
  }
}





void listFiles(const char* dirname,TreeMap* mapaLibro, TreeMap* mapaLibroTitulos) {
    
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    struct dirent* entity;
    //printf("Reading files in: %s\n", dirname);

    entity = readdir(dir);
    while (entity != NULL) {
        //printf("%hhd %s/%s\n", entity->d_type, dirname, entity->d_name);
        //printf("%s\n" ,entity->d_name); 
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
  //printf("hola");
  FILE *fp = fopen (carpeta, "r");
  int cont = 0;
  char linea[200];
  int i = 0;
  int cont2 = 0;
  bool flag = true;
  palabra* aux2 = (palabra*)malloc(sizeof(palabra));
  
  //printf("TEXTO NUEVO\n\n\n\n\n\n\n\n\n\n\n\n");
  while (fgets (linea, 200, fp) != NULL) // Se lee la linea
  { 
        //while(feof(fp)==0)//esta malo
        //if(cont %100==0) printf("%d\n", cont); 
        //{ 
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
                  //printf("entro null\n");
                  palabra* aux2 = (palabra*)malloc(sizeof(palabra));
                  strcpy(aux2->palabra,strdup(aux));
                  aux2->cant++;
                  insertTreeMap(lib->mapaPalabras,strdup(aux),aux2);
                  //insertTreeMap(lib->mapaPalabras);
                  //aux2->cant++;
                }
                else
                {
                  aux2->cant++;
                  //printf("HOLAAAAAAAAAAAAA\n");
                }         
              }
            }
            i++;
            aux = get_csv_field(linea, i);
          }
        cont++;
        //}
    i=0;

  }
}






void CargarDocumentos(TreeMap* mapa,List* lista, TreeMap * mapaLibroTitulo)
{
  char archivo[1000];
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
    //printf("Se va a insertar al mapa\n");
    
    insertTreeMap(mapaLibros,lib->Id,lib);
    insertTreeMap(mapaLibroTitulos,lib->titulo,lib);
    leerPalabras(carpeta,lib);
  }

}





void agregarLibros (TreeMap* mapaLibros,char* key, TreeMap* mapaLibroTitulo )
{
  //guardar key
  libro * aux = (libro*)malloc(sizeof(libro));
  aux = (libro*)searchTreeMap(mapaLibros, key);
  
  if(aux == NULL)
  {
    obtenerNombre(key,mapaLibros,mapaLibroTitulo);
  
  }
  else exit(1);
  
}
char *get_csv_field (char * tmp, int k) 
{
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







void BuscarporPalabra(TreeMap * mapaLibro)
{
  char pal[100];
  
  printf("ingresa una palabra : ");
  scanf("%s",pal);
  getchar();
  palabra * aux = (palabra *)malloc(sizeof(palabra));
  libro* Libro = (libro *)firstTreeMap(mapaLibro);
  //printf("'%s'\n",pal);
  while(Libro !=NULL)
  {
    //printf("%s\n", Libro->titulo );
    aux=searchTreeMap(Libro->mapaPalabras,pal);
    if(aux != NULL)
    {
      printf("%s\n",Libro->titulo);
      
    }
    Libro = nextTreeMap(mapaLibro);
  }
  //CalcularFrecuencia(mapaLibro);
  
}






void MostrarDocumentosOrdenados(TreeMap * mapaDesordenado)
{

  libro* aux = (libro *)firstTreeMap(mapaDesordenado);

  while(aux!=NULL)
  {
    printf("Id : %s\n",aux->Id);
    printf("Titulo : %s\n",aux->titulo);
    
    aux = (libro *)nextTreeMap(mapaDesordenado);
  }
}





void CalcularCantDoc (TreeMap * mapaLibros, libro * aux)
{
  libro * auxLibro = (libro*)malloc(sizeof(libro));
  palabra* auxPalabra = (palabra*)malloc(sizeof(palabra));
  palabra* auxPalabra2 = (palabra*)malloc(sizeof(palabra));
  float cont = 0;

  auxLibro = firstTreeMap(mapaLibros);
  auxPalabra = firstTreeMap(aux->mapaPalabras);
  
  
    while (auxPalabra != NULL)
    {
      auxLibro = firstTreeMap(mapaLibros);
      cont=0;
      while (auxLibro != NULL)
      {
        //cont=0;
        //printf("holaaaaaaaaa\n");
        auxPalabra2 = searchTreeMap(auxLibro->mapaPalabras,auxPalabra->palabra);
        //printf("palabra %s\n", auxPalabra2->palabra);
        if (auxPalabra2 != NULL) 
        {
          cont++;
          //printf("palabra %s\n", auxPalabra2->palabra);
        }
        auxLibro = nextTreeMap(mapaLibros);
        //printf("contadooooor 2 :%d\n",cont);
      }
      //printf("contadoooor :%d\n",cont);
      auxPalabra->cantidadDoc = cont;
      auxPalabra = nextTreeMap(aux->mapaPalabras); 
    }
}

void masRelevantes(TreeMap* mapaLibros,palabra* palabrarelev)
{
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
  //palabra* palabrarelev = (palabra*)calloc(9,sizeof(palabra));
  if(aux2 == NULL)
  {
    printf("Lo sentimos, el libro ingresado no pertenece a esta carpeta\n");
  }
  else
  {
  for(int j = 0; j < 10 ; j++)
  {
    bool esta = false;
   while(pal != NULL)
    {
      if(pal->relevancia > palabrarelev[j].relevancia)
      {
        printf("hola");
        for(int i = 0; i < 10 ; i++)
        {
          if(strcmp(palabrarelev[i].palabra,pal->palabra)==0) 
           {
            esta = false;
             break;
           }
          else
            esta = true;
            
        }
        if(esta == true)
        {
          printf("hola\n");    //palabrafrec[j].palabra,  palabrafrec[j].frecuencia)  
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
  for(int j = 0 ; j < 10; j++)
  {
    printf("%s %f\n", palabrarelev[j].palabra,  palabrarelev[j].relevancia);
  }
  
}



void PalabrasRelevantes(TreeMap* mapaLibro, TreeMap* mapaLibroTitulos)
{
  //CalcularCantDoc(mapaLibroTitulos);
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
    printf("entro\n");
    CalcularCantDoc(mapaLibro, aux);
    printf("salio\n");
    
  }
  
    //CalcularCantDoc(mapaLibroTitulos, aux);
  
  palabra* palabraaux = (palabra*)malloc(sizeof(palabra));  
  palabraaux = firstTreeMap(aux->mapaPalabras);

  while (palabraaux != NULL )
  { 
     
      palabraaux->relevancia = (palabraaux->cant/aux->cant)*(log10(100/palabraaux->cantidadDoc));
      if (palabraaux != NULL)
      {
       // printf("palabra : %s |||| relevancia : %f\n",palabraaux->palabra ,palabraaux->relevancia); 
      }
      palabraaux = nextTreeMap(aux->mapaPalabras);
  }  
  masRelevantes(mapaLibro,palabrarelev);
  
}
//mostrar los 10 titulos con más altas frecuencias en orden


void CalcularFrecuencia(TreeMap * mapaLibro)
{
  libro* libro = firstTreeMap(mapaLibro);
  palabra* pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  //palabra* auxpal = (palabra *)malloc(sizeof(palabra));
  char auxpal[1000];
  int cont;
  float fre ;
  
  while(libro != NULL || pal != NULL)
  {
    libro->cant = 0;
    while(pal != NULL)
    {
      //CalcularCantDoc(mapaLibro,pal);
      libro->cant = libro->cant + pal->cant;
      strcpy(auxpal,pal->palabra);
      //auxpal = pal;
      
     // printf("palabra: %s\n",pal->palabra);
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }
 // CalcularCantDoc(mapaLibro,pal);
  libro = firstTreeMap(mapaLibro);
  pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  cont = 0;
  
  while(libro != NULL || pal != NULL)
  {

    while(pal != NULL)
    {
      pal->frecuencia = ((pal->cant) / (libro->cant)); 
      //CalcularRelevancia(mapaLibro,pal);
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }
  libro = firstTreeMap(mapaLibro);
  pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  cont = 0;

  //CalcularRelevancia(mapaLibro);
 //CalcularCantDoc(mapaLibro,auxpal,pal);



  
  while(libro != NULL || pal != NULL)
  {

    while(pal != NULL)
    {
     // CalcularCantDoc(mapaLibro,pal);
      //printf("palabra = %s\n", pal->palabra);
      //printf("Frecuencia = %f\n", pal->frecuencia);
      //CalcularCantDoc(mapaLibro,pal);
      pal = nextTreeMap(libro->mapaPalabras);
    }
    libro = nextTreeMap(mapaLibro);
    if (libro != NULL) pal = (palabra *)firstTreeMap(libro->mapaPalabras);
  }  
  
}

void PalabrasMayorFrecuencia(TreeMap *mapaLibros)
{
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
  else
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
          //printf("hola\n");    //palabrafrec[j].palabra,  palabrafrec[j].frecuencia)  
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
  for(int j = 0 ; j < 10; j++)
  {
    printf("%s %f\n", palabrafrec[j].palabra,  palabrafrec[j].frecuencia);
  }
}


/*
  if(aux == NULL)
  {
    printf("Lo sentimos, el libro ingresado no pertenece a esta carpeta\n");
  }
  else
  {
    while(aux != NULL)
    { 
      while(pal != NULL)
      {
        if(pal->frecuencia > palabrafrec[i]->frecuencia)
        {
          for(int j = 0; j < 10 ; j++)
          {
            if(palabrafrec[j]->frecuencia != pal->frecuencia) 
            {
              //palabrafrec[i]->frecuencia = pal->frecuencia;
              //palabrafrec[i]->cant = pal->cant;
              //palabrafrec[i]->relevancia = pal->relevancia;
              //strcpy(palabrafrec[i]->palabra,pal->palabra);
              printf("%s %f\n", pal->palabra,  pal->frecuencia);
            }
          }
        }
        pal = nextTreeMap(aux->mapaPalabras);
      }
      aux = firstTreeMap(mapaLibros);
      i++;
    }
    printf("palabras con mayor frecuencia = ");
    for(int j = 0 ; j < 10; j++)
    {
      printf("%s %f\n", palabrafrec[j]->palabra,  palabrafrec[j]->frecuencia);
    }
  }
*/
/*void MostrarContexto()
{
  char id[10];
  char palabra;
  char titulo[1000];
  char contexto[1024];
  char palabra2;
  printf("Ingrese el id de el libro\n");
 // printf("Incluya el .txt\n");
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
    //
  
    
    
  
  }
  


  
}*/
void BuscarLibroporTitulo(Map * mapaLibro)
{
  char lineapalabras[50];
  int cont = 0;
  libro * lib =  (libro*)malloc(sizeof(libro));
  palabra * pal =  (palabra*)malloc(sizeof(palabra));
  
  printf("Ingrese las palabras\n");
  fgets(lineapalabras, 50, stdin);
  //getchar();
  printf("%s", lineapalabras);

  //Ciclo para sacar las palabras, guardar en lista y aumentar contador;



  lib = firstMap(mapaLibro);
  
  while(lib != NULL)
  {
    
    
    
  }

}