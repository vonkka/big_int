graph* graph_transpose(graph* g) {
	graph* new_g = NULL;
	if (g) {
		new_g = graph_create(g->count);
		for (int i = 0; i < g->count; ++i) {
			grph_node* cur = g->adj_list[i].head;
			while (cur) {
				add_arc(new_g, cur->num, i);
				cur = cur->next;
			}
		}
	}
	return new_g;
}

void time_dfs_step(graph* g, int** time_op, int** time_cl, int cur, int* cl_pos) {
	grph_node* adj = g->adj_list[cur].head;
	while (adj) {
		if ((*time_op)[adj->num] == -1) {
			(*time_op)[adj->num] = 1;
			time_dfs_step(g, time_op, time_cl, adj->num, cl_pos);
		}
		adj = adj->next;
	}
	(*time_cl)[cur] = *cl_pos;
	--(*cl_pos);
	return;
}

void time_dfs(graph* g, int** time_op, int** time_cl, int* cl_pos, int* ord) {
	int* cr_ord = (int*)malloc(sizeof(int) * g->count);
	if (!ord) {
		for (int i = 0; i < g->count; ++i) {
			cr_ord[i] = i;
		}
	}
	else cr_ord = ord;
	for (int i = 0; i < g->count; ++i) {
		if ((*time_op)[cr_ord[i]] == -1) {
			(*time_op)[cr_ord[i]] = 1;
			time_dfs_step(g, time_op, time_cl, cr_ord[i], cl_pos);
		}
	}
	return;
}

int* dfs2_gr(graph* g, int* order) {
	int* time_op = (int*)malloc(sizeof(int) * g->count);
	int* time_cl = (int*)malloc(sizeof(int) * g->count);
	int cl_pos = g->count - 1;
	for (int i = 0; i < g->count; ++i) {
		time_op[i] = -1;
		time_cl[i] = -1;
	}
	time_dfs(g, &time_op, &time_cl, &cl_pos, order);
	int k = 0;
	int* sorted_f = (int*)malloc(sizeof(int) * g->count);
	while (k < g->count) {
		for (int i = 0; i < g->count; ++i) {
			if (time_cl[i] == k) sorted_f[k] = i;
		}
		++k;
	}
	return sorted_f;
}

void scc_2dfs(graph* g) {
	if (g) {
		int* sorted_f = dfs2_gr(g, NULL);
		int* sorted_s = dfs2_gr(graph_transpose(g), sorted_f);

		cout << "\nStrongly connected components:\n";
		int i = 0; int k = 0;
		while (i < g->count) {
			if (sorted_f[i] == sorted_s[g->count - k - 1]) {
				cout << sorted_s[g->count - k - 1] << '\n';
				++k;
				i = k;
			}
			else {
				cout << sorted_s[g->count - k - 1] << ", ";
				++k;
			}
		}
		cout << '\n';
	}
}
