#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LONG_INT 2147483647
#define STRLEN 32768
#define CMDLEN 15

typedef struct node {
    int weight;
    int id;
    char color;
    struct node *parent;
    struct node *left;
    struct node *right;
    struct node *back;
} node;


int k; //il numero di posti nella classifica
int d; //numero di nodi dei grafi
int actualK; //numero di nodi attualmente presenti nell'albero
int full; // è 1 (true) se actualK == k
long int maxweight; //il massimo peso presente nella classifica
char first; //utile a NON printare lo spazio finale

//RB TREE implementation ----------------------------------------

struct node *root;
struct node *nullnode;

void rightRotate(struct node *x) {
    struct node *y = x->left;
    x->left = y->right;
    if (y->right != nullnode) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void leftRotate(struct node *x) {
    struct node *y = x->right;
    x->right = y->left;
    if (y->left != nullnode) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void fix(struct node *v) {
    struct node *u;

    while (v->parent->color == 'r') {
        if (v->parent == v->parent->parent->right) {
            u = v->parent->parent->left;

            if (u->color == 'r') {
                u->color = 'b';
                v->parent->color = 'b';
                v->parent->parent->color = 'r';
                v = v->parent->parent;
            } else {
                if (v == v->parent->left) {
                    v = v->parent;
                    rightRotate(v);
                }
                v->parent->color = 'b';
                v->parent->parent->color = 'r';
                leftRotate(v->parent->parent);
            }
        } else {
            u = v->parent->parent->right;

            if (u->color == 'r') {
                u->color = 'b';
                v->parent->color = 'b';
                v->parent->parent->color = 'r';
                v = v->parent->parent;
            } else {
                if (v == v->parent->right) {
                    v = v->parent;
                    leftRotate(v);
                }
                v->parent->color = 'b';
                v->parent->parent->color = 'r';
                rightRotate(v->parent->parent);
            }
        }

        if (v == root) {
            break;
        }
    }
    root->color = 'b';
}

struct node *insert(int _weight, int _id) {

    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    newNode->weight = _weight;
    newNode->id = _id;
    newNode->parent = NULL;
    newNode->left = nullnode;
    newNode->right = nullnode;
    newNode->color = 'r';
    newNode->back = NULL;

    struct node *y = NULL;
    struct node *x = root;

    while (x != nullnode || x->weight == _weight) {
        y = x;
        if (x->weight == _weight) {
            break;
        }
        if (newNode->weight < x->weight) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    //x potrebbe essere nullnode (non c'è)

    if (x == nullnode) {
        newNode->parent = y;
        if (y == NULL) {
            root = newNode;
        } else if (newNode->weight < y->weight) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }

        if (newNode->parent == NULL) {
            newNode->color = 'b';
            return newNode;
        }

        if (newNode->parent->parent == NULL) {
            return newNode;
        }

        fix(newNode);
        return newNode;
    }

    //if(x->weight == _weight)
    newNode->back = x->back;
    x->back = newNode;
    return newNode;
}

void initNullNode() {
    nullnode = malloc(sizeof(struct node));
    nullnode->weight = -1;
    nullnode->id = -1;
    nullnode->color = 'b';
    nullnode->parent = NULL;
    nullnode->left = NULL;
    nullnode->right = NULL;
    nullnode->back = NULL;
    root = nullnode;
}

void fixDelete(struct node *x) {
    struct node *s;
    while (x != root && x->color == 'b') {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == 'r') {
                s->color = 'b';
                x->parent->color = 'r';
                leftRotate(x->parent);
                s = x->parent->right;
            }

            if (s->left->color == 'b' && s->right->color == 'b') {
                s->color = 'r';
                x = x->parent;
            } else {
                if (s->right->color == 'b') {
                    s->left->color = 'b';
                    s->color = 'r';
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = 'b';
                s->right->color = 'b';
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == 'r') {
                s->color = 'b';
                x->parent->color = 'r';
                rightRotate(x->parent);
                s = x->parent->left;
            }

            if (s->left->color == 'b' && s->right->color == 'b') {
                s->color = 'r';
                x = x->parent;
            } else {
                if (s->left->color == 'b') {
                    s->right->color = 'b';
                    s->color = 'r';
                    leftRotate(s);
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = 'b';
                s->left->color = 'b';
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = 'b';
}

struct node *minimum(struct node *node) {
    while (node->left != nullnode) {
        node = node->left;
    }
    return node;
}

struct node *maximum(struct node *node) {
    while (node->right != nullnode) {
        node = node->right;
    }
    return node;
}

void rbTransplant(struct node *a, struct node *b) {
    if (a->parent == NULL) {
        root = b;
    } else if (a == a->parent->left) {
        a->parent->left = b;
    } else {
        a->parent->right = b;
    }
    b->parent = a->parent;
}

//non usata nel progetto, ma utile per l'implementazione di un albero RB
void deleteKey(int key) {
    struct node *p = root;
    struct node *z = nullnode;
    struct node *x;
    struct node *y;

    while (p != nullnode) {
        if (p->weight == key) {
            z = p;
        }

        if (p->weight <= key) {
            p = p->right;
        } else {
            p = p->left;
        }
    }

    if (z == nullnode) {
        printf("\nerror, no such node in tree");
    }

    y = z;
    char y_original_color = y->color;
    if (z->left == nullnode) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == nullnode) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (y_original_color == 'b') {
        fixDelete(x);
    }
}

//non usata nel progetto, ma utile per l'implementazione di un albero RB
void deleteNode(struct node *z) {
    struct node *x;
    struct node *y;

    if (z == nullnode) {
        printf("\nerror, no such node in tree");
    }

    y = z;
    char y_original_color = y->color;
    if (z->left == nullnode) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == nullnode) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (y_original_color == 'b') {
        fixDelete(x);
    }
}

int deleteMaxWeight() {
    struct node *z;
    struct node *x;
    struct node *y;

    z = maximum(root);

    if (z->back != NULL) {
        struct node *pb = z->back;
        z->back = z->back->back;
        free(pb);
        return 1; // 1 significa che c'è ancora un nodo più vecchio di questo, non cambiare maxweight
    }

    y = z;
    char y_original_color = y->color;
    if (z->left == nullnode) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == nullnode) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (y_original_color == 'b') {
        fixDelete(x);
    }
    return 0;
}

//END OF RB TREE implementation ----------------------------------------

void print(struct node *p) {
    if (p != nullnode) {

        print(p->left);

        if (p->back != NULL) {
            print(p->back);
        }
        if (first == '1') {
            printf("%d", p->id);
            first = '0';
        } else {
            printf(" %d", p->id);
        }
        print(p->right);
    }
}

//DEBUG
void insertInBucket(struct node *p, int *i, int bucket[]) {
    if (p != nullnode) {
        if (p->back != NULL) {
            insertInBucket(p->back, i, bucket);
        }

        insertInBucket(p->left, i, bucket);
        bucket[(*i)] = p->id;
        (*i)++;
        insertInBucket(p->right, i, bucket);
    }
}

//DEBUG
void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void printID() {
#ifndef EVAL

    int q = 0;
    int bucket[k];
    insertInBucket(root, &q, bucket);

    int i, j;
    int max = k;
    if (actualK < k) max = actualK;
    for (i = 0; i < k - 1; i++)
        for (j = 0; j < max - i - 1; j++)
            if (bucket[j] > bucket[j + 1])
                swap(&bucket[j], &bucket[j + 1]);

    if (actualK > 0)
        printf("%d", bucket[0]);
    for (int s = 1; s < max; s++)
        printf(" %d", bucket[s]);
    printf("\n");

#endif
#ifdef EVAL
    first = '1';
    print(root);
    printf("\n");
#endif
}

//SSSP con Dijkstra

int minimumDistance(long int distance[], char pSet[]) {
    int min = MAX_LONG_INT;
    int min_index = -1;

    for (int v = 0; v < d; v++) {
        if (pSet[v] == '0' && distance[v] <= min) {
            min = distance[v];
            min_index = v;
        }
    }
    return min_index;
}

int applyDijkstra(int graph[d][d]) {
    long int distance[d];
    char pSet[d];

    for (int i = 0; i < d; i++) {
        pSet[i] = '0';
        distance[i] = MAX_LONG_INT;
    }

    distance[0] = 0;

    int u;
    for (int c = 0; c < d - 1; c++) {
        u = minimumDistance(distance, pSet);
        pSet[u] = '1';

        for (int v = 0; v < d; v++) {
            if ((pSet[v] == '0') && (graph[u][v] != 0) && (distance[u] != MAX_LONG_INT) &&
                (distance[u] + graph[u][v] < distance[v])) {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }

    int result = 0;
    for (int i = 0; i < d; i++)
        if (distance[i] != MAX_LONG_INT)
            result += distance[i];
    return result;
}

int main() {
    initNullNode();

    actualK = 0;
    full = '0';
    maxweight = -1;

    if (2 != scanf("%d %d", &d, &k)) return -1;


    int j;
    int graph[d][d];
    long int weight;


    int val;
    int result;
    char *pt;
    char input[STRLEN];
    char command[CMDLEN];

    while (1) {
        if (NULL == fgets(command, CMDLEN, stdin)) return 0;

        if (command[0] == 'A') {
            //ROUTINE graph
            for (int i = 0; i < d; i++) {
                pt = fgets(input, STRLEN, stdin);
                j = 0;
                pt--;
                val = 0;

                do {
                    pt++;
                    if ((*pt) == '\n' || (*pt) == ',') {
                        graph[i][j] = val;
                        val = 0;
                        j++;
                    } else {
                        val = val * 10 + ((*pt) - '0');
                    }
                } while ((*pt) != '\n');
            }

            weight = applyDijkstra(graph);

            // se non è piena la classifica aggiungi il nodo-id tranquillamente
            // dopo averlo aggiunto, vedi se la classifica è piena
            // magari potrebbe essere aumentato il peso massimo,
            if (full == '0') {
                insert(weight, actualK);
                actualK++;
                if (weight > maxweight)
                    maxweight = weight;
                if (actualK == k)
                    full = '1';
            } else { //if(full == '1')
                if (weight <
                    maxweight) { //se la classifica è piena e il grafo è pesante di più o uguale, non aggiungerlo
                    result = deleteMaxWeight(); // se result è uno, non cambiare maxweight. Se result è 0, devi trovare il nuovo maxweight
                    insert(weight, actualK);
                    if (result == 0) {
                        maxweight = (maximum(root))->weight;
                    }
                }
                actualK++;
            }

        } else if (command[0] == 'T') {
            printID();
        }
    }
}
