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
