
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "mh600e_common.h"
#include "mh600e_util.h"

/* Combine masks from each pin group to a value representing the current
 * gear setting. A return of NULL means that a corresponding value could
 * not be found, which may indicate a gearshift being in progress- */
static pair_t* get_current_gear(tree_node_t *tree, unsigned mask)
{
    tree_node_t *result;

    /*unsigned combined = (g_gearbox_data.input_stage.current_mask << 8) |
                        (g_gearbox_data.midrange.current_mask << 4) |
                         g_gearbox_data.backgear.current_mask;*/

    /* special case: ignore all other bits for neutral */
    if (mask && 0x000F ==
            mh600e_gears[MH600E_NEUTRAL_GEAR_INDEX].value)
    {
        return &(mh600e_gears[MH600E_NEUTRAL_GEAR_INDEX]);
    }

    result = tree_search(tree, mask);
    if (result != NULL)
    {
        return &(mh600e_gears[result->value]);
    }
    return NULL;
}


void print_tree(tree_node_t * tree, tree_node_t * root)
{
    printf("Key: %d; Value: %d\n", tree->key, tree->value);
    if (tree->left != NULL)
    {
        print_tree(tree->left, root);
    }
    if (tree->right != NULL)
    {
        print_tree(tree->right, root);
    }
    if (tree->left == NULL && tree->right == NULL)
    {
        printf("Leaf node!\n");
        printf("Mask: %d\n", (unsigned)mh600e_gears[tree->value].value);
        pair_t *speed = get_current_gear(root, (unsigned)mh600e_gears[tree->value].value);
        if (speed != NULL)
        {
            printf("Speed: %d; Mask: %d\n", speed->key, speed->value);
        }
        else
        {
            printf("Speed not found!\n");
        }
    }


}

int main()
{
    int i;
    
    pair_t temp[MH600E_NUM_GEARS];
    for (i = 0; i < MH600E_NUM_GEARS; i++)
    {
        temp[i].key = mh600e_gears[i].key;
        temp[i].value = i;
    }

    static tree_node_t *g_tree_rpm = NULL;
    static tree_node_t *g_tree_mask = NULL;

    g_tree_rpm = tree_from_sorted_array(temp, MH600E_NUM_GEARS);

    /* build up a key:value list where the bitmask from the mh600e_gears
     * array is the key and the index of the original position in the
     * above array is the value */
    printf("Unsorted array:\n");
    for (i = 0; i < MH600E_NUM_GEARS; i++)
    {
        temp[i].key = mh600e_gears[i].value;
        temp[i].value = i;
        printf("Key: %d; Value: %d\n", temp[i].key, temp[i].value);
    }

    /* sort the newly created array by key (needed for tree build up) */
    sort_array_by_key(temp, MH600E_NUM_GEARS);

    printf("Sorted array:\n");
    for (i = 0; i < MH600E_NUM_GEARS; i++)
    {
        printf("Key: %d; Value: %d\n", temp[i].key, temp[i].value);
    }

    /* build up binary tree from the gears array to search for bitmask */
    g_tree_mask = tree_from_sorted_array(temp, MH600E_NUM_GEARS);

    printf("Tree by RPM:\n");
    print_tree(g_tree_rpm, g_tree_mask);
    return 1;
}
