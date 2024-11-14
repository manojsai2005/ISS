#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FOLDERS 256 // maximum number of folders/files in a single folder.
#define MAX_PATH 256

typedef struct trienode{
    struct trienode* childeren[MAX_FOLDERS];
    bool isLeaf;
    char* data;

} trienode;

trienode* root;

// allocate memory for the trienode.
trienode* CreateNode(char* data) {
    trienode* new_node = (trienode*)malloc(sizeof(trienode));
    for(int i = 0; i < MAX_FOLDERS; i++) new_node->childeren[i] = NULL;
    new_node->isLeaf = true;
    new_node->data = (char*)malloc(strlen(data)+1);
    strcpy(new_node->data, data);
    return new_node;
}

// split path into elements and store it in an array of strings.
void ParsePath(char* path, char** path_arr) {
    char* token = strtok(path, "/");
    int curr = 0;
    while(token != NULL) {
        char* path_element = (char*)malloc(strlen(token)+1);
        strcpy(path_element, token);
        path_arr[curr++] = path_element;
        token = strtok(NULL, "/");
    }
    path_arr[curr] = NULL;
}

// print the trie layer wise
// utility function to checkout the trie.
void PrintTrieBFS(trienode* root) {
    trienode* queue[MAX_FOLDERS];
    int front = 0;
    int rear = 0;
    int layer = 0;
    queue[rear++] = root;
    queue[rear++] = NULL; 

    while(front < rear) {
        trienode* curr = queue[front++];
        if (curr == NULL) {
            if (front < rear) {
                queue[rear++] = NULL;
            }
            layer++;
            printf("\n");
            continue;
        }
        printf("Layer %d: ", layer);
        for(int i = 0; i < MAX_FOLDERS; i++) {
            if(curr->childeren[i] != NULL) {
                printf("{data:%s childno:%d isleaf:%d} ", curr->childeren[i]->data, i, (int)(curr->isLeaf));
                queue[rear++] = curr->childeren[i];
            }
        }
        printf("\n");
    }
}

// function to add a path to the trie
//! All paths must begin with a "~/"
void AddPathToTrie(char* path, trienode* root) {
    char* path_arr[MAX_PATH];
    ParsePath(path, path_arr);
    trienode* curr = root;
    for(int i = 0; i < MAX_PATH; i++) {
        if(path_arr[i] == NULL) {
            break;
        }
        bool found = false;
        for(int j = 0; j < MAX_FOLDERS; j++) {
            if(curr->childeren[j] != NULL && strcmp(curr->childeren[j]->data, path_arr[i]) == 0) {
                found = true;
                curr = curr->childeren[j];
                break;
            }
        }
        if(!found) {
            trienode* new_node = CreateNode(path_arr[i]);
            for(int j = 0; j < MAX_FOLDERS; j++) {
                if(curr->childeren[j] == NULL) {
                    curr->childeren[j] = new_node;
                    curr->isLeaf = false;
                    break;
                }
            }
            curr = new_node;
        }
    }
}

int main() {
    root = CreateNode("~");
    char path1[MAX_PATH] = "~/folder1/folder2/file1";
    char path2[MAX_PATH] = "~/folder1/folder3/file2";
    AddPathToTrie(path1, root); // add path1 to trie 
    AddPathToTrie(path2, root); // add path2 to trie
    PrintTrieBFS(root); // print the trie layer wise
    return 0;
}