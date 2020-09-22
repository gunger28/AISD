#include "stdafx.h"
#include "locale.h"
#include "iostream"
#include "conio.h"
#include "stdlib.h"
#include "string"
#include "windows.h"
#include "vector"
#include "time.h"
#include "map"
#include "list"
#include "iomanip"
#include "limits"
#include "functional"
#include "queue"
#include <sstream>
#include <iostream>

using namespace std;

int sz=0;

string i_to_s(int x){
    char buffer[20];
    _itoa(x, buffer, 10);         // 10 - ��� ������� ���������
    string s = buffer; 
    return s;
}

int minimum(int x1, int x2){
	if (x1<x2)
		return x1;
	return x2;
};

//�������

template <class NameT, class DataT>
class Vertex {
    NameT name;     //��� �������
    DataT data;     //������ �������
public:
	 bool nameSet, dataSet;
    
	//������������
    Vertex() : nameSet(false), dataSet(false) {}
    Vertex(NameT name, DataT data) : name(name), data(data), nameSet(true), dataSet(true) {}
    
	//���������, ��������� ����� � ������
    void setName(NameT name) { 
		this->name = name; 
		nameSet = true; 
	}
    void setData(DataT data) { 
		this->data = data; 
		dataSet = true; 
	}
    NameT getName() { 
		return name; 
	}
    DataT getData() { 
		return data; 
	}
    bool isNameSet() { 
		return nameSet; 
	}
    bool isDataSet() { 
		return dataSet; 
	}
};
//====================================

template <class VertexT, class WeightT, class DataT>
class Edge {
    VertexT *v1, *v2;   //�������, ������� ��������� �����
    WeightT weight;     //���
    DataT data;         //������ ����� 
	bool weightSet, dataSet;
public:
    //������������
    Edge(VertexT *v1, VertexT *v2) : v1(v1), v2(v2), weightSet(false), weight(0), dataSet(false) {}
    Edge(VertexT *v1, VertexT *v2, WeightT weight) : v1(v1), v2(v2), weight(weight), weightSet(true), dataSet(false) {}
    Edge(VertexT *v1, VertexT *v2, WeightT weight, DataT data) : v1(v1), v2(v2), weight(weight), data(data), weightSet(true), dataSet(true) {}
    
	//��������� � ��������� ���� � ������
    void setWeight(WeightT weight) { 
		this->weight = weight; 
		weightSet = true; 
	}
    void setData(DataT data) { 
		this->data = data; 
		dataSet = true; 
	}
    WeightT getWeight() { 
		return weight; 
	}
    DataT getData() { 
		return data; 
	}
	VertexT* getVertex1(){
		return v1;
	};
	VertexT* getVertex2(){
		return v2;
	};
    bool isWeightSet() { 
		return weightSet; 
	}
    bool isDataSet() { 
		return dataSet; 
	}
};

//====================
// ������������� �����
template <class EdgeT>
class GraphForm {
public:
    //������� � �������� ������ � ����
    virtual bool insertVertex(int index) = 0;
    virtual bool deleteVertex(int index) = 0;
    virtual bool insertEdge(int v1, int v2, EdgeT *t) = 0;
    virtual bool deleteEdge(int v1, int v2) = 0;
    //������� �������� � ��������� �� ������� ����
    virtual int deleteEdgesFromVertex(int index, bool directed) = 0;
    //�������� � ���������
    virtual bool hasEdge(int v1, int v2) = 0;
    virtual EdgeT* getEdge(int v1, int v2) = 0;
};

//��������� �����

template <class EdgeT>
class GraphMatrixForm : public GraphForm<EdgeT> {
    friend class EdgeIterator;
    bool directed;
    vector<vector<EdgeT*>> matrix; //������� ���������
public:
    GraphMatrixForm(bool directed) : directed(directed) {}
    //������� � �������� ������ � ����
  
	bool insertVertex(int index) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(index < 0 || index > size) return false;
        //������ ����� ������ ������
        vector<EdgeT*> newLine;
        newLine.assign(size, NULL);
        //��������� ����� ������:
        matrix.insert(matrix.begin() + index, newLine);
        ++size;
        //��������� ����� �������:
        for(int i = 0; i < size; ++i)
            matrix[i].insert(matrix[i].begin() + index, (EdgeT*)NULL);
        return true;
    }
    bool deleteVertex(int index) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(index < 0 || index >= size) return false;
        //������� ������:
        matrix.erase(matrix.begin() + index);
        --size;
        //������� �������:
        for(int i = 0; i < size; i++)
            matrix[i].erase(matrix[i].begin() + index);
        return true;
    }
    bool insertEdge(int v1, int v2, EdgeT *t) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //����� ��� ����� ��� ����
        if(v1 == v2 || matrix[v1][v2] != NULL) return false;
        //��������� �����
        matrix[v1][v2] = t;
        return true;
    }
    bool deleteEdge(int v1, int v2) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //����� ���
        if(v1 == v2 || matrix[v1][v2] == NULL) return false;
        matrix[v1][v2] = NULL;
        return true;
    }
    //������� �������� � ��������� �� ������� ����
    int deleteEdgesFromVertex(int index, bool directed) {
        int size = matrix.size(); //����� ������
        int deleted = 0;
        //�������� ����� �������
        if(index < 0 || index >= size) return 0;
        //������� ��������� � �������� ����
        for(int i = 0; i < size; i++) {
            if(matrix[i][index] != NULL) {
                delete matrix[i][index];
                matrix[i][index] = NULL;
                ++deleted;
                //������� ������������ �����
                if(directed == false)
                    matrix[index][i] = NULL;
            }
            if(matrix[index][i] != NULL) {
                delete matrix[index][i];
                matrix[index][i] = NULL;
                ++deleted;
            }
        }
        return deleted;
    }
    //�������� � ���������
    bool hasEdge(int v1, int v2) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        if(v1 == v2) //�����
			return false;
        if(matrix[v1][v2] != NULL) 
			return true;
        return false;
    }
    EdgeT* getEdge(int v1, int v2) {
        int size = matrix.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			throw 1;
        if(v1 == v2 || matrix[v1][v2] == NULL)//����� 
			throw 1;
        return matrix[v1][v2];
    }

    //�������� ����

    class EdgeIterator {
        bool end;
        GraphMatrixForm *f;
    public:
		int curI, curJ;
        EdgeIterator(GraphMatrixForm *f) {
            this->f = f;
            begin();
        }
        bool begin() {
            for(int i = 0; i < f->matrix.size(); ++i)
                for(int j = 0; j < f->matrix.size(); ++j)
                    if(f->matrix[i][j]) {
                        curI = i;
                        curJ = j;
                        end = false;
                        return true;
                    }
					end = true;
					return false;
        }
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			for(int i=f->matrix.size()-1; i>=0; --i)
				for(int j=f->matrix.size()-1; j>=0; --j)
					if (f->matrix[i][j]){
						curI=i;
						curJ=j;
						end=false;
						return true;
					}
					end=true;
					return false;
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curI < f->matrix.size()) {
                while(curJ < f->matrix.size()) {
                    if(f->matrix[curI][curJ])
                        return true;
                    ++curJ;
                }
                ++curI;
                curJ = (f->directed ? 0 : curI + 1);
            }
            end = true;
            return false;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
			if (!(f->matrix[curI][curJ]))
				throw 1;
            return f->matrix[curI][curJ];
        }
    };

    //�������� ��������� ����
    
	class OutputEdgeIterator {
        int curI, curJ;
        bool end;
        GraphMatrixForm *f;
    public:
        OutputEdgeIterator(GraphMatrixForm *f, int vIndex) {
            this->f = f;
            curI = vIndex;
            begin();
        }
        bool begin() {
            for(curJ = 0; curJ < f->matrix.size(); ++curJ)
                if(f->matrix[curI][curJ]) {
                    end = false;
                    return true;
                }
            end = true;
            return false;
        }
		bool toend(){
			for(curJ=f->matrix.size()-1; curJ>=0; --curJ)
				if (f->matrix[curI][curJ]){
					end=false;
					return true;
				}
				end=true;
				return false;
		}
        bool onEnd() { 
			return end; 
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curJ < f->matrix.size()) {
                if(f->matrix[curI][curJ])
                    return true;
                ++curJ;
            }
            end = true;
            return false;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return f->matrix[curI][curJ];
        }
    };
};

//===========

//��������� �����

template <class EdgeT>
class GraphListForm : public GraphForm<EdgeT> {
public:
	//������� ������
	class Node {
	public:
		EdgeT* edge; //���� �����
		int v2;      //������ �������, ������� ����� ���������
	};
	vector<list<Node>> edgeList; //������ ���������
	bool directed;
	
	GraphListForm(bool directed) : directed(directed) {}
    //������� � �������� ������ � ����
    bool insertVertex(int index) {
        int size = edgeList.size(); //����� ������
        if(index < 0 || index > size) //�������� ����� �������
			return false;
        //������ ����� ������ ���������
        list<Node> newList;
        //���������
        edgeList.insert(edgeList.begin() + index, newList);
        ++size;
        //��������� �����������
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 >= index)//���� ������� ������� ����� ������� ��-���, ��� �����������,
                    ++((*j).v2);//�� ����������� ���� �����
        return true;
    }
    bool deleteVertex(int index) {
        int size = edgeList.size(); //����� ������
        if(index < 0 || index >= size)  //�������� ����� �������
			return false;
        //������� �� ������� ������ � �����
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 == index) {
                    edgeList[i].erase(j);
                    break;
                }
        //������� ������ ���������
        edgeList.erase(edgeList.begin() + index);
        --size;
        //��������� �����������
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 > index)//���� ������� ������� ����� ������� �����, ��� ���������,
                    --((*j).v2);//�� ��������� ���� �����
        return true;
    }
    bool insertEdge(int v1, int v2, EdgeT *t) {
        int size = edgeList.size(); //����� ������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)//�������� ����� ���-����
			return false;
        if(v1 == v2 || hasEdge(v1, v2))  //����� ��� ����� ��� ����
			return false;
        //��������� �����
        Node node;
        node.edge = t;
        node.v2 = v2;
        edgeList[v1].push_front(node);
        return true;
    }
    bool deleteEdge(int v1, int v2) {
        int size = edgeList.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        //����� ���
        if(v1 == v2 || hasEdge(v1, v2) == false) 
			return false;
        //������� �����
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2) {
                edgeList[v1].erase(j);
                break;
            }
        return true;
    }
    //������� �������� � ��������� �� ������� ����
    int deleteEdgesFromVertex(int index, bool directed) {
        int size = edgeList.size(); //����� ������
        int deleted = 0;
        //�������� ����� �������
        if(index < 0 || index >= size) 
			return 0;
        //������� ��������� � �������� ����
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 == index) {
                    delete (*j).edge;
                    edgeList[i].erase(j);
                    ++deleted;
                    //������� ������������ �����
                    if(directed == false)
                        for(list<Node>::iterator k = edgeList[index].begin(); k != edgeList[index].end(); ++k)
                            if((*k).v2 == i) {
                                edgeList[index].erase(k);
                                break;
							}
							break;
				}
	    if (directed==true){
			for(list<Node>::iterator z = edgeList[index].begin(); z != edgeList[index].end(); ++z){  
				delete (*z).edge; 
				deleted++;
			}
		}	
        return deleted;
    }
    //�������� � ���������
    bool hasEdge(int v1, int v2) {
        int size = edgeList.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        //�����
        if(v1 == v2) 
			return false;
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2)
                return true;
        return false;
    }
    EdgeT* getEdge(int v1, int v2) {
        int size = edgeList.size(); //����� ������
        //�������� ����� �������
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			throw 1;
        //�����
        if(v1 == v2) 
			throw 1;
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2)
                return (*j).edge;
        throw 1;
    }

    //�������� ����
    
	class EdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool end;
        GraphListForm *f;
    public:
        EdgeIterator(GraphListForm *f) {
            this->f=f;
            begin();       
		}
        bool begin() {
            for(curI = 0; curI < f->edgeList.size(); ++curI)
                for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                    if((*curJ).edge) {
                        end = false;
                        return true;                    
					}
            end = true;
            return false;        
		}
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			 typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			 for(curI = 0; curI < f->edgeList.size(); ++curI)
                for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
					if ((*curJ).edge)
						prev_curJ=curJ;
			 if (!(*prev_curJ).edge){
				 end=true;
				 return false;
			 }
			 else{
				 end=false;
				 curJ=prev_curJ;
				 return true;
			 };  
		};
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curI < f->edgeList.size()) {
                while(curJ != f->edgeList[curI].end()) {
                    if((*curJ).edge && (f->directed || !f->directed && (*curJ).v2 > curI))
                        return true;
                    ++curJ;                
				}
                ++curI;
                if(curI < f->edgeList.size())
                    curJ = f->edgeList[curI].begin();
            }
            end = true;
            return true;        
		}
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return (*curJ).edge;
        }
    };

    //�������� ��������� ����
    
	class OutputEdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool end;
        GraphListForm *f;
    public:
        OutputEdgeIterator(GraphListForm *f, int vIndex) {
            this->f = f;
            curI = vIndex;
            begin();
        }
        bool begin() {
            for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                if((*curJ).edge) {
                    end = false;
                    return true;                
				}
            end = true;
            return false;        
		}
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			 typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			 for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
				 if ((*curJ).edge)
					 prev_curJ=curJ;
			 if (!(*prev_curJ).edge){
				 end=true;
				 return false;
			 }
			 else{
				 end=false;
				 curJ=prev_curJ;
				 return true;
			 };  
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curJ != f->edgeList[curI].end()) {
                if((*curJ).edge)
                    return true;
                ++curJ;            
			}
            end = true;
            return true;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return (*curJ).edge;        
		}    
	};
};


//===============

//����

template <class VertexT, class EdgeT>
class Graph {
    friend class VertexIterator;
    vector<VertexT*> vertexVector;//������ �����������
    GraphForm<EdgeT> *data;       //������ �����
    bool directed;                //������������ ����
    bool dense;                   //�-����
    int edgeCounter;              //����� ����

    //��������� ������� ������� �� �����������
	int getIndexFromName(string n){
		int index=0;
		for(int i=0; i<vertexVector.size(); ++i){
			if (vertexVector[index]->getName()==n)
				break;
			++index;
		}
		if(index == vertexVector.size())
            throw "������ ������� ��� � �����";
        return index;
	};

public:
    //������������:
    //������ L ����
    Graph() {
        data = new GraphListForm<EdgeT>(false);
        directed = false;
        dense = false;
        edgeCounter = 0;
    }
    //������ ���-�� ������, ��������������, �����
    Graph(int vertexCount, bool directed, bool dense) {
        if(dense)
            data = new GraphMatrixForm<EdgeT>(directed);//!!!!!!!!!!!!1
        else
            data = new GraphListForm<EdgeT>(directed);//!!!!!!!!!!!!!!!!!!!!
        //������ ������� � �������� � ���������
        for(int i = 0; i < vertexCount; ++i)
            data->insertVertex(i);
        for(int i = 0; i < vertexCount; ++i) {
            VertexT *v = new VertexT();
			v->setName(i_to_s(sz));
			sz++;
            vertexVector.push_back(v);
            data->insertVertex(i);
        }
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;
    }
    //+ ������ ���-�� ��������� ����
    Graph(int vertexCount, int edgeCount, bool directed, bool dense) {
        if(dense)
            data = new GraphMatrixForm<EdgeT>(directed);
        else
            data = new GraphListForm<EdgeT>(directed);
        //������ ������� � �������� � ���������
        for(int i = 0; i < vertexCount; ++i)
            data->insertVertex(i);
        for(int i = 0; i < vertexCount; ++i) {
            VertexT *v = new VertexT(); 
			v->setName(i_to_s(sz));
			sz++;
            vertexVector.push_back(v);
            data->insertVertex(i);
        } 
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;

        //��������� ����
        if(edgeCount <= 0) return;
        //������������ ����� ����, ����� �� ��������� ���������
        if(directed) {
            if(edgeCount > vertexCount * (vertexCount - 1))
                edgeCount = vertexCount * (vertexCount - 1);
            srand(time(0));
        } else {
            if(edgeCount > (vertexCount * (vertexCount - 1)) / 2)
                edgeCount = (vertexCount * (vertexCount - 1)) / 2;
        }
        //���������
        int v1, v2;

        while(edgeCounter < edgeCount) {
            v1 = rand() % vertexCount;
            v2 = rand() % vertexCount;
            if(v1 == v2)
                continue;
            if(data->hasEdge(v1, v2))
                continue;

            EdgeT *t = new EdgeT(vertexVector[v1], vertexVector[v2]);
            data->insertEdge(v1, v2, t);
			t->setWeight((rand() % 9) +1);
            if(directed == false)
                data->insertEdge(v2, v1, t);
            ++edgeCounter;
        }
    }
    Graph(const Graph<VertexT, EdgeT> &G) {
        delete data;
        if(G.isDense())
            data = new GraphMatrixForm<EdgeT>(*(G.data));
        else
            data = new GraphListForm<EdgeT>(*(G.data));
        directed = G.directed;
        dense = G.dense;
    }
    ~Graph() {
		while (vertexVector.size()!=0)
			deleteVertex(vertexVector.size()-1);
        delete data;
    }
    //���������� � �����
    //����� ������, ����, ������������
    int getVertexCount() { 
		return vertexVector.size(); 
	}
    int getEdgeCount() { 
		return edgeCounter; 
	}
    double getRatio() {
        int max = vertexVector.size() * (vertexVector.size() - 1);
        if(!directed)
            max /= 2;
        return (double)edgeCounter / (double)max;
    }
    //��� �����
    bool isDirected() { 
		return directed; 
	}
    bool isDense() { 
		return dense; 
	}
    //��������������
    void toDenseGraph() {
        //������ ��������� �������� �-�����
		GraphForm<EdgeT> *newData = new GraphMatrixForm<EdgeT>(this->directed);

        //������ �������
        for(int i = 0; i < vertexVector.size(); ++i)
            newData->insertVertex(i);

        //��������� ����
        for(int i = 0; i < vertexVector.size(); ++i)
            for(int j = 0; j < vertexVector.size(); ++j)
                if(data->hasEdge(i, j))
                    newData->insertEdge(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = true;
    }
    void toSparseGraph() {
        //������ ��������� �������� L-�����
		GraphForm<EdgeT> *newData = new GraphListForm<EdgeT>(this->directed);

        //������ �������
        for(int i = 0; i < vertexVector.size(); ++i)
            newData->insertVertex(i);

        //��������� ����
        for(int i = 0; i < vertexVector.size(); ++i)
            for(int j = 0; j < vertexVector.size(); ++j)
                if(data->hasEdge(i, j))
                    newData->insertEdge(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = false;
    }


    //������
    //�������
    VertexT* insertVertex() {
        VertexT *v = new VertexT;
        if(data->insertVertex(vertexVector.size()) == false)
            throw 1;
		v->setName(i_to_s(sz));
		sz++;
        vertexVector.push_back(v);
        return v;
    }
    VertexT* insertVertex(int index) {
        VertexT *v = new VertexT;
        if(data->insertVertex(index) == false)
            throw 1;
        vertexVector.insert(vertexVector.begin() + index, v);
        return v;
    }
    bool deleteVertex(VertexT *v) {
        int index = getIndex(v);
        edgeCounter -= data->deleteEdgesFromVertex(index, directed);
        if(data->deleteVertex(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }
	bool deleteVertex(int index) {
        edgeCounter -= data->deleteEdgesFromVertex(index, directed);
        if(data->deleteVertex(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }
    bool deleteVertex(string n) {
		VertexT *v=new VertexT;
		bool rez;
		try{
			v=getVertexFromName(n);
		}
		catch(char*){
			return false;
		}
		rez=deleteVertex(v);
		return rez;
    }
    VertexT* getVertex(int index) {
        if(index < 0 || index >= vertexVector.size())
            throw "������ ������� ��� � �����";
        return vertexVector[index];
    }
	VertexT* getVertexFromName(string n){
		int i;
		for(i=0; i<vertexVector.size(); i++)
			if (vertexVector[i]->getName()==n)
				return vertexVector[i];
		if (i==vertexVector.size())
			throw "������ ������� ��� � �����";
	};
	int getIndex(VertexT *v) {
		int index = 0;
        for(int i = 0; i < vertexVector.size(); ++i) {
            if(vertexVector[index] == v)
                break;
            ++index;
        }
        if(index == vertexVector.size())
            throw "������ ������� ��� � �����";
        return index;
    }
    //и���
	void insertEdge(string v1, string v2){
		VertexT* _v1=new VertexT;
		VertexT* _v2=new VertexT;
		try{
			_v1=getVertexFromName(v1);
		}
		catch(char*){
			throw "�������� ��� ������� 1";
		};
		try{
			_v2=getVertexFromName(v2);
		}
		catch(char*){
			throw "�������� ��� ������� 2";
		}
		try{
			insertEdge(_v1, _v2);
		}
		catch (char* Error){
			throw "����� �����������, ��� ����� ��� ����";
		}
		return;
	};
    EdgeT* insertEdge(VertexT *v1, VertexT *v2) {
        EdgeT *e = new EdgeT(v1, v2);
        if(!data->insertEdge(getIndex(v1), getIndex(v2), e))
			throw "����� �����������, ��� ����� ��� ����";
        if(directed == false)
            data->insertEdge(getIndex(v2), getIndex(v1), e);
        ++edgeCounter;
        return e;
    }
	bool deleteEdge(string v1, string v2){
		VertexT* _v1=new VertexT;
		VertexT* _v2=new VertexT;
		try{
			_v1=getVertexFromName(v1);
		}
		catch(char*){
			throw "�������� ����� ������� 1";
		};
		try{
			_v2=getVertexFromName(v2);
		}
		catch(char*){
			throw "�������� ����� ������� 2";
		}
		if(!deleteEdge(_v1, _v2))
			return false;
		
		return true;
	};
    bool deleteEdge(VertexT *v1, VertexT *v2) {
        if(data->deleteEdge(getIndex(v1), getIndex(v2))) {
            --edgeCounter;
            if(directed == false)
                data->deleteEdge(getIndex(v2), getIndex(v1));
            return true;
        } else
            return false;
    }
    bool hasEdge(VertexT *v1, VertexT *v2) {
        int ind1, ind2;
        try { 
			ind1 = getIndex(v1); 
		} 
		catch(int &a) { 
			return false; 
		}
        try {
			ind2 = getIndex(v2); 
		}
		catch(int &a) 
		{ 
			return false; 
		}
        return data->hasEdge(getIndex(v1), getIndex(v2));
    }
    bool hasEdge(int v1, int v2) {
        if(v1 < 0 || v1 >= vertexVector.size()) 
			return false;
        if(v2 < 0 || v2 >= vertexVector.size()) 
			return false;
        return data->hasEdge(v1, v2);
    }
    EdgeT* getEdge(VertexT *v1, VertexT *v2) {
		EdgeT* e;
		try{
			e=data->getEdge(getIndex(v1), getIndex(v2));
		}
		catch(int){
			throw "        False";
		}
        return e;
    }
	bool write_data_edge(string _v1, string _v2, int data){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			return false;
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			return false;
		}
		e=getEdge(v1, v2);
		e->setData(data);
		return true;
	};
	bool write_weight_edge(string _v1, string _v2, int w){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			return false;
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			return false;
		}
		e=getEdge(v1, v2);
		e->setWeight(w);
		return true;
	};
	int read_data_edge(string _v1, string _v2){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			throw "        False";
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			throw "        False";
		}
		e=getEdge(v1, v2);
		if (e->isDataSet())
			return e->getData();
		else
			throw "        Null";
	};
	int read_weight_edge(string _v1, string _v2){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			throw "        False";
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			throw "        False";
		}
		e=getEdge(v1, v2);
		if (e->isWeightSet())
			return e->getWeight();
		else
			throw "        Null";
	};
	void print_graph() {
		int i, j;
		VertexT* v;
		EdgeT* e;
		if (isDense()) {
			cout << " |";
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << setw(4) << v->getName();
			}
			cout << endl;
			cout << "--|";
			for (i = 0; i <= 4 * getVertexCount(); i++)
				cout << "-";
			cout << endl;
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << " " << v->getName() << "|";
				for (j = 0; j < getVertexCount(); j++) {
					if (hasEdge(i, j)) {
						e = getEdge(getVertex(i), getVertex(j));
						cout << setw(4) << 1;
					}
					else {
						cout << setw(4) << "0";
					}
				}
				cout << endl;
			}
		}
		else {
			GraphListForm<EdgeT>* form = static_cast<GraphListForm<EdgeT>*>(data);
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << "*" << v->getName() << " ->";
				list<typename GraphListForm<EdgeT>::Node> l = form->edgeList[i];
				for (typename list<typename GraphListForm<EdgeT>::Node>::iterator it = l.begin(); it != l.end(); it++) {
					if ((*it).edge != NULL) {
						cout << getVertex((*it).v2)->getName() << "->";
					}
				}
				cout << endl;
			}
		}
	};

		int read_data_vertex(string v1){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				throw "        False";
			};
			int d;
			d=v->getData();
			if (v->dataSet!=false)
				return d;
			else
				throw "        Null";
		};
		bool write_data_vertex(string v1, int d){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				return false;
			};
			v->setData(d);
			return true;
		};
		bool write_name_vertex(string v1, string str){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				return false;
			};
			v->setName(str);
			return true;
		};


    //��������� ������
    
	//�������� ������
    
	class VertexIterator {
        Graph<VertexT, EdgeT> *graph; //�������������� ����
        bool end;
        int current;
    public:
        VertexIterator(Graph<VertexT, EdgeT> &g) {
            graph = &g;
            begin();
        }
        //��������� � ������
        void begin() {
            if(graph->vertexVector.size() == 0) {
                end = true;
                return;
            }
            current = 0;
            end = false;
			return;
        }
        //����� �� ��������� �������
        void operator++() {
            if(end){
                current=-1;
				return;
			}
            current++;
            if(graph->vertexVector.size()-1 == current)
                end = true;
            return;
        };
		void toend(){
			current=graph->vertexVector.size()-1;
			end=true;
			return;
		};
        //�������� �� �����
        bool onEnd() {
            return end;
        }
        //��������� �������
        VertexT* operator*() {
            if(current >= graph->vertexVector.size() || current==-1)
                throw 1;
            return graph->vertexVector[current];
        }
		string read_vertex() {
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			string d;
			d=v->getName();
			if (v->nameSet!=false)
				return d;
			else
				throw "        Null";
        }
		int read_data_vertex(){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			int d;
			d=v->getData();
			if (v->dataSet!=false)
				return d;
			else
				throw "        Null";
		};
		void write_data_vertex(int d){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			v->setData(d);
			return;
		};
		void write_name_vertex(string str){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			v->setName(str);
			return;
		};
		
	};

    //�������� ����

    class EdgeIterator {
        Graph<VertexT, EdgeT> *graph; //�������������� ����
        typename GraphMatrixForm<EdgeT>::EdgeIterator *mIt;
        typename GraphListForm<EdgeT>::EdgeIterator *lIt;
        bool useM;
    public:
		int curV1, curV2;
		bool end;
        EdgeIterator(Graph<VertexT, EdgeT> &g) {
            graph = &g;
            if(graph->isDense()) {
                mIt = new GraphMatrixForm<EdgeT>::EdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data));
                useM = true;
            } else {
                lIt = new GraphListForm<EdgeT>::EdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data));
                useM = false;
            }
            begin();
        }
        //��������� � ������
        bool begin() {
            if(useM) return 
				mIt->begin();
            else 
				return lIt->begin();
        }
        //����� �� ��������� �������
        bool operator++() {
            if(useM)
                return mIt->next();
            else
                return lIt->next();
        }
        //�������� �� �����
        bool onEnd() {
            //return end;
            if(useM)
                return mIt->onEnd();
            else
                return lIt->onEnd();
        }
		bool toend(){
			if(useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
        //��������� �����
        EdgeT* operator*() {
			if (onEnd())
				throw "        Null";
            if(useM)
                return mIt->getEdge();
            else
                return lIt->getEdge();
        }
		string read_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			VertexT *_v1, *_v2;
			int w;
			string str1, str2, str3, str4;
			_v1=e->getVertex1();
			_v2=e->getVertex2();
			if (_v1->nameSet==false)
				str1="Null";
			else
				str1=_v1->getName();
			if (_v2->nameSet==false)
				str2="Null";
			else
				str2=_v2->getName();
			if (e->isWeightSet()==false)
				str3="Null";
			else
				str3=i_to_s(e->getWeight());
			str4="        ��������� �������: "+str1+". �������� �������: "+str2+". ��� �����: "+str3;
			return str4;
		}
		int read_data_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			int a;
			if (e->isDataSet())
				return e->getData();
			else
				throw "        Null";
		};
		bool write_data_edge(int d){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setData(d);
			return true;
		};
		bool write_weight_edge(int w){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setWeight(w);
			return true;
		}
    };

    //�������� ��������� ����

    class OutputEdgeIterator {
        Graph<VertexT, EdgeT> *graph; //�������������� ����
        typename GraphMatrixForm<EdgeT>::OutputEdgeIterator *mIt;
        typename GraphListForm<EdgeT>::OutputEdgeIterator *lIt;
        bool useM;
    public:
		int curV1, curV2;
		bool end;
        OutputEdgeIterator(Graph<VertexT, EdgeT> &g, VertexT &v) {
            graph = &g;
            curV1 = graph->getIndex(&v);
            if(graph->isDense()) {
                mIt = new GraphMatrixForm<EdgeT>::OutputEdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data), curV1);
                useM = true;
            } else {
                lIt = new GraphListForm<EdgeT>::OutputEdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data), curV1);
                useM = false;
            }
            begin();
        }
        //��������� � ������
        bool begin() {
            if(useM)
                return mIt->begin();
            else
                return lIt->begin();
        }
        //����� �� ��������� �������
        bool operator++() {
            if(useM)
                return mIt->next();
            else
                return lIt->next();
        }
		bool toend(){
			if(useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
        //�������� �� �����
        bool onEnd() {
            if(useM)
                return mIt->onEnd();
            else
                return lIt->onEnd();
        }
        //��������� �����
        EdgeT* operator*() {
			if (onEnd())
				throw "        False";
            if(useM)
                return mIt->getEdge();
            else
                return lIt->getEdge();
        }
		string read_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			VertexT *_v1, *_v2;
			int w;
			string str1, str2, str3, str4;
			_v1=e->getVertex1();
			_v2=e->getVertex2();
			if (_v1->nameSet==false)
				str1="Null";
			else
				str1=_v1->getName();
			if (_v2->nameSet==false)
				str2="Null";
			else
				str2=_v2->getName();
			if (e->isWeightSet()==false)
				str3="Null";
			else
				str3=i_to_s(e->getWeight());
			str4="        ��������� �������: "+str1+". �������� �������: "+str2+". ��� �����: "+str3;
			return str4;
		}
		int read_data_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			int a;
			if (e->isDataSet())
				return e->getData();
			else
				throw "        Null";
		};
		bool write_data_edge(int d){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setData(d);
			return true;
		};
		bool write_weight_edge(int w){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setWeight(w);
			return true;
		}
    };

	//template<class VertexT, class EdgeT>
	class Task1 {
		int** Adj;			//������� ������� ������
		int* c;			//���, �� ������� ���������� �������
		int n;				//��� ���� � �����	
		int v0 = 0;		//��������� �������
		int cd = 0;
		int* exit;		// �����
		Graph<VertexT, EdgeT>* graph; //�������������� ���� 				

	public:
		typename Graph<VertexT, EdgeT>::OutputEdgeIterator* out;

		Task1(Graph<VertexT, EdgeT>& g) {
			graph = &g;
		}
		Task1(const Graph<VertexT, EdgeT>& T) {
			graph = T.graph;
		}
		~Task1() {
			delete �, Adj, exit;
		}
		bool restart(int v0, int n) {
			
			this->n = n;
			this->v0 = v0;
			cd = graph->getVertexCount();

			c = new int [cd];
			exit = new int[cd];
			Adj = new int* [cd];

			for (int i = 0; i < cd; i++) {
				Adj[i] = new int[cd];
					c[i] = -1;
			}
			c[v0] = v0;
			exit[0] = v0;
			search();
			if (!DFS_Visit(1)) return false;

			result(n);
			return true;
		}
		int DFS_Visit(int k) {

			int v, q1 = 0;
			for (v = 0; v < cd && !q1; v++)
			{
				if (Adj[exit[k - 1]][v] > 0)
				{
					if (k == n && v == v0) q1 = 1; // show(n);
					else if (c[v] == -1)
					{
						c[v] = k; exit[k] = v;
						q1 = DFS_Visit(k + 1);
						if (!q1) c[v] = -1;
					}
					else continue;
				}
			}
			return q1;
		}

		void search() {
			int vv1, vv2;
			for (int u = 0; u < cd; u++) {
				try {
					out = new Graph<VertexT, EdgeT>::OutputEdgeIterator(*graph, *graph->getVertex(u));
				}
				catch (char*) {
					continue;
				}
				if (!out->begin()) {
					continue;
				}

				for (int j = 0; j < cd; j++) {
					Adj[u][j] = 0;
					if (graph->hasEdge(u, j)) {
						EdgeT* e = graph->getEdge(graph->getVertex(u), graph->getVertex(j));
						Adj[u][j] = 1;
					}
				}
			}
		}
		void result(int n) {

			for (int i = 0; i < n; i++) {
				cout << exit[i] << " -> ";
			}
			cout << v0;
		}
		//-----------------------------------------------------

	};
};

struct tmpE
{
	int v1, v2;
	int w;
	int s;
};

typedef double TElem;
typedef vector<TElem> TVec;
typedef vector<TVec> TMatrix;
typedef vector<int> TIntVec;
typedef vector<TIntVec> TIntMatrix;
TElem inf = numeric_limits<TElem>::infinity();

//��������� ������ �������:
template <class Elem>
void PrintMatrix(const vector<vector<Elem>>& A)
{
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A[i].size(); j++)
		{
			cout << setw(3) << A[i][j];
		}
		cout << endl;
	}
}

template <class VertexT, class EdgeT>
class Task2 {
private:
	Graph<VertexT, EdgeT>* graph;   //�������������� ����
	int n;							//����� ������ �����
	stringstream res;               //���������

	struct TDiam
	{
		TElem max;	//�������� ��������
		int i, j;	//������� �������
	};
	void GenerateWeightMatrix(TMatrix& W)
	{
		//���������� ������� ����� ������ ���� �����:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (graph->hasEdge(i, j))
				{
					EdgeT* e = graph->getEdge(graph->getVertex(i), graph->getVertex(j));
					if (e->isWeightSet())
						W[i][j] = e->getWeight();
					else
						throw exception("�� ���������� ��� �����");
				}
				else
				{
					W[i][j] = inf;
				}
			}
		}
	}
	void Floyd(const TMatrix& W, TMatrix& D, TIntMatrix& P)
	{
		//������������� ������� ����� ���������� �����:
		for (int i = 0; i < n; i++)
		{
			D[i][i] = 0;
			for (int j = 0; j < n; j++)
			{
				if (i != j)
				{
					D[i][j] = W[i][j];
				}
			}
		}
		//������������� ������� ����������������:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i != j && W[i][j] != inf)
				{
					P[i][j] = i;
				}
				else
				{
					P[i][j] = -1;
				}
			}
		}
		//�������� ������:
		for (int k = 0; k < n; k++)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					if (D[i][j] > D[i][k] + D[k][j])
					{
						D[i][j] = D[i][k] + D[k][j];
						P[i][j] = P[k][j];
					}
				}
			}
		}
	}
	TDiam FindDiam(const TMatrix& D)
	{
		TDiam diam = { -inf, -1, -1 };
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (D[i][j] != inf && D[i][j] > diam.max)
				{
					diam.max = D[i][j];
					diam.i = i;
					diam.j = j;
				}
			}
		}
		return diam;
	}
	void PrintPath(const TIntMatrix& P, int s, int f)
	{
		if (f == s)
		{
			res << f << " -> ";
		}
		else
		{
			PrintPath(P, s, P[s][f]);
			res << f << " -> ";
		}
	}
public:
	//������������-�����������
	Task2(Graph<VertexT, EdgeT>* g) : graph(g)
	{
		restart();
	}
	Task2(const Task2<VertexT, EdgeT>& t1) : graph(t1.g), n(t1.n) { }
	void set(Graph<VertexT, EdgeT>* g)
	{
		graph = g;
		restart();
	}
	void restart()
	{
		res.str("");
		n = graph->getVertexCount();
		if ((graph->getEdgeCount()) < 1)
			throw exception("� ����� ��� ����");
		TMatrix W(n, TVec(n));			//������� �����
		GenerateWeightMatrix(W);		//��������� ������� �����
		TMatrix D(n, TVec(n));			//������� ����� ���������� �����
		TIntMatrix P(n, TIntVec(n));	//������� ����������������
		Floyd(W, D, P);
		TDiam diam = FindDiam(D);		//����� �������� �����
		res << "������� ����� = " << diam.max << endl;
		if (diam.max < inf)
		{
			res << "����, ��������������� ��������:" << endl;
			PrintPath(P, diam.i, diam.j);
			res << endl;
		}
	}
	string result()
	{
		return res.str();
	}
};


class QueueNode{
public:
	float PriorityWeight;
	int Parent;
	int QueIndex; 
	QueueNode(float w, int p){
		PriorityWeight=w;
		Parent=p;
	}
	
};   


class MyObj{
public:
	bool operator()(QueueNode* w1, QueueNode* w2){
		if (w1->PriorityWeight>w2->PriorityWeight)
			return true;
		else
			return false;
	};
};

class MinEdge{	
public: 	
	int Pr;
	int V;
	int Weight;
	MinEdge(int v, int w, int p){
		Pr = p;
		V = v;
		Weight=w;
	};
};

//==============

int enter_menu(){
	int menu;
	cin>>menu;
	while (!cin.good()){
		cout<<"�������� ������ ������!"<<endl;
		cout<<"������� ������ ����� ����: ";
		cin.clear();
		_flushall();
		cin>>menu;
	}
	return menu;
}

void main()
{
	int menu;
	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>> *graph;
	Vertex<string, int> *vertex;
	Edge<Vertex<string, int>, int, int> *edge;

	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator *vIt;
    Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator *eIt;
    Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator *oIt;
	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::Task1* Tt1;

	setlocale (0, "Rus");
	//system("mode 110,50");
	cout<<"�������� ����� ����: "<<endl;
	cout<<"1 - ������� ���� � V ���������, ��� �����, ���� D, ����� ����-��������� F"<<endl;
	cout<<"2 - ������� ������ L-���� � ������� ������ ������ � ����"<<endl;
	cout<<"3 - ������� ���� � V ���������, � E ���������� �������, ���� D , ����� F"<<endl;
	cout<<"-1 - �����"<<endl;
	bool flag1=false;
	while (!flag1){
		cout<<">>";
		menu=enter_menu();
		switch (menu){
		case 1:{
			int v;
			bool directed, dense;
			string dir, den;
			cout<<"������� ���������� ������: ";
			cin>>v;
			cout<<"�������, ��������������� �� ����: ";
			cin>>dir;
			cout<<"������� ����� ����� (0 - �������, 1 - ������): ";
			cin>>den;
			if (dir=="true")
				directed=true;
			else
				directed=false;
			if (den=="true")
				dense=true;
			else
				dense=false;
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(v, directed, dense);
			cout<<"���� ������"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case 2:{
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>();
			cout<<"���� ������"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case 3:{
			int v, e;
			bool directed, dense;
			string dir, den;
			cout<<"������� ���������� ������: ";
			cin>>v;
		    cout<<"������� ���������� ����: ";
			cin>>e;
			cout<<"�������, ��������������� �� ����: ";
			cin>>dir;
			cout<<"������� ����� ����� (1 - �������, 0 - ������): ";
			cin>>den;
			if (dir=="true" || dir=="True")
				directed=true;
			else
				directed=false;
			if (den=="true" || dir=="True")
				dense=true;
			else
				dense=false;
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(v, e, directed, dense);
			cout<<"���� ������"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case -1: {
			return;
				 };
			break;
		default: {
			cout<<"������� ������ ����� ����!"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ������ �����: ";
				 };
		}
	}
	cout<<"�������� ����� ����: "<<endl;
	cout << "=================================================================" << endl;
	cout<<"0 - ����������� ����"<<endl;
	cout<<"1 - ����� ����� ������ � �����"<<endl;
	cout<<"2 - ����� ����� ���� � �����"<<endl;
	cout<<"3 - ����� ���� �����"<<endl;
	cout<<"4 - ����� ����� ������������� �����"<<endl;
	cout<<"5 - ����� ������������ ������������ �����"<<endl;
	cout << "=================================================================" << endl;
	cout<<"6 - ������������� ���� � L-�����"<<endl;
	cout<<"7 - ������������� ���� � M-�����"<<endl;
	cout << "=================================================================" << endl;
	cout<<"8 - �������� ������� � �����"<<endl;
	cout<<"9 - ������� ������� �� �����"<<endl;
	cout << "=================================================================" << endl;
	cout<<"10 - �������� ����� � ����"<<endl;
	cout<<"11 - ������� ����� �� �����"<<endl;
	cout << "=================================================================" << endl;
	cout<<"12 - �������� ��� ��������"<<endl;
	cout<<"13 - �������� ������"<<endl;
	cout<<"14 - �������� ����"<<endl;
	cout<<"15 - �������� ��������� ����"<<endl;
	cout<<"16 - �������� ��� ������"<<endl;
	cout << "=================================================================" << endl;
	cout<<"20 - ����� �������� ����� �� �������� �������"<<endl;
	cout<<"21 - ����������� �������� � ����, �����. �������� (�������� ������)"<<endl;
	cout << "=================================================================" << endl;
	cout<<"-1 - �����"<<endl;
	while (2>1){
		cout<<">>";
		menu=enter_menu();
		switch (menu){
		case 0:{
			cout<<"������ �����: "<<endl;
			graph->print_graph();
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 1:{
			cout<<"����� ������ � �����: "<<graph->getVertexCount()<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 2:{
			cout<<"����� ���� � �����: "<<graph->getEdgeCount()<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 3:{
			cout<<"��� �����: ";
			if (graph->isDirected()==true)
				cout<<"���������������"<<endl;
			else
				cout<<"�����������������"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 4:{
			cout<<"����� ������������� �����: ";
			if (graph->isDense()==true)
				cout<<"M-����"<<endl;
			else
				cout<<"L-����"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 5:{
			cout<<"����������� ������������: "<<graph->getRatio()<<endl;
			   };
			break;
		case 6:{
			graph->toSparseGraph();
			cout<<"���� ������������"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 7:{
			graph->toDenseGraph();
			cout<<"���� ������������"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 8:{
			try{
				graph->insertVertex();
			}
			catch (int Error){
				cout<<Error;
			};
			cout<<"������� ��������� � ����"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 9:{
			string v1;
			cout<<"������� ��������� �������: ";
			cin>>v1;
			if (graph->deleteVertex(v1)==true)
				cout<<"������� �������"<<endl;
			else
				cout<<"������ ������� ��� � �����"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
			   };
			break;
		case 10:{
			string v1, v2;
			cout<<"������� �������, �� ������� ������� �����: ";
			cin>>v1;
			cout<<"������� �������, � ������� ������ �����: ";
			cin>>v2;
			try{
				graph->insertEdge(v1, v2);
			}
			catch(char* Error){
				cout<<Error<<endl;
				cout<<"===================================="<<endl;
			    cout<<"������� ����� ����: ";
				break;
			};
			cout<<"����� ��������� � ����"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
				};
			break;
		case 11:{
			string v1, v2;
			bool rez;
			cout<<"������� �������, �� ������� ������� �����: ";
			cin>>v1;
			cout<<"������� �������, � ������� ������ �����: ";
			cin>>v2;
			try{
				rez=graph->deleteEdge(v1, v2);
			}
			catch(char* Error){
				cout<<Error<<endl;
				cout<<"===================================="<<endl;
			    cout<<"������� ����� ����: ";
				break;
			};
			if (rez)
				cout<<"����� �������"<<endl;
			else
				cout<<"�����, ����������� ����� �������� �������, �� ����������"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
				};
			break;
		case 12:{
			bool flag=false;
			cout<<"�������� ��� ��������:"<<endl;
			cout<<"        1 - ��������� ������ � �������"<<endl;
			cout<<"        2 - �������� ������ � �������"<<endl;
			cout<<"        3 - �������� ����� ��� �������"<<endl;
			cout<<"        -1 -����� �� ����"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
					case 1:{
						int cur;
						string v;
						cout<<"        ������� ��� �������: ";
						cin>>v;
						try{
							cur=graph->read_data_vertex(v);
						}
						catch (char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ������ �������: "<<cur<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case 2:{
						int cur;
						string v;
						bool rez;
						cout<<"        ������� ��� �������: ";
						cin>>v;
						cout<<"        ������� ������: ";
						cin>>cur;
						rez=graph->write_data_vertex(v, cur);
						if (!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ������ � ������� ��������"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case 3:{
						bool rez;
						string v, str;
						cout<<"        ������� ������ ��� �������: ";
						cin>>v;
						cout<<"        ������� ����� ���: ";
						cin>>str;
						rez=graph->write_name_vertex(v, str);
						if(!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ��� ������� ������������"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case -1:{
						flag=true;
						cout<<"===================================="<<endl;
							};
						break;
					default: {
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ������ �����: "; 
							 };
						break; 
				};
			};
				}
			break;
		case 13:{
			bool flag=false;
			vIt=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator(*graph);
			cout<<"�������� ������:"<<endl;
			cout<<"        1 - ���������� �������� ������ �� ��������� �������"<<endl;
			cout<<"        2 - ���������� �������� ������ �� �������� �������"<<endl;
			cout<<"        3 - ������� � ��������� �������"<<endl;
			cout<<"        4 - ��������� ������� �� ������� ������� ���������"<<endl;
			cout<<"        5 - ��������� ������ ������� �� ������� ������� ���������"<<endl;
			cout<<"        6 - �������� ������ � ������� �� ������� ������� ���������"<<endl;
			cout<<"        7 - �������� ��� ������� �� ������� ������� ���������"<<endl;
			cout<<"        -1 -����� �� ���� ���������"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					vIt->begin();
			        cout<<"        �������� ������ ����� ���������� �� ������ �������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 2:{
			        vIt->toend();
			        cout<<"        �������� ������ ����� ���������� �� ��������� �������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 3:{
					++*vIt;
			        cout<<"        �������� ������ ����� ���������� �� ��������� �������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 4:{
					string cur;
					try{
						cur=vIt->read_vertex();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������� �� ������� ������� ���������: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=vIt->read_data_vertex();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ������� �� ������� ������� ���������: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 6:{
					int cur;
					cout<<"        ������� ������: ";
					cin>>cur;
					try{
						vIt->write_data_vertex(cur);
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ������� �������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 7:{
					string cur;
					cout<<"        ������� ���: ";
					cin>>cur;
					try{
						vIt->write_name_vertex(cur);
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ��� ������� �������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case -1:{
					flag=true;
					delete vIt;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        ������� ������ ����� ����!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        ������� ������ �����: "; 
						 };
					break;
				}
			}
				};
			break;
		case 14:{
			bool flag=false;
			eIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator(*graph);
			cout<<"�������� ����: "<<endl;
			cout<<"        1 - ���������� �������� ���� �� ��������� �����"<<endl;
			cout<<"        2 - ���������� �������� ���� �� �������� �����"<<endl;
			cout<<"        3 - ������� � ��������� �������"<<endl;
			cout<<"        4 - ��������� ���������, �������� ������� ����� � ���"<<endl<<"        �� ������� ������� ���������"<<endl;
			cout<<"        5 - ��������� ������ ����� �� ������� ������� ���������"<<endl;
			cout<<"        6 - �������� ������ � ����� �� ������� ������� ���������"<<endl;
			cout<<"        7 - �������� ��� ����� �� ������� ������� ���������"<<endl;
			cout<<"        -1 -����� �� ���� ���������"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					bool rez;
					rez=eIt->begin();
					if (rez)
						cout<<"        �������� ���� ����� ���������� �� ������ �����"<<endl;
					else
						cout<<"        False. и��� � ����� ���"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 2:{
					bool rez;
			        rez=eIt->toend();
					if(rez)
						cout<<"        �������� ���� ����� ���������� �� ��������� �����"<<endl;
					else
						cout<<"        False. и��� � ����� ���"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 3:{
					bool rez;
					rez=++*eIt;
					if(rez)
						cout<<"        �������� ���� ����� ���������� �� ��������� �������"<<endl;
					else
						cout<<"        Null"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 4:{
					string cur;
				    try{
						cur=eIt->read_edge();
					}
					catch(char* Error){
						cout<<"        Null"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					};
			        cout<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=eIt->read_data_edge();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ����� �� ������� ������� ���������: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 6:{
					int cur;
					bool rez;
					cout<<"        ������� ������: ";
					cin>>cur;
					rez=eIt->write_data_edge(cur);
					if (!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ����� �������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 7:{
				    int w;
					bool rez;
					cout<<"        ������� ���: ";
					cin>>w;
					rez=eIt->write_weight_edge(w);
					if(!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ��� ����� ������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case -1:{
					flag=true;
					delete eIt;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        ������� ������ ����� ����!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        ������� ������ �����: "; 
						 };
					break;
				}
			}
			};
			break;
        case 15:{
			bool flag=false;
			int v;
			cout<<"������� �������: ";
			cin>>v;
			oIt= new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator(*graph, *graph->getVertex(v));
			cout<<"�������� ��������� ����: "<<endl;
			cout<<"        1 - ���������� �������� ��������� ���� �� ��������� �����"<<endl;
			cout<<"        2 - ���������� �������� ��������� ���� �� �������� �����"<<endl;
			cout<<"        3 - ������� � ��������� �������"<<endl;
			cout<<"        4 - ��������� ���������, �������� ������� ����� � ���"<<endl<<"        �� ������� ������� ���������"<<endl;
			cout<<"        5 - ��������� ������ ����� �� ������� ������� ���������"<<endl;
			cout<<"        6 - �������� ������ � ����� �� ������� ������� ���������"<<endl;
			cout<<"        7 - �������� ��� ����� �� ������� ������� ���������"<<endl;
			cout<<"        -1 -����� �� ���� ���������"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					bool rez;
					rez=oIt->begin();
					if (rez)
						cout<<"        �������� ��������� ���� ����� ���������� �� ������ �����"<<endl;
					else
						cout<<"        False. и��� � ����� ���"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 2:{
					bool rez;
			        rez=oIt->toend();
					if(rez)
						cout<<"        �������� ��������� ���� ����� ���������� �� ��������� �����"<<endl;
					else
						cout<<"        False. и��� � ����� ���"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 3:{
					bool rez;
					rez=++*oIt;
					if(rez)
						cout<<"        �������� ��������� ���� ����� ���������� �� ��������� �������"<<endl;
					else
						cout<<"        Null"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 4:{
					string cur;
				    try{
						cur=oIt->read_edge();
					}
					catch(char* Error){
						cout<<"        Null"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					};
			        cout<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=oIt->read_data_edge();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ����� �� ������� ������� ���������: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 6:{
					int cur;
					bool rez;
					cout<<"        ������� ������: ";
					cin>>cur;
					rez=oIt->write_data_edge(cur);
					if (!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ������ ����� �������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case 7:{
				    int w;
					bool rez;
					cout<<"        ������� ���: ";
					cin>>w;
					rez=oIt->write_weight_edge(w);
					if(!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        ������� ����� ����: ";
						break;
					}
			        cout<<"        ��� ����� ������� �� ������� ������� ���������"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        ������� ����� ����: ";
					   };
					break;
				case -1:{
					delete oIt;
					flag=true;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        ������� ������ ����� ����!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        ������� ������ �����: "; 
						 };
					break;
				}
			}
			};
			break;
            case 16:{
				bool flag=false;
				cout<<"�������� ��� ������:"<<endl;
				cout<<"        1 - ��������� ��� �����"<<endl;
				cout<<"        2 - ��������� ������ � �����"<<endl;
				cout<<"        3 - �������� ������ � �����"<<endl;
				cout<<"        4 - �������� ��� �����"<<endl;
				cout<<"        -1 -����� �� ����"<<endl;
				cout<<"        >>";
				while (!flag){
					menu=enter_menu();
					switch (menu){
					case 1:{
						string v1, v2;
						int w;
						cout<<"        ������� ��� ��������� �������: ";
						cin>>v1;
						cout<<"        ������� ��� �������� �������: ";
						cin>>v2;
						try{
							w=graph->read_weight_edge(v1, v2);
						}
						catch(char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						};
						cout<<"        ��� �����: "<<w<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case 2:{
						int d;
						string v1, v2;
						cout<<"        ������� ��� ��������� �������: ";
						cin>>v1;
						cout<<"        ������� ��� �������� �������: ";
						cin>>v2;
						try{
							d=graph->read_data_edge(v1, v2);
						}
						catch (char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ������ �����: "<<d<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case 3:{
						int d;
						string v1, v2;
						bool rez;
						cout<<"        ������� ��� ��������� �������: ";
						cin>>v1;
						cout<<"        ������� ��� �������� �������: ";
						cin>>v2;
						cout<<"        ������� ������: ";
						cin>>d;
						rez=graph->write_data_edge(v1, v2, d);
						if (!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ������ � ������� ��������"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case 4:{
						bool rez;
						string v1, v2;
						int w;
						cout<<"        ������� ��� ��������� �������: ";
						cin>>v1;
						cout<<"        ������� ��� �������� �������: ";
						cin>>v2;
						cout<<"        ������� ���: ";
						cin>>w;
						rez=graph->write_weight_edge(v1, v2, w);
						if(!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        ������� ����� ����: ";
							break;
						}
						cout<<"        ��� ����� �����������"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ����� ����: ";
						   };
						break;
					case -1:{
						flag=true;
						cout<<"===================================="<<endl;
							};
						break;
					default: {
						cout<<"        ������� ������ ����� ����!"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        ������� ������ �����: "; 
							 };
						break; 
				};
			};
				}
			break;
		case 20:{
			Tt1 = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::Task1(*graph);
			bool temp = false;
			cout << "������� ������: " << endl;
			int v;
			cin >> v;
			cout << "������� ����� �����: " << endl;
			int cic;
			cin >> cic;

			if (temp) {
				cout << endl << "����: " << endl;
			}
			else cout << "��� �����" << endl;
			temp = Tt1->restart(v, cic);

			cout<<endl<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
				};
			break;
		case 21:{
			Task2<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>* taskTwo;
			taskTwo = new Task2<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(graph);
			
			cout<<"���������:" << endl << taskTwo->result()<<endl;
			cout<<"������ ������"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ����� ����: ";
				};
			break;
		case -1: {
			return;
				 };			break;
		default: {
			cout<<"������� ������ ����� ����!"<<endl;
			cout<<"===================================="<<endl;
			cout<<"������� ������ �����: ";
				 };
			break;
		}
	}

	getch();
	return;
}