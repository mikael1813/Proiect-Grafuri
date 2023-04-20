#include<iostream> 
#include <list> 
#include <vector>
#include <fstream>

using namespace std;


ifstream f("plan.in");
ofstream out("plan.out");
int n, m;


class Graph
{
	int V; // Numarul nodurilor 
  
	list<int>* adj; // Lista muchiilor
public:
	Graph(int V); // Constructor 

	// adauga o muchie 
	void addMuchie(int v, int w);

	// Returneaza nodurile la care nodul s poate ajunge
	vector<int> BFS(int s);

	// Returneaza un vector de vectori iar fiecare vector din vectorul principal este o componenta conexa a grafului
	vector<vector<int>> CompConexe(int n);

	// Verifica daca din x exista un drum pana in y
	bool drum(int x, int y);

	// Returneaza un vector de vectori
	// Fiecare vector repr componentele conexe care au drum una de la cealalta (daca e cazul)
	vector<vector<int>> Inlantuiri(int n);

	// Verifica daca k apare intr-unul din vectorii lui lista
	bool gaseste(vector<vector<int>> lista, int k);

	// Returneaza vectorul cu drumurile ce trebuie construite si scrie in fisier nr de drumuri
	vector<int> DrumuriMinim();
};

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
}

void Graph::addMuchie(int v, int w)
{
	adj[v].push_back(w);  
}

vector<int> Graph::BFS(int s)
{
	vector<int> x;

	bool* vizitat = new bool[V];
	for (int i = 0; i < V; i++)
		vizitat[i] = false;

	list<int> queue;
 
	vizitat[s] = true;
	queue.push_back(s);

	list<int>::iterator i;

	while (!queue.empty())
	{
		
		s = queue.front();
		x.push_back(s);
		queue.pop_front();

		for (i = adj[s].begin(); i != adj[s].end(); ++i)
		{
			if (!vizitat[*i])
			{
				vizitat[*i] = true;
				queue.push_back(*i);
			}
		}
	}

	return x;
}

vector<vector<int>> Graph::CompConexe(int n) {
	vector<vector<int>> componente;
	bool* vizitat = new bool[n];

	for (int i = 0; i < n; i++) vizitat[i] = false;

	for (int i = 0; i < n; i++) {
		if (vizitat[i] == false) {
			vector<int> component;
			component.push_back(i);
			vizitat[i] = true;
			vector<int> v = BFS(i);
			for (auto j : v) {
				if (j != i) {
					vector<int> v2 = BFS(j);
					bool ok = false;
					for (auto k : v2) {
						if (k == i) ok = true;
					}
					if (ok) {
						component.push_back(j);
						vizitat[j] = true;
					}
				}
			}

			componente.push_back(component);
		}
	}

	return componente;
}

bool Graph::drum(int x, int y) {
	vector<int> v = BFS(x);
	for (auto i : v) {
		
		if (i == y) return true;
	}
	return false;
}

vector<vector<int>> Graph::Inlantuiri(int n) {
	vector<vector<int>> componente = CompConexe(n);

	vector<vector<int>> lista;

	for (int i = 0; i < componente.size(); i++) {
		bool ok = false;
		for (int j = 0; j < componente.size(); j++) {
			
			if (i!=j && drum(componente.at(i).at(0), componente.at(j).at(0))) {
				ok = true;
				if (lista.size() == 0) {
					vector<int>x{ i,j };
					lista.push_back(x);
				}
				else
					if (!gaseste(lista, i)) {
						vector<int> q{ i };
						lista.push_back(q);
					}
					for (auto& k : lista) {
						if (k.at(0) == j) {
							k.insert(k.begin(), i);
						}
						else if (k.at(k.size()-1) == i) {
							k.push_back(j);
						}
						else {
							for (int x = 0; x < k.size() - 1; x++) {
								if (k.at(x) == i && drum(componente.at(j).at(0), k.at(x + 1)) && k.at(x + 1) != j) {
									k.insert(k.begin() + x + 1, j);
									break;
								}
								else if (k.at(x) == j && drum(k.at(x - 1), componente.at(i).at(0)) && k.at(x - 1) != i) {
									k.insert(k.begin() + x - 1 , i);
									lista.pop_back();
									break;
								}
							}
						}
				}
			}
		}
		if (!ok && !gaseste(lista,i)) {
			vector<int> x{ i };
			lista.push_back(x);
		}
	}

	return lista;
}


bool Graph::gaseste(vector<vector<int>> lista, int k) {
	for (auto i : lista) {
		for (auto j : i) {
			if (j == k) return true;
		}
	}
	return false;
}

vector<int> Graph::DrumuriMinim() {

	vector<vector<int>> lista = Inlantuiri(n);

	vector<vector<int>> componente = CompConexe(n);

	if (lista.size() == 1) {
		vector<int> v {componente.at(lista.at(0).at(lista.at(0).size() - 1)).at(0), componente.at(lista.at(0).at(0)).at(0)};
		cout <<1 << endl;
		return v;
	}
	else {
		int sum = lista.size();
		vector<int> q;
		for (int i = 0; i < lista.size() - 1; i++) {
			q.push_back(componente.at(lista.at(i).at(lista.at(i).size() - 1)).at(0));
			q.push_back(componente.at(lista.at(i + 1).at(0)).at(0));
		}
		q.push_back(componente.at(lista.at(lista.size() - 1).at(0)).at(0));
		q.push_back(componente.at(lista.at(0).at(0)).at(0));
		out <<sum << endl;
		return q;
	}
}


int main()
{
	f >> n >> m;
	Graph g(n);

	int x, y;

	for (int i = 0; i < m; i++) {
		f >> x >> y;
		g.addMuchie(x, y);
		
	}


	vector<int> c = g.DrumuriMinim();

	if (c.size() == 1) {
		out << c.at(0) << " " << c.at(1) << endl;;
	}
	else
	for (int i = 0; i < c.size() - 1;i=i+2) {
		out << c.at(i) << " " << c.at(i + 1) << endl;
	}

	f.close();
	out.close();

	return 0;
}
