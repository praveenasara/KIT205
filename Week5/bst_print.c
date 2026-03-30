#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "bst_print.h"
#include <cstddef>

/* ================================================
   PRINT VISUAL (hierarchy / ASCII tree)
   ================================================ */

static void print_bst_helper(BSTNodePtr node, char* prefix,
    int has_sibling, int is_right_child)
{
    char new_prefix[1024];
    if (node == NULL) return;

    printf("%s%s[%c] %d\n",
        prefix,
        has_sibling ? "|-- " : "`-- ",
        is_right_child ? 'R' : 'L',
        node->data_item);

    snprintf(new_prefix, sizeof(new_prefix), "%s%s",
        prefix, has_sibling ? "|   " : "    ");

    if (node->left && node->right) {
        print_bst_helper(node->left, new_prefix, 1, 0);
        print_bst_helper(node->right, new_prefix, 0, 1);
    }
    else if (node->left) { print_bst_helper(node->left, new_prefix, 0, 0); }
    else if (node->right) { print_bst_helper(node->right, new_prefix, 0, 1); }
}

void print_bst_visual(BST* self)
{
    printf("\n=== BST Structure ===\n");
    if (self->root == NULL) {
        printf("  (empty)\n");
    }
    else {
        printf("[root] %d\n", self->root->data_item);
        if (self->root->left && self->root->right) {
            print_bst_helper(self->root->left, "", 1, 0);
            print_bst_helper(self->root->right, "", 0, 1);
        }
        else if (self->root->left) { print_bst_helper(self->root->left, "", 0, 0); }
        else if (self->root->right) { print_bst_helper(self->root->right, "", 0, 1); }
    }
    printf("=====================\n");
}

/* ================================================
   PRINT 2D - proper spatial tree layout
   ================================================ */

#define HSPACING  4
#define CANVAS_W  300
#define CANVAS_H  64

typedef struct {
    int val;
    int depth;
    int col;
    int left_col;
    int right_col;
} NodeLayout;

static NodeLayout nls[256];
static int        nl_count;
static int        col_counter;
static char       canvas[CANVAS_H][CANVAS_W];

static int collect(BSTNodePtr n, int depth)
{
    int lc, mc, rc;
    if (!n) return -1;
    lc = collect(n->left, depth + 1);
    mc = col_counter++;
    rc = collect(n->right, depth + 1);
    nls[nl_count].val = n->data_item;
    nls[nl_count].depth = depth;
    nls[nl_count].col = mc;
    nls[nl_count].left_col = lc;
    nls[nl_count].right_col = rc;
    nl_count++;
    return mc;
}

static void put_char(int x, int y, char c)
{
    if (x >= 0 && x < CANVAS_W - 1 && y >= 0 && y < CANVAS_H)
        canvas[y][x] = c;
}

static void put_str(int x, int y, const char* s)
{
    int i;
    for (i = 0; s[i] && x + i < CANVAS_W - 1; i++)
        if (y >= 0 && y < CANVAS_H)
            canvas[y][x + i] = s[i];
}

void print_bst_2d(BST* self)
{
    int  i, r, x, end, px, py, by, len, lx, rx, max_x, max_y;
    char tmp[16];

    if (!self->root) {
        printf("  (empty)\n===================\n");
        return;
    }

    nl_count = col_counter = 0;
    for (i = 0; i < CANVAS_H; i++) {
        memset(canvas[i], ' ', CANVAS_W - 1);
        canvas[i][CANVAS_W - 1] = '\0';
    }

    collect(self->root, 0); {
        int  i, r, x, end, px, py, by, len, lx, rx, max_x, max_y;
        char tmp[16];

        if (!self->root) {
            printf("  (empty)\n===================\n");
            return;
        }

        nl_count = col_counter = 0;
        for (i = 0; i < CANVAS_H; i++) {
            memset(canvas[i], ' ', CANVAS_W - 1);
            canvas[i][CANVAS_W - 1] = '\0';
        }

        collect(self->root, 0);

        max_x = 0;
        max_y = 0;

        /* First pass: underscores and pipes */
        for (i = 0; i < nl_count; i++) {
            px = nls[i].col * HSPACING;
            py = nls[i].depth * 2;
            by = py + 1;
            len = snprintf(tmp, sizeof(tmp), "%d", nls[i].val);

            /* Left child: underscores left of node, pipe below child */
            if (nls[i].left_col >= 0) {
                lx = nls[i].left_col * HSPACING;
                for (x = lx; x < px; x++)
                    put_char(x, py, '_');
                put_char(lx, by, '|');
                if (by > max_y) max_y = by;
            }

            /* Right child: underscores right of node, pipe below child */
            if (nls[i].right_col >= 0) {
                rx = nls[i].right_col * HSPACING;
                for (x = px + len; x <= rx; x++)
                    put_char(x, py, '_');
                put_char(rx, by, '|');
                if (rx + 1 > max_x) max_x = rx + 1;
                if (by > max_y) max_y = by;
            }
        }

        /* Second pass: node values printed on top of underscores */
        for (i = 0; i < nl_count; i++) {
            px = nls[i].col * HSPACING;
            py = nls[i].depth * 2;
            len = snprintf(tmp, sizeof(tmp), "%d", nls[i].val);
            put_str(px, py, tmp);
            if (px + len > max_x) max_x = px + len;
            if (py > max_y) max_y = py;
        }

        for (r = 0; r <= max_y; r++) {
            end = max_x;
            while (end > 0 && canvas[r][end - 1] == ' ') end--;
            canvas[r][end] = '\0';
            printf("%s\n", canvas[r]);
        }
        printf("\n===================\n");
    }
}

/* ================================================
   TRAVERSALS
   ================================================ */

static void preorder_node(BSTNodePtr self)
{
    if (self == NULL) return;
    printf("%d ", self->data_item);
    preorder_node(self->left);
    preorder_node(self->right);
}

static void inorder_node(BSTNodePtr self)
{
    if (self == NULL) return;
    inorder_node(self->left);
    printf("%d ", self->data_item);
    inorder_node(self->right);
}

static void postorder_node(BSTNodePtr self)
{
    if (self == NULL) return;
    postorder_node(self->left);
    postorder_node(self->right);
    printf("%d ", self->data_item);
}

void preorder_bst(BST* self)
{
    printf("Pre-order  (root->L->R): ");
    preorder_node(self->root);
    printf("\n");
}

void inorder_bst(BST* self)
{
    printf("In-order   (L->root->R): ");
    inorder_node(self->root);
    printf("\n");
}

void postorder_bst(BST* self)
{
    printf("Post-order (L->R->root): ");
    postorder_node(self->root);
    printf("\n");
}