

struct HyperPlane {
    vector<float> coefficients;
}

struct BSPTreeNode {
    List polygons;
    HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
}

struct BSPTree {
    BSPTreeNode* root;
}

BSPTree*
BuildBSPTree(List polygons) {
    result = new BSPTree;
    result->root = BuildBSPTreeNode(polygons);
    return result;
}

BSPTreeNode* BuildBSPTreeNode(list polygons) {
    if (polygons.IsEmpty())
        return NULL;

    BSPTreeNode* tree = new BSPTreeNode;
    polygon* root = polygons.GetFromList();

    tree->partition = root->GetHyperPlane();
    tree->polygons.AddToList(root);

    list front_list, back_list;

    polygon* poly;
    while ((poly = polygons.GetFromList()) != 0) {
        int result = tree->partition.ClassifyPolygon(poly);
        switch (result) {
            case COINCIDENT:
                tree->polygons.AddToList(poly);
                break;
            case IN_BACK_OF:
                back_list.AddToList(poly);
                break;
            case IN_FRONT_OF:
                front_list.AddToList(poly);
                break;
            case SPANNING:
                polygon *front_piece, *back_piece;
                SplitPolygon(poly, tree->partition, front_piece, back_piece);
                back_list.AddToList(back_piece);
                front_list.AddToList(front_piece);
                break;
        }
    }
    tree->front = BuildBSPTreeNode(front_list);
    tree->back = BuildBSPTreeNode(back_list);
}

void DrawBSPTree(BSP_tree* tree, point eye) {
    if (tree == NULL)
        return;
    real result = tree->partition.ClassifyPoint(eye);
    if (result > 0) {
        DrawBSPTree(tree->back, eye);
        tree->polygons.DrawPolygons();
        DrawBSPTree(tree->front, eye);
    } else if (result < 0) {
        DrawBSPTree(tree->front, eye);
        tree->polygons.DrawPolygonList();
        DrawBSPTree(tree->back, eye);
    } else {
        // the eye point is on the partition plane...
        DrawBSPTree(tree->front, eye);
        DrawBSPTree(tree->back, eye);
    }
}