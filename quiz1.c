#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"

//------------------------------------------------------
typedef JRB Graph;
//------------------------------------------------------
Graph createGraph()
{
  return make_jrb();
}
//------------------------------------------------------
void addEdge(Graph g, int v1, int v2)
{
  JRB node1 = jrb_find_int(g, v1);
  JRB node2 = jrb_find_int(g, v2);

  if (node1 == NULL){
    JRB tree1 = make_jrb();
    jrb_insert_int(g, v1, new_jval_v(tree1));
    jrb_insert_int(tree1, v2, new_jval_i(1));
  }
  else {
    JRB tree1 = (JRB)jval_v(node1->val);
    jrb_insert_int(tree1, v2, new_jval_i(1));
  }

  if (node2 == NULL){
    JRB tree2 = make_jrb();
    jrb_insert_int(g, v2, new_jval_v(tree2));
    jrb_insert_int(tree2, v1, new_jval_i(1));
  }
  else {
    JRB tree2 = (JRB)jval_v(node2->val);
    jrb_insert_int(tree2, v1, new_jval_i(1));
  }
}
//------------------------------------------------------
void printGraph(Graph g)
{
  JRB col, row;
  jrb_traverse(col, g){
    printf("%d:   ", col->key);
    row = (JRB)jval_v(col->val);
    if (row){
      JRB rowHead;
      jrb_traverse(rowHead, row){
        printf("%d   ", rowHead->key);
      }
    }
    printf("\n\n");
  }
}
//------------------------------------------------------
int getAdjacentVertices(Graph g, int vertex, int *output)
{
  JRB node = jrb_find_int(g, vertex);
  JRB tree = (JRB)jval_v(node->val);
  
  int count = 0;
  jrb_traverse(node, tree){
    output[count++] = jval_i(node->key);
  }

  return count;
}
//------------------------------------------------------
void deleteGraph(Graph g)
{
  JRB node;
  jrb_traverse(node, g){
    jrb_free_tree(jval_v(node->val));
  }
}
//------------------------------------------------------
void printVertex(int v) { printf("%4d", v); }
//------------------------------------------------------
void BFS(Graph graph, int start, int stop, void (*func)(int)){
    int a[100]; // arraaay cac dinh da duyet qua
    memset(a, 0, sizeof(int) * 100);
    int output[100];
    int n;   //number of adjacent vertices
    int vertex;  // gia tri dinh dang xet
    Dllist item;
    Dllist queue = new_dllist();
    dll_append(queue, new_jval_i(start));
    while (!dll_empty(queue))
    {
        item = dll_first(queue);
        vertex = jval_i(item->val);
        dll_delete_node(item); 
        if (a[vertex] == 0){
            func(vertex);
            a[vertex] = 1;
            if (vertex == stop) return;
            n = getAdjacentVertices(graph, vertex, output);
            for (int i = 0; i < n; i++){
                if (a[output[i]] != 1) dll_append(queue, new_jval_i(output[i]));
            }
        }
    } 
}
//----------------------------------------------------------
int main(){
    Graph g = createGraph();
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 2, 3);
    addEdge(g, 2, 4);
    addEdge(g, 4, 5);
    printf("\nBFS: start from node 1 to 5 : ");
    BFS(g, 1, 3, printVertex);
    printf("\nBFS: start from node 1 to 5 : ");
    BFS(g, 1, -1, printVertex);
}