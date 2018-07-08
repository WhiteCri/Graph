#pragma once
#include <iostream>
#include <deque>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <functional>


////////////////////
template <typename Keytype_, typename Weighttype_ = int>
class Graph {
	//inner class definition //
public:
	/* vertex definition*/
	class Vertex {
	public:
		Vertex(Keytype_ label, int cost = 0) : label(label), cost(cost) {}

		//operator overloading
		friend std::ostream& operator<<(std::ostream& os, const Vertex& vtx) {
			std::cout << vtx.label << '[' << vtx.cost << ']';
			return os;
		}
		bool operator==(const Vertex& vtx) { return (label == vtx.label); }
		bool operator==(const Keytype_& name) { return (label == name); }
		bool operator!=(const Vertex& vtx) { return (!(*this == vtx)); }
		bool operator!=(const Keytype_& name) { return (!(*this == name)); }
		bool operator<(const Vertex vtx) { return label < vtx.label; }
		operator Keytype_() const { return label; }
		Keytype_ label;
		int cost;
	};

	/* edge definition*/
	class Edge {
	public:
		Edge(Keytype_ from, Keytype_ to, int weight = 0)
			: from(from), to(to), weight(weight) {}
		friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
			os << '(' << edge.from << "->" << edge.to << ')' << "{" << edge.weight << "}";
			return os;
		}
		bool operator==(const Edge& edge) { return ((from == edge.from) && (to == edge.to)); }
		bool operator!=(const Edge& edge) { return (!(*this == edge)); }
		Keytype_ from, to;
		Weighttype_ weight;
	};
public:
	typedef typename std::vector<Vertex> VertexList;
	typedef typename std::vector<Edge> EdgeList;
	typedef std::map<Vertex, std::vector<Edge>, std::function<
		bool(const Vertex& lhs, const Vertex& rhs)
	> > GraphMap;
public:
	//vtx member function
	void addVtx(Keytype_ name, Weighttype_ cost = 0) {
		Vertex target(name);
		graph[target] = std::vector<Edge>();
	}

	void eraseVtx(Keytype_ name) /* Erase Vertex include the edges that contain that name */
	{
		graph.erase(Vertex(name));
	}

	bool hasVtx(Keytype_ name) {
		return graph.count(Vertex(name));
	}

	VertexList getAdjVertex(Keytype_ name) {//the result ary is sorted
		auto edgelist = getAdjEdge(name);
		if (!edgelist.size()) return std::vector<Vertex>();//empty adjEdge list

		VertexList vlist;
		for (auto& edge : edgelist)
			vlist.emplace_back(edge.to);
		return vlist;
	}

	VertexList getVtxList() {
		VertexList vlist;
		for (auto& element : graph)
			vlist.emplace_back(element.first);
		return vlist;
	}

	//edge member function
	void addEdge(Keytype_ from, Keytype_ to, Weighttype_ weight) {
		auto printErr = [](Keytype_ name) {
			std::cout << "No Vertex " << name << "!" << std::endl;
			return;
		};
		//check two vertex exist.
		auto fromIter = graph.find(Vertex(from));
		auto toIter = graph.find(Vertex(to));
		if (fromIter == graph.end()) printErr(from);
		if (toIter == graph.end()) printErr(to);

		//add edge. because the graph has no direction, I add two edges.
		fromIter->second.emplace_back(from, to, weight);
		toIter->second.emplace_back(to, from, weight);
	}

	EdgeList getAdjEdge(Keytype_ name) {
		if (graph.count(Vertex(name))) return graph[Vertex(name)];
		return std::vector<Edge>();
	}

	int getWeight(Keytype_ from, Keytype_ to) {
		auto eleIter = graph.find(Vertex(from));
		if (eleIter == graph.end()) { return -1; }
		if (!graph.count(Vertex(to))) { return -1; }; //there's no vertex 'to'

		auto& elist = eleIter->second;
		for (auto edge : elist)
			if (edge.to == to)
				return edge.weight;

		return -1;
	}

	EdgeList getEdgeList() {//the result ary is sorted by key.
		EdgeList elist;
		for (auto& element : graph)
			for (auto& edge : element.second)
				elist.push_back(edge);
		return elist;
	}

	//graph member function
	void printGraph() {
		for (auto& vtxpair : graph) {
			std::cout << vtxpair.first << " : ";
			for (auto& edge : vtxpair.second)
				std::cout << edge << ", ";
			std::cout << std::endl;
		}
	}

	//Constructor
	Graph(){
		auto cmp = [](const Vertex& lhs, const Vertex& rhs)->bool {
			return lhs.label < rhs.label;
		};
		graph = GraphMap(cmp);
	}

	//operator overloading
	Graph operator-(Graph& rhs) {
		Graph re(*this);//copy self
		auto vlist = rhs.getVtxList();
		for (auto& vtx : vlist)
			re.eraseVtx(vtx);
		return re;
	}

	bool operator==(const Graph& rhs) {//the definition of same is : IF GRAPHs HAS SAME VTX SET
		Graph rhs_ = rhs;//copy

		//check size
		if (graph.size() != rhs.graph.size()) return false;

		//check vtx list
		auto this_vtxlist = getVtxList();
		auto rhs_vtxlist = rhs_.getVtxList();
		for (unsigned i = 0; i < this_vtxlist.size(); ++i)
			if (this_vtxlist[i] != rhs_vtxlist[i])
				return false;
		return true;
	}

	bool operator!=(const Graph& rhs) {
		return (!(*this == rhs));
	}

	friend std::ostream& operator<<(std::ostream& os, const VertexList& vtxList) {
		for (auto& vtx : vtxList) 
			std::cout << vtx << ' ';
		return os;
	}
	
	friend std::ostream& operator<<(std::ostream& os, VertexList&& vtxList) {
		for (auto& vtx : vtxList)
			std::cout << vtx << ' ';
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, const EdgeList& edgeList) {
		for (auto& edge : edgeList)
			std::cout << edge << ' ';
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, EdgeList&& edgeList) {
		for (auto& edge : edgeList)
			std::cout << edge << ' ';
		return os;
	}

private:
	GraphMap graph;
};