#include <iostream>
#include "time.h"
#include <windows.h>
#include <string.h>
#include "euclid.h"
#include "big_int.h"
#include "polynom.h"
#include "help_functions.h"
#include "RPN.h"
#include "trees.h"
#include "graph.h"
using namespace std;

int main()
{   
    //GRAPHS

    //while (true) {
    //    cout << "Enter the graph size: ";
    //    int size;
    //    cin >> size;
    //    graph* check_gr = graph_create(size);
    //    for (int i = 0; i < size; ++i) {
    //        int input = 1; int len = 0;
    //        int* adj = str_to_mas_int(&input, &len);
    //        for (int k = 0; k < len; ++k) {
    //            if (i != adj[k]) add_adj(&check_gr->adj_list[i].head, adj[k]);
    //        }
    //    }
    //    graph_print(check_gr);
    //    graph* edged = edged_graph(check_gr);
    //    graph_print(edged);
        //add_edge(check_gr, 3, 0);
        //graph_print(edged);
    //}
    //    graph* edged = edged_graph(check_gr);
    //    graph_print(edged);
    //    graph_print(check_gr);

        //del_arc(check_gr, 1, 3);
        //graph_print(check_gr);
        //del_arc(check_gr, 3, 1);
        //graph_print(check_gr);
        //add_edge(check_gr, 1, 3);
        //graph_print(check_gr);
        //graph_free(check_gr);
    //}

    //BPT

    while (true) {
        cout << "Enter the graph size: ";
        int size;
        cin >> size;
        graph* check_gr = graph_create(size);
        for (int i = 0; i < size; ++i) {
            int input = 1; int len = 0;
            int* adj = str_to_mas_int(&input, &len);
            for (int k = 0; k < len; ++k) {
                if (i != adj[k]) add_adj(&check_gr->adj_list[i].head, adj[k]);
            }
        }
        graph_print(check_gr);
        que* q = topology_sort_dfs(check_gr);
        if (q) {
                cout << '\n' << "Sorted list: ";
                while (q->next) {
                    que* temp = q;
                    cout << q->num << ", ";
                    q = q->next;
                    free(temp);
                }
                cout << q->num << '\n';
                cout << '\n';
            }
        else cout << '\n' << "There is cycle" << '\n' << '\n';
        q_free(q);
    }
}
