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
    if (check_gr) {
        graph_print(check_gr);
        graph_out(check_gr, "C:/Users/1/Desktop/Programming/graph_out.txt");
    }

int graph_in(const char* filename) {
	FILE* myfile;
	errno_t err;
	err = fopen_s(&myfile, filename, "r");
	if (!err) {
		char* line = (char*)malloc(sizeof(32));
		int i = 0;
		int len = 0;
		fgets(line, 32, myfile);
		int* mas = fstr_toint(line, &len);
		int size = mas[0];
		if (size > 0) {
			graph* g = graph_create(size);
			while (fgets(line, 32, myfile)) {
				len = 0;
				int* adj = fstr_toint(line, &len);
				if (adj) {
					for (int k = 0; k < len; ++k) {
						if (i != adj[k]) add_adj(&g->adj_list[i].head, adj[k]); // free mas and adj??
					}
				}
				++i;
			}
			graph_print(g);
		}
		fclose(myfile);
		return 1;
	}
	return 0;
}

int graph_out(graph* g, const char* filename) {
	FILE* myfile;
	errno_t err;
	err = fopen_s(&myfile, filename, "w");
	if (!err) {
		fprintf(myfile, "%d\n", g->count);
		for (int i = 0; i < g->count; ++i) {
			fprintf(myfile, "%d: ", i);
			grph_node* node_ptr = g->adj_list[i].head;
			if (node_ptr) {
				while (node_ptr->next) {
					fprintf(myfile, "%d, ", node_ptr->num);
					node_ptr = node_ptr->next;
				}
				fprintf(myfile, "%d", node_ptr->num);
			}
			fprintf(myfile, "%c", '\n');
		}
		return 1;
	}
	return 0;
}
