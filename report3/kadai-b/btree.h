typedef struct tnode{

    int data;
    int flag;
    struct tnode *left;
    struct tnode *right;
} Tnode;

Tnode *btree_create();
void btree_insert(int v, Tnode *t);
void btree_destroy(Tnode *t);
void btree_dump(Tnode *t);

