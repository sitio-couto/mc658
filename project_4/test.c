#include "dcmstp-solver.h"

int main(int argc, char *argv[]){
    int i, j;
    double **lg;
    mat_graph *g;
    struct out *o;
    
    // Verifying arguments
    if (argc < 4){
        printf("Usage: dcmstp-solver <instance> <max_time> <method>\n");
        return 1;
    }
    
    //g = read_input_list(argv[1]);
    g = read_input_matrix(argv[1]);
    
    mat_graph new;

    int t0[5] = {0,2,2,6,0};
    int t1[5] = {2,0,3,8,5};
    int t2[5] = {0,3,0,0,7};
    int t3[5] = {6,8,0,0,9};
    int t4[5] = {0,5,7,3,0};
    int *t[5] = {t0,t1,t2,t3,t4};

    new.m = 0;
    new.deg = calloc(5, sizeof(int));
    new.n = 5;
    new.mat = t;
    // printf("HMST=(%d)",first_primal(&new));

    lg = malloc(sizeof(double*)*g->n);
    for(i=0; i<new.n; i++)
        lg[i] = malloc(sizeof(double)*g->n);

    for (i=0; i < new.n; i++){
        for(j=0; j<new.n; j++){	
            lg[i][j] = new.mat[i][j];
        }
    }

    int mst = mst_prim(lg, new.n);

    return 0;
}

int first_primal(mat_graph *g) {
    int c1, c2;
    int i, j, k, primal = 0;
    int qnt_e = (g->n*g->n - g->n)/2;
    int v[g->n];
    int deg[g->n];
    edge2vert e[qnt_e];

    k = 0;
    for(i=0; i < g->n; ++i) {
        v[i] = 0;
        deg[i] = g->deg[i];
        for(j=i+1; j < g->n; ++j) {
            e[k].a = i;
            e[k].b = j; 
            e[k].cost = g->mat[i][j];
            ++k;
        }
    }

    qsort(e, qnt_e, sizeof(edge2vert), compare);

    k = 0;
    for (i=0; i < qnt_e; ++i) {
        // printf("(%d,%d)->%d\n",e[i].a,e[i].b,e[i].cost);
        c1 = (!v[e[i].a] || !v[e[i].b]);
        // c2 = ((deg[e[i].a] > 0 && deg[e[i].b] > 0));

        if (c1 && c2) {
            if (!v[e[i].a]) {
                v[e[i].a] = 1;
                ++k;
            }
            if (!v[e[i].b]){
                v[e[i].b] = 1;
                ++k;
            }
            --deg[e[i].a]; 
            --deg[e[i].b];

            primal += e[i].cost;
        }
        
        if (k == g->n) break;
    }

    printf("(%d)\n", primal);
    for (i=0; i < g->n; ++i){
        if(v[i] == 0) printf("FLAWED!!\n");
    }
    
    return primal;
}

int compare(const void * a, const void * b) { 
    return (((edge2vert*)a)->cost - ((edge2vert*)b)->cost); 
}