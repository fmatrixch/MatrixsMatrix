#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

//Must be compiled with a C++-compatible compiler; use of struct as class in C is strictly prohibited.

struct Mapping { int connection; char* weight; };
enum STATUS { ARRAY = 1,LINKEDLIST = 2,TREE = 4,GRAPH = 8 };
enum TREE_STATUS { COMP,INCO };
enum GRAPH_STATUS_COMPLETE { COMPLETE,INCOMPLETE };
enum GRAPH_STATUS_KONIGSBERG { TRAIL,CIRCUIT,NORMAL };

struct MT {
		Mapping** matrix;
		bool* visited;
		int nodes,edges = 0;
		bool is_array = false, is_on = false, is_bi = false;

		MT(int x) : nodes(x) {	
			matrix = (Mapping**)malloc(sizeof(Mapping*) * nodes);
			visited = (bool*)malloc(sizeof(bool) * nodes);
		 	for (int i = 0;i < nodes;++i) {
				matrix[i] = (Mapping*)malloc(sizeof(Mapping) * nodes);
				for (int j = 0;j < nodes;++j){
					matrix[i][j].connection = 0;
					matrix[i][j].weight = NULL;
				}
				visited[i] = false;
			}	
		}
		~MT() {
			for (int i = 0;i < nodes;++i) {
			       	for (int j = 0;j < nodes;++j) free(matrix[i][j].weight);	
				free(matrix[i]); 
			}
			free(matrix);
			free(visited);
		}
		void markArray() { is_array = true; }
		void setWeight(Mapping& x,const char* s){
			free(x.weight);
			if (s && *s){
				x.weight = (char*)malloc(strlen(s) + 1);
				strcpy(x.weight,s);
			}else{
				x.weight = (char*)malloc(1);
				x.weight[0] = '\0';
			}
		}
		void fillNodes(){
			markArray();
			char buffer[256];
			for (int x = 0;x < nodes;x++) {
				printf("Input node #%d: ",x);
				scanf("%255s",buffer);	
				while (getchar() != '\n');
				setWeight(matrix[x][0],buffer);
			}
		}
		void newEdge_lr(int x,int y,const char* i) { 
			is_on = true; 
			(matrix[x][y].connection)++; 
			setWeight(matrix[x][y],i);
			edges++; 
		}
		void newEdge_bi(int x,int y,const char* i) {
			is_bi = true;
			(matrix[x][y].connection)++;
			setWeight(matrix[x][y],i);
			(matrix[y][x].connection)++;
			setWeight(matrix[y][x],i);
			edges += 2;
		}
		void out(){
			printf("ADJACENCY MATRIX (CONNECTIONS)\n");
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++) printf("%d ",matrix[x][y].connection);
				printf("\n");
			}
			printf("ADJACENCY MATRIX (WEIGHTS)\n");
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++) {
				       if (matrix[x][y].weight != NULL) printf("%s ",matrix[x][y].weight);
				       else printf(" "); 
				}
				printf("\n");
			}
		}
		bool linkedTest(){
			if (is_array) return false;
			int check = is_bi ? (2*(nodes - 1)) : (nodes - 1);
			if (edges != check) return false;
			for (int x = 0;x < nodes - 1;x++){
				if ((matrix[x][x + 1].connection) != 1) return false;
			}
			return true;
		}
		int checkMatrixStatus(){
			int result = 0;
			if (is_array) result |= ARRAY;
			if (linkedTest()) result |=  LINKEDLIST;
			if ((is_on) && (edges == (nodes - 1))) result |= TREE;
			if (result == 0) result |= GRAPH;
			return result;
		}
		void outArray(){
			for (int x = 0;x < nodes;x++) printf("%d ",x);
			printf("\n");
			for (int x = 0;x < nodes;x++) { 
				if (matrix[x][0].weight != NULL) printf("%s ",matrix[x][0].weight);
			       	else printf(" ");	
			}
			printf("\n");
		}
		void outLinkedList(){
			for (int x = 0;x < nodes - 1;x++) printf("%d - ",x);
			printf("END\n");
			for (int x = 0;x < nodes - 1;x++) { 
				if (matrix[x][x + 1].weight != NULL) printf("%s - ",matrix[x][x + 1].weight);
				else printf(" - ");
		       	}
			printf("END\n");
		}
		void DFS(int x){
			visited[x] = true;
			printf("%d ",x);
			for (int y = 0;y < nodes;y++){
				if (matrix[x][y].connection > 0 && !(visited[y])) DFS(y);
			}
		}
		enum TREE_STATUS checkTreeStatus(){
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++){
					if (matrix[x][y].connection > 0 && y != ((2*x) + 1) && y != ((2*x) + 2)) return INCO;
				}
			}
			int end = -1;
			for (int x = 0;x < nodes;x++){
				if ((visited[x]) && (x >= end)) end = x;
			}
			if (end % 2 != 0) return INCO;
			return COMP;
		}
		enum GRAPH_STATUS_COMPLETE checkGraphStatusComplete(){
			if (is_bi) { if (edges == (nodes * (nodes - 1))) return COMPLETE; }
			else { if (edges == ((nodes * (nodes - 1)) / 2)) return COMPLETE; }
			return INCOMPLETE;
		}
		enum GRAPH_STATUS_KONIGSBERG checkGraphStatusKonigsberg(){
			int odd = 0, even = 0;
			for (int x = 0;x < nodes;x++){
				int number = 0;
				for (int y = 0;y < nodes;y++) {
				       	number+=(matrix[x][y].connection);
			       		if (!is_bi) number+=(matrix[y][x].connection);	
				}
				if (number % 2 == 0) even++;
				else odd++;
			}
			if (odd == 0) return CIRCUIT; 
			else if (odd == 2) return TRAIL;
			return NORMAL;
		}
};

void interface(){
	int n;
	printf("Enter a designated number of nodes: ");
	scanf("%d",&n);
	while (getchar() != '\n');
	MT example(n);
	char selection;
	printf("Select input mode\n(A - Bidirectional\n(B - Left-to-right\n(C - Node-only\nPress any other key to jump input\n");
	scanf("%c",&selection);
	while (getchar() != '\n');
	if (selection == 'A'){
		while (true){
			int i,j;
			char k[256];
			printf("Input your edge (-1 -1 -1 to end; to skip the third input please input -3): ");
			scanf("%d %d %255s",&i,&j,k);
			while (getchar() != '\n');
			if (i >= 0 && j >= 0 && strcmp(k,"-3") == 0) example.newEdge_bi(i,j,"");
			else if (i >= 0 && j >= 0 && strcmp(k,"-1") != 0) example.newEdge_bi(i,j,k);
			else break;
		}	
	}else if (selection == 'B'){
		while (true){
			int i,j;
			char k[256];
			printf("Input your edge (-1 -1 -1 to end; to skip the third input please input -3): ");
			scanf("%d %d %255s",&i,&j,k);
			while (getchar() != '\n');
			if (i >= 0 && j >= 0 && strcmp(k,"-3") == 0) example.newEdge_lr(i,j,"");
			else if (i >= 0 && j >= 0 && strcmp(k,"-1") != 0) example.newEdge_lr(i,j,k);
			else break;
		}
	}else if (selection == 'C') example.fillNodes();	
	else { printf("Jumped input. \nYour matrix won't have any edge.\n"); example.markArray(); }
	printf("Complete matrix: \n");
	example.out();
	int result = example.checkMatrixStatus();
	if (result & ARRAY) {
		printf("This matrix represents an array.\nOutput:\n");
		example.outArray();
	}	
	if (result & LINKEDLIST) {
	       	printf("This matrix represents a linked list.\nOutput:\n");
		example.outLinkedList();
	}	
	if (result & TREE) { 
		printf("This matrix represents a tree.\nOutput with pre-order DFS: ");
		example.DFS(0);	
		printf("\nThis tree ");
		enum TREE_STATUS t = example.checkTreeStatus();
		if (t == COMP) printf("is complete.\n");
		else printf("is not complete.\n");
	}
	if (result & GRAPH) printf("This matrix represents a graph.\n"); 
	enum GRAPH_STATUS_COMPLETE c = example.checkGraphStatusComplete();
	enum GRAPH_STATUS_KONIGSBERG k = example.checkGraphStatusKonigsberg();
	printf("This matrix ");
	if (c == COMPLETE) printf("is complete.\n");
	else printf("is not complete.\n");
	printf("This matrix ");
	if (k == TRAIL) printf("is an Eulerian trail.\n");
	else if (k == CIRCUIT) printf("is an Eulerian circuit.\n");
	else if (k == NORMAL) printf("is neither an Eulerian trail nor an Eulerian circuit.\n");
}

int main()
{
	interface();
	return 0;
}
