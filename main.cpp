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



// Topology sort

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



// SCC

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

    //    int* comps = scc_2dfs(check_gr);
    //    if (comps) {
    //        cout << "\nStrongly connected components:\n";
    //        bool check = 0;
    //        for (int n = 0; n < check_gr->count; ++n) {
    //            for (int i = 0; i < check_gr->count; ++i) {
    //                if (comps[i] == n) {
    //                    cout << i << ' ';
    //                    check = 1;
    //                }
    //            }
    //            if (check) cout << '\n';
    //            check = 0;  
    //        }
    //    }
    //    else {
    //        cout << "Incorrect input\n";
    //        break;
    //    }

    //    comps = scc_1dfs(check_gr);
    //    if (comps) {
    //        cout << "\nStrongly connected components:\n";
    //        bool check = 0;
    //        for (int n = 0; n < check_gr->count * 3; ++n) {
    //            for (int i = 0; i < check_gr->count; ++i) {
    //                if (comps[i] == n) {
    //                    cout << i << ' ';
    //                    check = 1;
    //                }
    //            }
    //            if (check) cout << '\n';
    //            check = 0;
    //        }
    //    }
    //    else {
    //        cout << "Incorrect input\n";
    //        break;
    //    }
    //}


// SAT2

    while (true) {
        int len = 0;
        cout << "Enter the number of values: ";
        char* input = (get_str_input(&len));
        char* num = (char*)malloc(sizeof(len));
        for (int i = 0; i < len - 1; ++i) {
            if (isdigit(input[i])) num[i] = input[i];
            else {
                num = NULL;
                break;
            }
        }
        char* endp = NULL;
        int size = NULL;
        if (!num) cout << "Wrong input";
        else {
            size = strtol(num, &endp, 10);
            if (size) {
                len = 0;
                char* eq = get_str_input(&len);
                int* solution = sat2(eq, size);
                for (int i = 0; i < size; ++i) cout << 'n' << i << ": " << solution[i] << '\n';
            }
            else cout << "Invalid size" << '\n';
        }
        
    }
