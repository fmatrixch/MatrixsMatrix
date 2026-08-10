#include <iostream>
#include <string>
using namespace std;

struct Mapping { int connection; string weight; };
enum STATUS { ARRAY,LINKEDLIST,TREE,GRAPH };
enum TREE_STATUS { COMP,INCO };
enum GRAPH_STATUS_COMPLETE { COMPLETE,INCOMPLETE };
enum GRAPH_STATUS_KONIGSBERG { TRAIL,CIRCUIT,NORMAL };

class MT {
	private:
		Mapping** matrix;
		bool* visited;
		int nodes,edges = 0;
		bool is_array = false, is_on = false, is_bi = false;
	public:
		MT(int x) : nodes(x) {	
			matrix = new Mapping*[nodes];
			visited = new bool[nodes];
		 	for (int i = 0;i < nodes;++i) matrix[i] = new Mapping[nodes]();	
		}
		~MT() {
			for (int i = 0;i < nodes;++i) delete[] matrix[i];
			delete[] matrix;
			delete[] visited;
		}
		void markArray() { is_array = true; }
		void fillNodes(){
			markArray();
			for (int x = 0;x < nodes;x++) {
				cout << "Input node #" << x << ": ";	
				cin >> matrix[x][0].weight;
			}
		}
		void newEdge_lr(int x,int y,string i) { 
			is_on = true; 
			(matrix[x][y].connection)++; 
			matrix[x][y].weight = i;
			edges++; 
		}
		void newEdge_bi(int x,int y,string i) {
			is_bi = true;
			(matrix[x][y].connection)++;
			matrix[x][y].weight = i;
			(matrix[y][x].connection)++;
			matrix[y][x].weight = i;
			edges += 2;
		}
		void out(){
			cout << "ADJACENCY MATRIX (CONNECTIONS)" << endl;
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++) cout << matrix[x][y].connection << " ";
				cout << endl;
			}
			cout << "ADJACENCY MATRIX (WEIGHTS)" << endl;
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++) cout << matrix[x][y].weight << " ";
				cout << endl;
			}
		}
		bool linkedTest(){
			if ((is_array or is_bi) and (!is_on)) return false;
			if (edges != (nodes - 1)) return false;
			for (int x = 0;x < nodes - 1;x++){
				if ((matrix[x][x + 1].connection) != 1) return false;
			}
			return true;
		}
		enum STATUS checkMatrixStatus(){
			if (is_array) return ARRAY;
			else if (linkedTest()) return LINKEDLIST;
			else if (is_on and edges == (nodes - 1)) return TREE;
			else return GRAPH;
		}
		void outArray(){
			for (int x = 0;x < nodes;x++) cout << x << " ";
			cout << endl;
			for (int x = 0;x < nodes;x++) cout << matrix[x][0].weight << " ";
			cout << endl;
		}
		void outLinkedList(){
			for (int x = 0;x < nodes - 1;x++) cout << x << " -> ";
			cout << "END" << endl;
			for (int x = 0;x < nodes - 1;x++) cout << matrix[x][x + 1].weight << " -> ";
			cout << "END" << endl;
		}
		void DFS(int x){
			visited[x] = true;
			cout << x << " ";
			for (int y = 0;y < nodes;y++){
				if (matrix[x][y].connection > 0 and !(visited[y])) DFS(y);
			}
		}
		enum TREE_STATUS checkTreeStatus(){
			for (int x = 0;x < nodes;x++){
				for (int y = 0;y < nodes;y++){
					if (matrix[x][y].connection > 0 and y != ((2*x) + 1) and y != ((2*x) + 2)) return INCO;
				}
			}
			int end = -1;
			for (int x = 0;x < nodes;x++){
				if ((visited[x]) and (x >= end)) end = x;
			}
			if (end % 2 != 0) return INCO;
			return COMP;
		}
		enum GRAPH_STATUS_COMPLETE checkGraphStatusComplete(){
			if (is_bi) { if (edges == (nodes * (nodes - 1))) return COMPLETE; }
			if (edges == ((nodes * (nodes - 1)) / 2)) return COMPLETE;
			return INCOMPLETE;
		}
		enum GRAPH_STATUS_KONIGSBERG checkGraphStatusKonigsberg(){
			int odd = 0, even = 0;
			for (int x = 0;x < nodes;x++){
				int number = 0;
				for (int y = 0;y < nodes;y++){
					if ((matrix[x][y]).connection > 0) number+=(matrix[x][y].connection);
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
	cout << "Enter a designated number of nodes: ";
	cin >> n;
	MT example(n);
	char selection;
	cout << "Select input mode:" << endl << "(A - Bidirectional" << endl << "(B - Left-to-right" << endl << "(C - Node-only" << endl << "Press any other key to jump input" << endl;
	cin >> selection;
	if (selection == 'A'){
		while (true){
			int i,j;
			string k;
			cout << "Input your edge (-1 -1 -1 to end; to skip the third input please input -3): ";
			cin >> i >> j >> k;
			if (i >= 0 and j >= 0 and k == "-3") example.newEdge_bi(i,j,"");
			else if (i >= 0 and j >= 0 and k != "-1") example.newEdge_bi(i,j,k);
			else break;
		}	
	}else if (selection == 'B'){
		while (true){
			int i,j;
			string k;
			cout << "Input your edge (-1 -1 -1 to end; to skip the third input please input -3): ";
			cin >> i >> j >> k;
			if (i >= 0 and j >= 0 and k == "-3") example.newEdge_lr(i,j,"");
			else if (i >= 0 and j >= 0 and k != "-1") example.newEdge_lr(i,j,k);
			else break;
		}
	}else if (selection == 'C') example.fillNodes();	
	else { cout << "Jumped input. " << endl << "Your matrix won't have any edge." << endl; example.markArray(); }
	cout << "Complete matrix: " << endl;
	example.out();
	enum STATUS result = example.checkMatrixStatus();
	cout << "This matrix ";
	if (result == ARRAY) {
		cout << "represents an array." << endl << "Output:" << endl;
		example.outArray();
	}	
	else if (result == LINKEDLIST) {
	       	cout << "represents a linked list." << endl << "Output:" << endl;
		example.outLinkedList();
	}	
	else if (result == TREE) { 
		cout << "represents a tree." << endl << "Output with pre-order DFS: ";
		example.DFS(0);	
		cout << endl << "This tree ";
		enum TREE_STATUS t = example.checkTreeStatus();
		if (t == COMP) cout << "is complete." << endl;
		else cout << "is not complete." << endl;
	}
	else if (result == GRAPH){
	       	cout << "represents a graph." << endl; 
		enum GRAPH_STATUS_COMPLETE c = example.checkGraphStatusComplete();
		enum GRAPH_STATUS_KONIGSBERG k = example.checkGraphStatusKonigsberg();
		cout << "This graph ";
		if (c == COMPLETE) cout << "is complete." << endl;
		else cout << "is not complete." << endl;
		cout << "This graph ";
		if (k == TRAIL) cout << "is an Eulerian trail." << endl;
		else if (k == CIRCUIT) cout << "is an Eulerian circuit." << endl;
		else if (k == NORMAL) cout << "is neither an Eulerian trail nor an Eulerian circuit." << endl;
	}
}

int main()
{
	interface();
	return 0;
}
