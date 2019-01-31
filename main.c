// 1. Ho rinominato la funzione kernel in isKernel

// 2. Ho dimenticato di mettere il value nell'STInsert, e' semplicemente un
// contatore dei vertici che viene incrementato ogni volta che inserisco un
// vertice nuovo

// 3. dimenticato di aggiornare il contatore dei vertici nel grafo G-V = V

// 4. Quando itero le linked list ho usato direttamente un int invece che fare
// List t = ...; t != NULL; t = t->next
// Quindi devo correggere tutte le t con t->head

// 5. Dentro isKernel uso i nel for che itera su tutti gli elementi del kernel,
// ma poi uso direttamente i e non kernel[i] per accedere all'elemento del
// kernel i-esimo, idem per j

// 6. Dentro trovaKernelMax ho dichiarato sol come int e non come int*
// 7. idem per convert e res

// 8. Dentro solve valid e' in realta' isKernel

// 9. solve ha bisogno di Graph G come argomento perche e' usato per chiamare
// isKernel

// 10. Dentro DFS non ho N ma G->V

// 11. Nella DFS per evitare i checks sul vettore mark e' solamente necessario
// spostare l'aggiornamento del max sotto il ciclo for ricorsivo, in questo modo
// se il codice dell'aggiornamento e' raggiunto vuol dire che ho necessariamente
// esaurito i percorsi disponibili

#include <stdio.h>
#include <stdlib.h>

#include "ST.h"
#include "graph.h"
#include "list.h"

int LinearSearch(int needle, int *haystack, int len) {
  for (int i = 0; i < len; ++i) {
    if (needle == haystack[i])
      return i;
  }
  return -1;
}

Graph GRAPHLoad(FILE *fp) {
  char idv1[20 + 1], idv2[20 + 1];
  Graph G = GRAPHInit();
  int V = 0;
  while (fscanf(fp, "%s %s ", idv1, idv2) != EOF) { // Leggo tutti i vertici
    if (STSearch(G->st, idv1) == -1) {
      STInsert(G->st, idv1, V++);
    }
    if (STSearch(G->st, idv2) == -1) {
      STInsert(G->st, idv2, V++);
    }
  }
  G->V = V;
  fseek(fp, 0, SEEK_SET); // Torno all'inizio del grafo
  while (fscanf(fp, "%s %s", idv1, idv2) != EOF) {
    int v1 = STSearch(G->st, idv1);
    int v2 = STSearch(G->st, idv2);
    GRAPHInsertE(G, v1, v2);
  }
  fclose(fp);
  return G;
}

void leggiKernel(FILE *fp, Graph G, int *kernel_len, int *kernel) {
  char idv[20 + 1];
  *kernel_len = 0;

  // Leggi tutti i vertici ed inseriscili in un array di int sovradimensionato
  while (fscanf(fp, "%s ", idv) != EOF) {
    kernel[*kernel_len] = STSearch(G->st, idv);
    (*kernel_len)++;
  }
}

int isKernel(Graph G, int *kernel, int kernel_len) {
  for (int i = 0; i < kernel_len; ++i) { // Per ogni elemento del kernel
    for (List t = G->ladj[kernel[i]]; t != NULL;
         t = t->next) { // Per ogni suo arco
      // Se i e' collegato ad un elemento del
      // kernel, ignorando i cappi => non e' valido
      int j = LinearSearch(t->head, kernel, kernel_len);
      if (j != -1 && kernel[j] != kernel[i]) {
        return 0;
      }
    }
  }

  int *mark = calloc(G->V, sizeof(int));

  for (int i = 0; i < kernel_len; ++i) {
    for (List t = G->ladj[kernel[i]]; t != NULL; t = t->next) {
      mark[t->head] = 1;
    }
  }

  // Se, dopo aver marcato tutti i vertici toccati dagli archi del kernel ne
  // trovo uno non marcato e che non appartiene al kernel => non e' valido
  for (int i = 0; i < G->V; ++i) {
    if (mark[i] == 0 && (LinearSearch(i, kernel, kernel_len) == -1)) {
      return 0;
    }
  }

  return 1;
}

int *convert(int *sol, int N, int *len) {
  int *res = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; ++i) {
    if (sol[i] == 1) {
      res[(*len)++] = i;
    }
  }
  return res;
}

void solve(int pos, int *sol, int N, int *kernelMax, int *kernelMax_len,
           Graph G) {
  if (pos == N) {
    int k_len = 0;
    int *k = convert(sol, N, &k_len);
    if (isKernel(G, k, k_len)) {
      if (k_len > *kernelMax_len) {
        *kernelMax_len = k_len;
        for (int i = 0; i < k_len; ++i) {
          kernelMax[i] = k[i];
        }
      }
    }
    return;
  }

  sol[pos] = 1;
  solve(pos + 1, sol, N, kernelMax, kernelMax_len, G);

  sol[pos] = 0;
  solve(pos + 1, sol, N, kernelMax, kernelMax_len, G);
}

void trovaKernelMax(Graph G, int *kernelMax, int *kernelMax_len) {
  int *sol = malloc(G->V * sizeof(int));
  solve(0, sol, G->V, kernelMax, kernelMax_len, G);
  free(sol);
}

void stampaKernelMax(FILE *fp, int *kernel, int kernel_len, Graph G) {
  for (int i = 0; i < kernel_len; ++i) {
    fprintf(fp, "%s\n", STGetIndex(G->st, kernel[i]));
  }
  fclose(fp);
}

// DFS con backtrack per trovare tutti i cammini semplici
// I cicli sono evitati con un vettore mark mentre il parametro count
// conta quanti vertici del kernel ho attraversato
void DFS(int start, int *mark, int *kernel, int kernel_len, Graph G, int *max,
         int count) {
  // Attraversa in tutti i vertici collegati a start che non ho ancora visitato
  for (List t = G->ladj[start]; t != NULL; t = t->next) {
    if (mark[t->head] == 0) {
      mark[t->head] = 1;
      DFS(t->head, mark, kernel, kernel_len, G, max,
          ((LinearSearch(start, kernel, kernel_len) != -1) ? (count + 1)
                                                           : count));
      mark[t->head] = 0;
    }
  }

  // Se non posso piu attraversare piu nessun vertice
  // Analizzo quanti vertici del kernel ho attraversato ed eventualmente
  // aggiorno il max
  if (count > *max) {
    *max = count;
  }
}

int camminoSempliceMax(Graph G, int *kernel, int kernel_len) {
  int max = 0;
  for (int v = 0; v < G->V; ++v) {
    printf("Partendo dal vertice: %s\n", STGetIndex(G->st, v));
    int *mark = calloc(G->V, sizeof(int));
    DFS(v, mark, kernel, kernel_len, G, &max, 0);
    free(mark);
  }
  return max;
}

int main(int argc, char **argv) {
  Graph G = GRAPHLoad(fopen(argv[1], "r")); // 2

  int kernel_len;
  int *kernel = (int *)malloc(G->V * sizeof(int));

  leggiKernel(fopen(argv[2], "r"), G, &kernel_len, kernel);

  printf("Il kernel proveniente dal file e' valido? %d\n",
         isKernel(G, kernel, kernel_len)); // 3

  kernel_len = 0;
  trovaKernelMax(G, kernel, &kernel_len); // 4
  stampaKernelMax(fopen(argv[3], "w"), kernel, kernel_len, G);

  printf("Il cammino semplice che attraversa la maggior parte di nodi del "
         "kernel ne attraversa: %d\n",
         camminoSempliceMax(G, kernel, kernel_len)); // 5
  return 0;
}
